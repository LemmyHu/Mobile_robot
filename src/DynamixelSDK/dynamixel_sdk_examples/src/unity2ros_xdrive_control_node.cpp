#include <cstdio>
#include <memory>
#include <string>
#include <chrono>
#include <cmath>
#include <vector>

#include "dynamixel_sdk/dynamixel_sdk.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_velocity.hpp"

// Control table address for X series (except XL-320)
#define ADDR_OPERATING_MODE 11
#define ADDR_TORQUE_ENABLE 64
#define ADDR_PRESENT_POSITION 132
#define ADDR_GOAL_POSITION 116
#define LEN_GOAL_POSITION 4
#define LEN_PRESENT_POSITION 4

// Protocol version
#define PROTOCOL_VERSION 2.0  // Default Protocol version of DYNAMIXEL X series.

// Default setting
#define BAUDRATE 57600  // Default Baudrate of DYNAMIXEL X series
#define DEVICE_NAME "/dev/ttyUSB0"  // [Linux]: "/dev/ttyUSB*", [Windows]: "COM*"

dynamixel::PortHandler *portHandler;
dynamixel::PacketHandler *packetHandler;
dynamixel::GroupSyncWrite *groupSyncWritePosition;
dynamixel::GroupSyncRead *groupSyncReadPosition;

using namespace std::chrono_literals;

class XDriveControlNode : public rclcpp::Node
{
public:
    XDriveControlNode()
            : Node("x_drive_control_node")
    {
        RCLCPP_INFO(this->get_logger(), "Run X drive control node");

        // Initialize GroupSyncWrite for goal position
        groupSyncWritePosition = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_GOAL_POSITION, LEN_GOAL_POSITION);
        if (!groupSyncWritePosition) {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncWritePosition");
            return;
        }

        // Initialize GroupSyncRead for present position
        groupSyncReadPosition = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
        if (!groupSyncReadPosition) {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncReadPosition");
            return;
        }

        this->declare_parameter("qos_depth", 10);
        int8_t qos_depth = 0;
        this->get_parameter("qos_depth", qos_depth);

        const auto QOS_RKL10V = rclcpp::QoS(rclcpp::KeepLast(qos_depth)).reliable().durability_volatile();

        x_drive_left_subscriber_ = this->create_subscription<std_msgs::msg::Int32>(
                "x_drive_target_left",
                QOS_RKL10V,
                [this](const std_msgs::msg::Int32::SharedPtr msg) -> void {
                    RCLCPP_INFO(this->get_logger(), "Received x_drive_target_left: %d", msg->data);
                    sendPositionToMotor(1, msg->data);
                });

        x_drive_right_subscriber_ = this->create_subscription<std_msgs::msg::Int32>(
                "x_drive_target_right",
                QOS_RKL10V,
                [this](const std_msgs::msg::Int32::SharedPtr msg) -> void {
                    RCLCPP_INFO(this->get_logger(), "Received x_drive_target_right: %d", msg->data);
                    sendPositionToMotor(2, msg->data);
                });

        // Publisher for present positions
        present_position_publisher_ = this->create_publisher<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>("present_position", QOS_RKL10V);

        // Timer to periodically read and publish present positions
        read_position_timer_ = this->create_wall_timer(
                100ms, std::bind(&XDriveControlNode::publishPresentPositions, this));
    }

    ~XDriveControlNode()
    {
        if (groupSyncWritePosition) {
            delete groupSyncWritePosition;
            groupSyncWritePosition = nullptr;
        }
        if (groupSyncReadPosition) {
            delete groupSyncReadPosition;
            groupSyncReadPosition = nullptr;
        }
    }

