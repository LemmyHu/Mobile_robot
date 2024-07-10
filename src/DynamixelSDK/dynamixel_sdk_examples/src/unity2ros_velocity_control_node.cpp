#include <cstdio>
#include <memory>
#include <string>
#include <chrono>
#include <cmath>
#include <vector>

#include "dynamixel_sdk/dynamixel_sdk.h"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_velocity.hpp"

// Control table address for X series (except XL-320)
#define ADDR_OPERATING_MODE 11
#define ADDR_TORQUE_ENABLE 64
#define ADDR_PRESENT_VELOCITY 128
#define ADDR_GOAL_VELOCITY 104
#define LEN_GOAL_VELOCITY 4
#define LEN_PRESENT_VELOCITY 4

// Protocol version
#define PROTOCOL_VERSION 2.0  // Default Protocol version of DYNAMIXEL X series.

// Default setting
#define BAUDRATE 57600  // Default Baudrate of DYNAMIXEL X series
#define DEVICE_NAME "/dev/ttyUSB0"  // [Linux]: "/dev/ttyUSB*", [Windows]: "COM*"

dynamixel::PortHandler *portHandler;
dynamixel::PacketHandler *packetHandler;
dynamixel::GroupSyncWrite *groupSyncWriteVelocity;
dynamixel::GroupSyncRead *groupSyncReadVelocity;

using namespace std::chrono_literals;

class VelocityControlNode : public rclcpp::Node
{
public:
    VelocityControlNode()
            : Node("velocity_control_node")
    {
        RCLCPP_INFO(this->get_logger(), "Run velocity control node");

        // Initialize GroupSyncWrite for goal velocity
        groupSyncWriteVelocity = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_GOAL_VELOCITY, LEN_GOAL_VELOCITY);
        if (!groupSyncWriteVelocity) {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncWriteVelocity");
            return;
        }

        // Initialize GroupSyncRead for present velocity
        groupSyncReadVelocity = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
        if (!groupSyncReadVelocity) {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncReadVelocity");
            return;
        }

        this->declare_parameter("qos_depth", 10);
        int8_t qos_depth = 0;
        this->get_parameter("qos_depth", qos_depth);

        const auto QOS_RKL10V = rclcpp::QoS(rclcpp::KeepLast(qos_depth)).reliable().durability_volatile();

        // Subscription to cmd_vel topic
        cmd_vel_subscriber_ = this->create_subscription<geometry_msgs::msg::TwistStamped>(
                "unity/cmd_vel",
                QOS_RKL10V,
                [this](const geometry_msgs::msg::TwistStamped::SharedPtr msg) -> void {
                    double linear_velocity = msg->twist.linear.x;  // in m/s
                    double angular_velocity = msg->twist.angular.z;  // in rad/s

                    // Log the received velocities
                    RCLCPP_INFO(this->get_logger(), "Received cmd_vel - Linear: %f, Angular: %f", linear_velocity, angular_velocity);

                    // Calculate wheel speeds
                    double v_left = linear_velocity - (angular_velocity * 3.5 * wheel_base_ / 2.0);
                    double v_right = linear_velocity + (angular_velocity * 3.5 * wheel_base_ / 2.0);

                    RCLCPP_INFO(this->get_logger(), "Computed Wheel Velocities - Left: %f, Right: %f", v_left, v_right);

                    // Convert wheel speeds to Dynamixel units
                    int32_t left_wheel_velocity = convertSpeedToVelocityUnits(v_left);
                    int32_t right_wheel_velocity = convertSpeedToVelocityUnits(v_right);

                    RCLCPP_INFO(this->get_logger(), "Converted Wheel Velocities - Left: %d, Right: %d", left_wheel_velocity, right_wheel_velocity);

                    // Send velocities to Dynamixel motors
                    sendVelocityToMotors(left_wheel_velocity, right_wheel_velocity);
                });

        // Publisher for present velocities
        present_velocity_publisher_ = this->create_publisher<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>("present_velocity", QOS_RKL10V);

        // Timer to periodically read and publish present velocities
        read_velocity_timer_ = this->create_wall_timer(
                100ms, std::bind(&VelocityControlNode::publishPresentVelocities, this));
    }

    ~VelocityControlNode()
    {
        if (groupSyncWriteVelocity) {
            delete groupSyncWriteVelocity;
            groupSyncWriteVelocity = nullptr;
        }
        if (groupSyncReadVelocity) {
            delete groupSyncReadVelocity;
            groupSyncReadVelocity = nullptr;
        }
    }

