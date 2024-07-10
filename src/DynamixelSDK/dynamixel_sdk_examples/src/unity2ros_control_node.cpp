#include <cstdio>
#include <memory>
#include <string>
#include <chrono>
#include <cmath>

#include "dynamixel_sdk/dynamixel_sdk.h"
#include "dynamixel_sdk_custom_interfaces/msg/set_position.hpp"
#include "dynamixel_sdk_custom_interfaces/srv/get_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/moving_status.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_load.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_velocity.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"
#include "geometry_msgs/msg/twist.hpp"

#include "unity2ros_control_node.hpp"

// Control table address for X series (except XL-320)
#define ADDR_OPERATING_MODE 11
#define ADDR_TORQUE_ENABLE 64
#define ADDR_POSITION_D_GAIN 80
#define ADDR_POSITION_I_GAIN 82
#define ADDR_POSITION_P_GAIN 84
#define ADDR_GOAL_PWM 100
#define ADDR_PROFILE_VELOCITY 112
#define ADDR_GOAL_POSITION 116
#define ADDR_PRESENT_PWM 124
#define ADDR_PRESENT_LOAD 126
#define ADDR_PRESENT_VELOCITY 128
#define ADDR_PRESENT_POSITION 132
#define ADDR_VELOCITY_TRAJECTORY 136
#define ADDR_POSITION_TRAJECTORY 140
#define LEN_GOAL_POSITION 4 // Length of the goal position data
#define LEN_PRESENT_POSITION 4 // Length of the present position data 32 Byte
#define LEN_PRESENT_VELOCITY 4
#define LEN_PROFILE_VELOCITY 4 // Length of the profile velocity data
#define LEN_PRESENT_LOAD 2
#define LEN_PRESENT_PWM 2
#define LEN_GOAL_PWM 2 // pwm 16 Byte

// Protocol version
#define PROTOCOL_VERSION 2.0  // Default Protocol version of DYNAMIXEL X series.

// Default setting
#define BAUDRATE 57600  // Default Baudrate of DYNAMIXEL X series
#define DEVICE_NAME "/dev/ttyUSB0"  // [Linux]: "/dev/ttyUSB*", [Windows]: "COM*"

dynamixel::PortHandler * portHandler;
dynamixel::PacketHandler * packetHandler;
dynamixel::GroupSyncWrite * groupSyncWritePosition;
dynamixel::GroupSyncWrite * groupSyncWriteVelocity;
dynamixel::GroupSyncWrite * groupSyncWritePWM;
dynamixel::GroupSyncRead * groupSyncReadPosition;
dynamixel::GroupSyncRead * groupSyncReadVelocity;
dynamixel::GroupSyncRead * groupSyncReadLoad;
dynamixel::GroupSyncRead * groupSyncReadPWM;

using namespace std::chrono_literals;

uint8_t dxl_error = 0;
int dxl_comm_result = COMM_TX_FAIL;

const double wheel_diameter_cm = 0.065;
const double wheel_circumference_cm = M_PI * wheel_diameter_cm;
const double max_position_units_per_revolution = 4095;
const double positional_units_per_cm = max_position_units_per_revolution / wheel_circumference_cm;
const double wheel_base_cm = 0.1475;

// Convert speed (linear and angular) to position units
int32_t convertSpeedToPosition(double speed_cm_s)
{
    return static_cast<int32_t>(speed_cm_s * positional_units_per_cm);
}