private:
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr x_drive_left_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr x_drive_right_subscriber_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>::SharedPtr present_position_publisher_;
    rclcpp::TimerBase::SharedPtr read_position_timer_;

    void sendPositionToMotor(int motor_id, int32_t position)
    {
        RCLCPP_INFO(this->get_logger(), "Preparing to send position %d to motor ID: %d", position, motor_id);
        std::vector<uint8_t> param_goal_position(LEN_GOAL_POSITION);

        // Prepare position parameter
        param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(position));
        param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(position));
        param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(position));
        param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(position));

        RCLCPP_INFO(this->get_logger(), "Sending position %d to motor ID: %d", position, motor_id);

        // Send position to the specified motor
        if (!groupSyncWritePosition->addParam(motor_id, param_goal_position.data())) {
            RCLCPP_ERROR(this->get_logger(), "Failed to add position param for motor ID: %d", motor_id);
        }

        // Transmit sync write packet for position
        int dxl_comm_result = groupSyncWritePosition->txPacket();
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "SyncWrite Position Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
        }
        groupSyncWritePosition->clearParam();
    }

    void publishPresentPositions()
    {
        std::vector<int32_t> ids = {1, 2};
        std::vector<int32_t> positions;

        // Read Present Position
        for (int32_t id : ids) {
            if (!groupSyncReadPosition->addParam(id)) {
                RCLCPP_ERROR(this->get_logger(), "Failed to addParam for ID: %d", id);
                return;
            }
        }
        int dxl_comm_result = groupSyncReadPosition->txRxPacket();
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "GroupSyncRead Position Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
            return;
        }
        for (int32_t id : ids) {
            if (groupSyncReadPosition->isAvailable(id, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION)) {
                int32_t position = groupSyncReadPosition->getData(id, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
                positions.push_back(position);
            } else {
                RCLCPP_ERROR(this->get_logger(), "Failed to get present position data for ID: %d", id);
                positions.push_back(0);  // Add zero if reading fails
            }
        }
        groupSyncReadPosition->clearParam();

        // Publish the present positions
        auto message = dynamixel_sdk_custom_interfaces::msg::PresentVelocity();
        message.ids = ids;
        message.velocities = positions; // Adjusted to use the positions vector
        present_position_publisher_->publish(message);

        RCLCPP_INFO(this->get_logger(), "Published Present Positions: Left: %d, Right: %d", positions[0], positions[1]);
    }
};

void setupDynamixel(int dxl_id)
{
    uint8_t dxl_error = 0;
    int dxl_comm_result = COMM_TX_FAIL;

    // Use Extended Position Control Mode (Operating Mode 4)
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, dxl_id, ADDR_OPERATING_MODE, 4, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(rclcpp::get_logger("x_drive_control_node"), "Failed to set Extended Position Control Mode for ID: %d. Error: %s", dxl_id, packetHandler->getTxRxResult(dxl_comm_result));
    } else {
        RCLCPP_INFO(rclcpp::get_logger("x_drive_control_node"), "Succeeded to set Extended Position Control Mode for ID: %d", dxl_id);
    }

    // Enable Torque of DYNAMIXEL
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, dxl_id, ADDR_TORQUE_ENABLE, 1, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(rclcpp::get_logger("x_drive_control_node"), "Failed to enable torque for ID: %d. Error: %s", dxl_id, packetHandler->getTxRxResult(dxl_comm_result));
    } else {
        RCLCPP_INFO(rclcpp::get_logger("x_drive_control_node"), "Succeeded to enable torque for ID: %d", dxl_id);
    }
}

int main(int argc, char *argv[])
{
    portHandler = dynamixel::PortHandler::getPortHandler(DEVICE_NAME);
    packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

    if (!portHandler || !packetHandler) {
        RCLCPP_ERROR(rclcpp::get_logger("x_drive_control_node"), "Failed to initialize PortHandler or PacketHandler");
        return -1;
    }

    if (!portHandler->openPort()) {
        RCLCPP_ERROR(rclcpp::get_logger("x_drive_control_node"), "Failed to open the port!");
        return -1;
    } else {
        RCLCPP_INFO(rclcpp::get_logger("x_drive_control_node"), "Succeeded to open the port.");
    }

    if (!portHandler->setBaudRate(BAUDRATE)) {
        RCLCPP_ERROR(rclcpp::get_logger("x_drive_control_node"), "Failed to set the baudrate!");
        return -1;
    } else {
        RCLCPP_INFO(rclcpp::get_logger("x_drive_control_node"), "Succeeded to set the baudrate.");
    }

    setupDynamixel(1);
    setupDynamixel(2);

    rclcpp::init(argc, argv);

    auto x_drive_control_node = std::make_shared<XDriveControlNode>();
    if (!x_drive_control_node) {
        RCLCPP_ERROR(rclcpp::get_logger("x_drive_control_node"), "Failed to initialize VelocityControlNode");
        return -1;
    }

    // Use a MultiThreadedExecutor to handle callbacks concurrently
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(x_drive_control_node);
    // Spin until ROS shutdown
    executor.spin();
    rclcpp::shutdown();

    // Disable Torque of DYNAMIXEL
    uint8_t dxl_error = 0;
    packetHandler->write1ByteTxRx(portHandler, BROADCAST_ID, ADDR_TORQUE_ENABLE, 0, &dxl_error);

    return 0;
}