private:
    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr cmd_vel_subscriber_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>::SharedPtr present_velocity_publisher_;
    rclcpp::TimerBase::SharedPtr read_velocity_timer_;

    // Robot parameters
    const double wheel_diameter_ = 0.065;  // meters
    const double wheel_radius_ = wheel_diameter_ / 2;
    const double wheel_base_ = 0.1475;  // meters

    int32_t convertSpeedToVelocityUnits(double speed_m_s)
    {
        // Convert speed in m/s to RPM
        double wheel_circumference = M_PI * wheel_diameter_;
        double rpm = (speed_m_s / wheel_circumference) * 60.0;

        // Convert RPM to Dynamixel velocity units
        int32_t velocity_units = static_cast<int32_t>((rpm / 0.229) * 0.25472);  // 0.229 RPM per unit for Dynamixel
        return velocity_units;
    }

    void sendVelocityToMotors(int32_t left_wheel_velocity, int32_t right_wheel_velocity)
    {
        std::vector<uint8_t> param_goal_velocity(LEN_GOAL_VELOCITY);

        // Send left wheel velocity
        param_goal_velocity[0] = DXL_LOBYTE(DXL_LOWORD(left_wheel_velocity));
        param_goal_velocity[1] = DXL_HIBYTE(DXL_LOWORD(left_wheel_velocity));
        param_goal_velocity[2] = DXL_LOBYTE(DXL_HIWORD(left_wheel_velocity));
        param_goal_velocity[3] = DXL_HIBYTE(DXL_HIWORD(left_wheel_velocity));
        if (!groupSyncWriteVelocity->addParam(1, param_goal_velocity.data())) {
            RCLCPP_ERROR(this->get_logger(), "Failed to add left wheel velocity param");
        }

        // Send right wheel velocity
        param_goal_velocity[0] = DXL_LOBYTE(DXL_LOWORD(right_wheel_velocity));
        param_goal_velocity[1] = DXL_HIBYTE(DXL_LOWORD(right_wheel_velocity));
        param_goal_velocity[2] = DXL_LOBYTE(DXL_HIWORD(right_wheel_velocity));
        param_goal_velocity[3] = DXL_HIBYTE(DXL_HIWORD(right_wheel_velocity));
        if (!groupSyncWriteVelocity->addParam(2, param_goal_velocity.data())) {
            RCLCPP_ERROR(this->get_logger(), "Failed to add right wheel velocity param");
        }

        // Transmit sync write packet for velocity
        int dxl_comm_result = groupSyncWriteVelocity->txPacket();
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "SyncWrite Velocity Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
        }
        groupSyncWriteVelocity->clearParam();
    }

    void publishPresentVelocities()
    {
        std::vector<int32_t> ids = {1, 2};
        std::vector<int32_t> velocities;

        // Read Present Velocity
        for (int32_t id : ids) {
            if (!groupSyncReadVelocity->addParam(id)) {
                RCLCPP_ERROR(this->get_logger(), "Failed to addParam for ID: %d", id);
                return;
            }
        }
        int dxl_comm_result = groupSyncReadVelocity->txRxPacket();
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
                velocities.push_back(0);  // Add zero if reading fails
            }
        }
        groupSyncReadVelocity->clearParam();

        // Publish the present velocities
        auto message = dynamixel_sdk_custom_interfaces::msg::PresentVelocity();
        message.ids = ids;
        message.velocities = velocities;
        present_velocity_publisher_->publish(message);

        RCLCPP_INFO(this->get_logger(), "Published Present Velocities: Left: %d, Right: %d", velocities[0], velocities[1]);
    }
};

void setupDynamixel(int dxl_id)
{
    uint8_t dxl_error = 0;
    int dxl_comm_result = COMM_TX_FAIL;

    // Use Velocity Control Mode
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, dxl_id, ADDR_OPERATING_MODE, 1, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(rclcpp::get_logger("velocity_control_node"), "Failed to set Velocity Control Mode for ID: %d. Error: %s", dxl_id, packetHandler->getTxRxResult(dxl_comm_result));
    } else {
        RCLCPP_INFO(rclcpp::get_logger("velocity_control_node"), "Succeeded to set Velocity Control Mode for ID: %d", dxl_id);
    }

    // Enable Torque of DYNAMIXEL
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, dxl_id, ADDR_TORQUE_ENABLE, 1, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(rclcpp::get_logger("velocity_control_node"), "Failed to enable torque for ID: %d. Error: %s", dxl_id, packetHandler->getTxRxResult(dxl_comm_result));
    } else {
        RCLCPP_INFO(rclcpp::get_logger("velocity_control_node"), "Succeeded to enable torque for ID: %d", dxl_id);
    }
}

int main(int argc, char *argv[])
{
    portHandler = dynamixel::PortHandler::getPortHandler(DEVICE_NAME);
    packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

    if (!portHandler || !packetHandler) {
        RCLCPP_ERROR(rclcpp::get_logger("velocity_control_node"), "Failed to initialize PortHandler or PacketHandler");
        return -1;
    }

    if (!portHandler->openPort()) {
        RCLCPP_ERROR(rclcpp::get_logger("velocity_control_node"), "Failed to open the port!");
        return -1;
    } else {
        RCLCPP_INFO(rclcpp::get_logger("velocity_control_node"), "Succeeded to open the port.");
    }

    if (!portHandler->setBaudRate(BAUDRATE)) {
        RCLCPP_ERROR(rclcpp::get_logger("velocity_control_node"), "Failed to set the baudrate!");
        return -1;
    } else {
        RCLCPP_INFO(rclcpp::get_logger("velocity_control_node"), "Succeeded to set the baudrate.");
    }

    setupDynamixel(1);
    setupDynamixel(2);

    rclcpp::init(argc, argv);

    auto velocity_control_node = std::make_shared<VelocityControlNode>();
    if (!velocity_control_node) {
        RCLCPP_ERROR(rclcpp::get_logger("velocity_control_node"), "Failed to initialize VelocityControlNode");
        return -1;
    }

    // Use a MultiThreadedExecutor to handle callbacks concurrently
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(velocity_control_node);
    // Spin until ROS shutdown
    executor.spin();
    rclcpp::shutdown();

    // Disable Torque of DYNAMIXEL
    uint8_t dxl_error = 0;
    packetHandler->write1ByteTxRx(portHandler, BROADCAST_ID, ADDR_TORQUE_ENABLE, 0, &dxl_error);

    return 0;
}
