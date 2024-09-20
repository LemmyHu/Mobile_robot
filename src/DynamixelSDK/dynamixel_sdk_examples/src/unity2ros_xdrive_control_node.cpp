#include <cstdio>
#include <memory>
#include <string>
#include <chrono>
#include <cmath>
#include <vector>

#include "dynamixel_sdk/dynamixel_sdk.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_load.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_velocity.hpp"

// Control table address for X series (except XL-320)
#define ADDR_OPERATING_MODE 11
#define ADDR_TORQUE_ENABLE 64
#define ADDR_PRESENT_LOAD 126
#define ADDR_PRESENT_POSITION 132
#define ADDR_PRESENT_VELOCITY 128
#define ADDR_GOAL_POSITION 116
#define LEN_GOAL_POSITION 4
#define LEN_PRESENT_POSITION 4
#define LEN_PRESENT_VELOCITY 4
#define LEN_PRESENT_LOAD 2

// Protocol version
#define PROTOCOL_VERSION 2.0  // Default Protocol version of DYNAMIXEL X series.

// Default setting
#define BAUDRATE 57600  // Default Baudrate of DYNAMIXEL X series
#define DEVICE_NAME "/dev/ttyUSB0"  // [Linux]: "/dev/ttyUSB*", [Windows]: "COM*"

dynamixel::PortHandler *portHandler;
dynamixel::PacketHandler *packetHandler;
dynamixel::GroupSyncWrite *groupSyncWritePosition;
dynamixel::GroupSyncRead *groupSyncReadPosition;
dynamixel::GroupSyncRead *groupSyncReadVelocity;
dynamixel::GroupSyncRead *groupSyncReadLoad;

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
        groupSyncReadVelocity = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
        if (!groupSyncReadPosition) {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncReadVelocity");
            return;
        }
        groupSyncReadLoad = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_LOAD, LEN_PRESENT_LOAD);
        if (!groupSyncReadPosition) {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncReadLoad");
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
                    int32_t modified_data = msg->data * 3;
                    RCLCPP_INFO(this->get_logger(), "Received x_drive_target_left: %d, Modified: %d", msg->data, modified_data);
                    sendPositionToMotor(1, modified_data);
                });

        x_drive_right_subscriber_ = this->create_subscription<std_msgs::msg::Int32>(
                "x_drive_target_right",
                QOS_RKL10V,
                [this](const std_msgs::msg::Int32::SharedPtr msg) -> void {
                    int32_t modified_data = msg->data * 3;
                    RCLCPP_INFO(this->get_logger(), "Received x_drive_target_right: %d, Modified: %d", msg->data, modified_data);
                    sendPositionToMotor(2, modified_data);
                });

        // Publisher for present positions
        present_position_publisher_ = this->create_publisher<dynamixel_sdk_custom_interfaces::msg::PresentPosition>("present_position", QOS_RKL10V);
        present_velocity_publisher_ = this->create_publisher<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>("present_velocity", QOS_RKL10V);
        present_load_publisher_ = this->create_publisher<dynamixel_sdk_custom_interfaces::msg::PresentLoad>("present_load", QOS_RKL10V);

        // Timer to periodically read and publish present positions
        read_position_timer_ = this->create_wall_timer(
                100ms, std::bind(&XDriveControlNode::publishPresentData, this));
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
        if (groupSyncReadVelocity) {
            delete groupSyncReadVelocity;
            groupSyncReadVelocity = nullptr;
        }
        if (groupSyncReadLoad) {
            delete groupSyncReadLoad;
            groupSyncReadLoad = nullptr;
        }
    }

private:
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr x_drive_left_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr x_drive_right_subscriber_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentPosition>::SharedPtr present_position_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>::SharedPtr present_velocity_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentLoad>::SharedPtr present_load_publisher_;
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

    void publishPresentData()
    {
        std::vector<int32_t> ids = {1, 2};
        std::vector<int32_t> positions;
        std::vector<int32_t> velocities;
        std::vector<int32_t> loads;

        // Read Present Position
        for (int32_t id : ids) {
            if (!groupSyncReadPosition->addParam(id)) {
                RCLCPP_ERROR(this->get_logger(), "Failed to addParam for ID: %d", id);
                return;
            }
            if (!groupSyncReadVelocity->addParam(id)) {
                RCLCPP_ERROR(this->get_logger(), "Failed to addParam for Velocity ID: %d", id);
                return;
            }
            if (!groupSyncReadLoad->addParam(id)) {
                RCLCPP_ERROR(this->get_logger(), "Failed to addParam for Load ID: %d", id);
                return;
            }
        }

        // Perform the sync read
        int dxl_comm_result = groupSyncReadPosition->txRxPacket();
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "GroupSyncRead Position Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
            return;
        }

        // Read and store the positions
        for (int32_t id : ids) {
            if (groupSyncReadPosition->isAvailable(id, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION)) {
                int32_t position = groupSyncReadPosition->getData(id, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
                positions.push_back(position);
            } else {
                RCLCPP_ERROR(this->get_logger(), "Failed to get present position data for ID: %d", id);
                positions.push_back(0);  // Add zero if reading fails
            }
        }

        // Perform sync read for velocity
        dxl_comm_result = groupSyncReadVelocity->txRxPacket();
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "GroupSyncRead Velocity Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
            return;
        }

        for (int32_t id : ids) {
            if (groupSyncReadVelocity->isAvailable(id, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY)) {
                int32_t velocity = groupSyncReadVelocity->getData(id, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
                velocities.push_back(velocity);
            } else {
                RCLCPP_ERROR(this->get_logger(), "Failed to get present velocity data for ID: %d", id);
                velocities.push_back(0);
            }
        }

        // Perform sync read for load
        dxl_comm_result = groupSyncReadLoad->txRxPacket();
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "GroupSyncRead Load Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
            return;
        }

        for (int32_t id : ids) {
            if (groupSyncReadLoad->isAvailable(id, ADDR_PRESENT_LOAD, LEN_PRESENT_LOAD)) {
                int32_t load = groupSyncReadLoad->getData(id, ADDR_PRESENT_LOAD, LEN_PRESENT_LOAD);
                loads.push_back(load > 32767 ? load - 65536 : load);  // Correct signed value
            } else {
                RCLCPP_ERROR(this->get_logger(), "Failed to get present load data for ID: %d", id);
                loads.push_back(0);
            }
        }

        groupSyncReadPosition->clearParam();
        groupSyncReadVelocity->clearParam();
        groupSyncReadLoad->clearParam();

        auto position_message = dynamixel_sdk_custom_interfaces::msg::PresentPosition();
        position_message.ids = ids;
        position_message.positions = positions;
        present_position_publisher_->publish(position_message);

        auto velocity_message = dynamixel_sdk_custom_interfaces::msg::PresentVelocity();
        velocity_message.ids = ids;
        velocity_message.velocities = velocities;
        present_velocity_publisher_->publish(velocity_message);

        auto load_message = dynamixel_sdk_custom_interfaces::msg::PresentLoad();
        load_message.ids = ids;
        load_message.loads = loads;
        present_load_publisher_->publish(load_message);

        RCLCPP_INFO(this->get_logger(), "Published Present Data: Positions: [%d, %d], Velocities: [%d, %d], Loads: [%d, %d]",
                    positions[0], positions[1], velocities[0], velocities[1], loads[0], loads[1]);
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