PositionControlNode::PositionControlNode()
        : Node("position_control_node")
{
    RCLCPP_INFO(this->get_logger(), "Run position control node");

    // Initialize GroupSyncWrite for goal position
    groupSyncWritePosition = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_GOAL_POSITION, LEN_GOAL_POSITION);
    if (!groupSyncWritePosition) {
        RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncWritePosition");
        return;
    }
    // Initialize GroupSyncWrite for profile velocity
    groupSyncWriteVelocity = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_PROFILE_VELOCITY, LEN_PROFILE_VELOCITY);
    if (!groupSyncWriteVelocity) {
        RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncWriteVelocity");
        return;
    }
    // Initialize GroupSyncWrite for goal PWM
    groupSyncWritePWM = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_GOAL_PWM, LEN_GOAL_PWM);
    if (!groupSyncWritePWM) {
        RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncWritePWM");
        return;
    }
    // Initialize GroupSyncRead for present position
    groupSyncReadPosition = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
    if (!groupSyncReadPosition) {
        RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncRead for position");
        return;
    }
    // Initialize GroupSyncRead for present velocity
    groupSyncReadVelocity = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
    if (!groupSyncReadVelocity) {
        RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncRead for velocity");
        return;
    }
    // Initialize GroupSyncRead for present load
    groupSyncReadLoad = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_LOAD, LEN_PRESENT_LOAD);
    if (!groupSyncReadLoad) {
        RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncRead for load");
        return;
    }
    // Initialize GroupSyncRead for present PWM
    groupSyncReadPWM = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_PWM, LEN_PRESENT_PWM);
    if (!groupSyncReadPWM) {
        RCLCPP_ERROR(this->get_logger(), "Failed to initialize GroupSyncRead for PWM");
        return;
    }

    this->declare_parameter("qos_depth", 10);
    int8_t qos_depth = 0;
    this->get_parameter("qos_depth", qos_depth);

    const auto QOS_RKL10V =
            rclcpp::QoS(rclcpp::KeepLast(qos_depth)).reliable().durability_volatile();

    present_pwm_publisher_ = this->create_publisher<PresentPWM>("present_pwm", QOS_RKL10V);
    present_load_publisher_ = this->create_publisher<PresentLoad>("present_load", QOS_RKL10V);
    present_position_publisher_ = this->create_publisher<PresentPosition>("present_position", QOS_RKL10V);
    present_velocity_publisher_ = this->create_publisher<PresentVelocity>("present_velocity", QOS_RKL10V);

    status_update_timer_ = this->create_wall_timer(
            100ms,  // Adjust the period according to your needs
            std::bind(&PositionControlNode::periodicStatusUpdate, this));

    // Subscription to cmd_vel topic
    cmd_vel_subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "cmd_vel",
            QOS_RKL10V,
            [this](const geometry_msgs::msg::Twist::SharedPtr msg) -> void
            {
                uint32_t profile_velocity = 270; // maximum velocity
                uint32_t goal_pwm = 500;
                double linear_velocity = msg->linear.x;  // in cm/s
                double angular_velocity = msg->angular.z;  // in rad/s

                // Log the received velocities
                RCLCPP_INFO(this->get_logger(), "Received cmd_vel - Linear: %f, Angular: %f", linear_velocity, angular_velocity);

                // Calculate wheel speeds
                double v_left = linear_velocity - (angular_velocity * wheel_base_cm / 2.0);
                double v_right = linear_velocity + (angular_velocity * wheel_base_cm / 2.0);

                RCLCPP_INFO(this->get_logger(), "Computed Wheel Velocities - Left: %f, Right: %f", v_left, v_right);

                // Convert wheel speeds to positional units
                int32_t left_wheel_position = convertSpeedToPosition(v_left);
                int32_t right_wheel_position = convertSpeedToPosition(v_right);

                // Set left wheel position
                RCLCPP_INFO(this->get_logger(), "Left Wheel Position: %d, Right Wheel Position: %d", left_wheel_position, right_wheel_position);

                // Send positions to Dynamixel motors
                std::vector<uint8_t> param_goal_position(LEN_GOAL_POSITION);
                std::vector<uint8_t> param_profile_velocity(LEN_PROFILE_VELOCITY);
                std::vector<uint8_t> param_goal_pwm(LEN_GOAL_PWM);

                // Set left wheel position
                param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(left_wheel_position));
                param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(left_wheel_position));
                param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(left_wheel_position));
                param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(left_wheel_position));
                groupSyncWritePosition->addParam(1, param_goal_position.data());
                param_profile_velocity[0] = DXL_LOBYTE(DXL_LOWORD(profile_velocity));
                param_profile_velocity[1] = DXL_HIBYTE(DXL_LOWORD(profile_velocity));
                param_profile_velocity[2] = DXL_LOBYTE(DXL_HIWORD(profile_velocity));
                param_profile_velocity[3] = DXL_HIBYTE(DXL_HIWORD(profile_velocity));
                groupSyncWriteVelocity->addParam(1, param_profile_velocity.data());
                param_goal_pwm[0] = DXL_LOBYTE(DXL_LOWORD(goal_pwm));
                param_goal_pwm[1] = DXL_HIBYTE(DXL_LOWORD(goal_pwm));
                groupSyncWritePWM->addParam(1, param_goal_pwm.data());

                // Set right wheel position
                param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(right_wheel_position));
                param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(right_wheel_position));
                param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(right_wheel_position));
                param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(right_wheel_position));
                groupSyncWritePosition->addParam(2, param_goal_position.data());
                param_profile_velocity[0] = DXL_LOBYTE(DXL_LOWORD(profile_velocity));
                param_profile_velocity[1] = DXL_HIBYTE(DXL_LOWORD(profile_velocity));
                param_profile_velocity[2] = DXL_LOBYTE(DXL_HIWORD(profile_velocity));
                param_profile_velocity[3] = DXL_HIBYTE(DXL_HIWORD(profile_velocity));
                groupSyncWriteVelocity->addParam(2, param_profile_velocity.data());
                param_goal_pwm[0] = DXL_LOBYTE(DXL_LOWORD(goal_pwm));
                param_goal_pwm[1] = DXL_HIBYTE(DXL_LOWORD(goal_pwm));
                groupSyncWritePWM->addParam(2, param_goal_pwm.data());

                // Transmit sync write packet for position
                dxl_comm_result = groupSyncWritePosition->txPacket();
                if (dxl_comm_result != COMM_SUCCESS) {
                    RCLCPP_ERROR(this->get_logger(), "SyncWrite Position Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
                }
                groupSyncWritePosition->clearParam();

                dxl_comm_result = groupSyncWriteVelocity->txPacket();
                if (dxl_comm_result != COMM_SUCCESS) {
                    RCLCPP_ERROR(this->get_logger(), "SyncWrite Velocity Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
                }
                groupSyncWriteVelocity->clearParam();

                groupSyncWritePWM->clearParam();
            });
}

PositionControlNode::~PositionControlNode()
{
    if (groupSyncWritePosition) {
        delete groupSyncWritePosition;
        groupSyncWritePosition = nullptr;
    }
    if (groupSyncWriteVelocity) {
        delete groupSyncWriteVelocity;
        groupSyncWriteVelocity = nullptr;
    }
    if (groupSyncWritePWM) {
        delete groupSyncWritePWM;
        groupSyncWritePWM = nullptr;
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
    if (groupSyncReadPWM) {
        delete groupSyncReadPWM;
        groupSyncReadPWM = nullptr;
    }
}

void PositionControlNode::periodicStatusUpdate() {
    RCLCPP_INFO(this->get_logger(), "Periodic status update called");
    std::vector<int32_t> ids_to_monitor = {1, 2};

    // Read Present Position
    for (int32_t id : ids_to_monitor) {
        groupSyncReadPosition->addParam(id);
    }
    dxl_comm_result = groupSyncReadPosition->txRxPacket();
    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(this->get_logger(), "GroupSyncRead Position Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
        return;
    }
    std::vector<int32_t> positions;
    for (int32_t id : ids_to_monitor) {
        if (groupSyncReadPosition->isAvailable(id, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION)) {
            int32_t position = groupSyncReadPosition->getData(id, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
            positions.push_back(position);
        } else {
            RCLCPP_ERROR(this->get_logger(), "Failed to get present position data for ID: %d", id);
        }
    }
    groupSyncReadPosition->clearParam();

    // Read Present Velocity
    for (int32_t id : ids_to_monitor) {
        groupSyncReadVelocity->addParam(id);
    }
    dxl_comm_result = groupSyncReadVelocity->txRxPacket();
    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(this->get_logger(), "GroupSyncRead Velocity Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
        return;
    }
    std::vector<int32_t> velocities;
    for (int32_t id : ids_to_monitor) {
        if (groupSyncReadVelocity->isAvailable(id, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY)) {
            int32_t velocity = groupSyncReadVelocity->getData(id, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
            velocities.push_back(velocity);
        } else {
            RCLCPP_ERROR(this->get_logger(), "Failed to get present velocity data for ID: %d", id);
        }
    }
    groupSyncReadVelocity->clearParam();

    // Read Present Load
    for (int32_t id : ids_to_monitor) {
        groupSyncReadLoad->addParam(id);
    }
    dxl_comm_result = groupSyncReadLoad->txRxPacket();
    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(this->get_logger(), "GroupSyncRead Load Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
        return;
    }
    std::vector<int32_t> loads;
    for (int32_t id : ids_to_monitor) {
        if (groupSyncReadLoad->isAvailable(id, ADDR_PRESENT_LOAD, LEN_PRESENT_LOAD)) {
            int32_t load = groupSyncReadLoad->getData(id, ADDR_PRESENT_LOAD, LEN_PRESENT_LOAD);
            loads.push_back(load);
        } else {
            RCLCPP_ERROR(this->get_logger(), "Failed to get present load data for ID: %d", id);
        }
    }
    groupSyncReadLoad->clearParam();

    // Read Present PWM
    for (int32_t id : ids_to_monitor) {
        groupSyncReadPWM->addParam(id);
    }
    dxl_comm_result = groupSyncReadPWM->txRxPacket();
    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(this->get_logger(), "GroupSyncRead PWM Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
        return;
    }
    std::vector<int32_t> pwms;
    for (int32_t id : ids_to_monitor) {
        if (groupSyncReadPWM->isAvailable(id, ADDR_PRESENT_PWM, LEN_PRESENT_PWM)) {
            int32_t pwm = groupSyncReadPWM->getData(id, ADDR_PRESENT_PWM, LEN_PRESENT_PWM);
            pwms.push_back(pwm);
        } else {
            RCLCPP_ERROR(this->get_logger(), "Failed to get present PWM data for ID: %d", id);
        }
    }
    groupSyncReadPWM->clearParam();

    // Publish Data
    publishPresentPosition(ids_to_monitor, positions);
    publishPresentVelocity(ids_to_monitor, velocities);
    publishPresentLoad(ids_to_monitor, loads);
    publishPresentPWM(ids_to_monitor, pwms);
}

void PositionControlNode::publishPresentPWM(const std::vector<int32_t>& ids, const std::vector<int32_t>& pwms) {
    auto message = PresentPWM();
    message.ids = ids;
    std::vector<int32_t> corrected_pwms;

    for (const auto& pwm : pwms) {
        // Convert unsigned 16-bit PWM to signed if needed
        int16_t corrected_pwm = (pwm > 32767) ? pwm - 65536 : pwm;
        corrected_pwms.push_back(corrected_pwm);
    }

    message.pwms = corrected_pwms;
    present_pwm_publisher_->publish(message);
    //RCLCPP_INFO(this->get_logger(), "Published Present PWM: %zu IDs", ids.size());
    //for (size_t i = 0; i < ids.size(); ++i) {
    //    RCLCPP_INFO(this->get_logger(), "ID: %d, PWM: %d", ids[i], corrected_pwms[i]);
    //}
}

void PositionControlNode::publishPresentLoad(const std::vector<int32_t>& ids, const std::vector<int32_t>& loads) {
    auto message = PresentLoad();
    message.ids = ids;
    std::vector<int32_t> corrected_loads;

    for (const auto& load : loads) {
        // Convert unsigned 16-bit load to signed if needed
        int16_t corrected_load = (load > 32767) ? load - 65536 : load;
        corrected_loads.push_back(corrected_load);
    }

    message.loads = corrected_loads;
    present_load_publisher_->publish(message);
    //RCLCPP_INFO(this->get_logger(), "Published Present Load: %zu IDs", ids.size());
    //for (size_t i = 0; i < ids.size(); ++i) {
    //    RCLCPP_INFO(this->get_logger(), "ID: %d, Load: %d", ids[i], corrected_loads[i]);
    //}
}

void PositionControlNode::publishPresentPosition(const std::vector<int32_t>& ids, const std::vector<int32_t>& positions) {
    const double motor_diameter_cm = 6.5;
    const int32_t max_position = 4095;
    const double circumference_cm = motor_diameter_cm * M_PI; // Pi * Diameter

    std::vector<double> distances;
    distances.reserve(positions.size());

    for (const auto& position : positions) {
        double distance = (static_cast<double>(position) / max_position) * circumference_cm;
        distances.push_back(distance);
    }

    auto message = PresentPosition();
    message.ids = ids;
    message.positions = positions;
    message.distances = distances;
    present_position_publisher_->publish(message);
    RCLCPP_INFO(this->get_logger(), "Published Present Position: %zu IDs", ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        RCLCPP_INFO(this->get_logger(), "ID: %d, Position: %d, Distances: %.2f cm", ids[i], positions[i], distances[i]);
    }
}

void PositionControlNode::publishPresentVelocity(const std::vector<int32_t>& ids, const std::vector<int32_t>& velocities) {
    auto message = PresentVelocity();
    message.ids = ids;
    message.velocities = velocities;
    present_velocity_publisher_->publish(message);
    //RCLCPP_INFO(this->get_logger(), "Published Present Velocity: %zu IDs", ids.size());
    //for (size_t i = 0; i < ids.size(); ++i) {
    //    RCLCPP_INFO(this->get_logger(), "ID: %d, Velocity: %d", ids[i], velocities[i]);
    //}
}

void setupDynamixel(int dxl_id)
{
    // Use Position Control Mode
    dxl_comm_result = packetHandler->write1ByteTxRx(
            portHandler,
            dxl_id,
            ADDR_OPERATING_MODE,
            4,
            &dxl_error
    );

    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(rclcpp::get_logger("position_control_node"), "Failed to set Extended Position Control Mode.");
    } else {
        RCLCPP_INFO(rclcpp::get_logger("position_control_node"), "Succeeded to set Extended Position Control Mode.");
    }

    // Enable Torque of DYNAMIXEL
    dxl_comm_result = packetHandler->write1ByteTxRx(
            portHandler,
            dxl_id,
            ADDR_TORQUE_ENABLE,
            1,
            &dxl_error
    );

    if (dxl_comm_result != COMM_SUCCESS) {
        RCLCPP_ERROR(rclcpp::get_logger("position_control_node"), "Failed to enable torque.");
    } else {
        RCLCPP_INFO(rclcpp::get_logger("position_control_node"), "Succeeded to enable torque.");
    }
}

int main(int argc, char * argv[])
{
    portHandler = dynamixel::PortHandler::getPortHandler(DEVICE_NAME);
    packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

    if (!portHandler || !packetHandler) {
        RCLCPP_ERROR(rclcpp::get_logger("position_control_node"), "Failed to initialize PortHandler or PacketHandler");
        return -1;
    }

    if (!portHandler->openPort()) {
        RCLCPP_ERROR(rclcpp::get_logger("position_control_node"), "Failed to open the port!");
        return -1;
    } else {
        RCLCPP_INFO(rclcpp::get_logger("position_control_node"), "Succeeded to open the port.");
    }

    if (!portHandler->setBaudRate(BAUDRATE)) {
        RCLCPP_ERROR(rclcpp::get_logger("position_control_node"), "Failed to set the baudrate!");
        return -1;
    } else {
        RCLCPP_INFO(rclcpp::get_logger("position_control_node"), "Succeeded to set the baudrate.");
    }

    setupDynamixel(1);
    setupDynamixel(2);

    rclcpp::init(argc, argv);

    auto position_control_node = std::make_shared<PositionControlNode>();
    if (!position_control_node) {
        RCLCPP_ERROR(rclcpp::get_logger("position_control_node"), "Failed to initialize PositionControlNode");
        return -1;
    }

    // Use a MultiThreadedExecutor to handle callbacks concurrently
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(position_control_node);
    // Spin until ROS shutdown
    executor.spin();
    rclcpp::shutdown();

    // Disable Torque of DYNAMIXEL
    packetHandler->write1ByteTxRx(
            portHandler,
            BROADCAST_ID,
            ADDR_TORQUE_ENABLE,
            0,
            &dxl_error
    );

    return 0;
}
