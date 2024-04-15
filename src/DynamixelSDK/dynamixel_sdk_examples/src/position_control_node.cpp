#include <cstdio>
#include <memory>
#include <string>

#include "dynamixel_sdk/dynamixel_sdk.h"
#include "dynamixel_sdk_custom_interfaces/msg/set_position.hpp"
#include "dynamixel_sdk_custom_interfaces/srv/get_position.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"

#include "position_control_node.hpp"

// Control table address for X series (except XL-320)
#define ADDR_OPERATING_MODE 11
#define ADDR_TORQUE_ENABLE 64
#define ADDR_GOAL_POSITION 116
#define ADDR_PRESENT_POSITION 132

// Protocol version
#define PROTOCOL_VERSION 2.0  // Default Protocol version of DYNAMIXEL X series.

// Default setting
#define BAUDRATE 57600  // Default Baudrate of DYNAMIXEL X series
#define DEVICE_NAME "/dev/ttyUSB0"  // [Linux]: "/dev/ttyUSB*", [Windows]: "COM*"

dynamixel::PortHandler * portHandler;
dynamixel::PacketHandler * packetHandler;

uint8_t dxl_error = 0;
uint32_t goal_position_1 = 0;
uint32_t goal_position_2 = 0;
int dxl_comm_result = COMM_TX_FAIL;

PositionControlNode::PositionControlNode()
        : Node("position_control_node")
{
    RCLCPP_INFO(this->get_logger(), "Run position control node");

    this->declare_parameter("qos_depth", 10);
    int8_t qos_depth = 0;
    this->get_parameter("qos_depth", qos_depth);

    const auto QOS_RKL10V =
            rclcpp::QoS(rclcpp::KeepLast(qos_depth)).reliable().durability_volatile();

    set_position_subscriber_ =
            this->create_subscription<SetPosition>(
                    "set_position",
                    QOS_RKL10V,
                    [this](const dynamixel_sdk_custom_interfaces::msg::SetPosition::SharedPtr msg) -> void
                    {
                        uint8_t dxl_error = 0;

                        // Position Value of X series is 4 byte data.
                        // For AX & MX(1.0) use 2 byte data(uint16_t) for the Position Value.
                        uint32_t goal_position_1 = msg->positions[0];  // Convert int32 -> uint32
                        uint32_t goal_position_2 = msg->positions[1];

                        // Write Goal Position (length : 4 bytes)
                        // When writing 2 byte data to AX / MX(1.0), use write2ByteTxRx() instead.
                        dxl_comm_result =
                                packetHandler->write4ByteTxRx(
                                        portHandler,
                                        1,
                                        ADDR_GOAL_POSITION,
                                        goal_position_1,
                                        &dxl_error
                                );
                        dxl_comm_result =
                                packetHandler->write4ByteTxRx(
                                        portHandler,
                                        2,
                                        ADDR_GOAL_POSITION,
                                        goal_position_2,
                                        &dxl_error
                                );

                        if (dxl_comm_result != COMM_SUCCESS) {
                            RCLCPP_INFO(this->get_logger(), "%s", packetHandler->getTxRxResult(dxl_comm_result));
                        } else if (dxl_error != 0) {
                            RCLCPP_INFO(this->get_logger(), "%s", packetHandler->getRxPacketError(dxl_error));
                        } else {
                            RCLCPP_INFO(this->get_logger(), "Set [ID: %d] [Goal Position: %d]", msg->ids[0], msg->positions[0]);
                            RCLCPP_INFO(this->get_logger(), "Set [ID: %d] [Goal Position: %d]", msg->ids[1], msg->positions[1]);
                        }
                    }
            );

    auto get_present_position =
            [this](
                    const std::shared_ptr<GetPosition::Request> request,
                    std::shared_ptr<GetPosition::Response> response) -> void
            {
                // Read Present Position (length : 4 bytes) and Convert uint32 -> int32
                // When reading 2 byte data from AX / MX(1.0), use read2ByteTxRx() instead.
                dxl_comm_result = packetHandler->read4ByteTxRx(
                        portHandler,
                        (uint8_t) request->ids[0],
                        ADDR_PRESENT_POSITION,
                        reinterpret_cast<uint32_t *>(&present_position),
                        &dxl_error
                );
                dxl_comm_result = packetHandler->read4ByteTxRx(
                        portHandler,
                        (uint8_t) request->ids[1],
                        ADDR_PRESENT_POSITION,
                        reinterpret_cast<uint32_t *>(&present_position),
                        &dxl_error
                );

                RCLCPP_INFO(
                        this->get_logger(),
                        "Get [ID: %d] [Present Position: %d]",
                        request->ids[0],
                        present_position
                );
                RCLCPP_INFO(
                        this->get_logger(),
                        "Get [ID: %d] [Present Position: %d]",
                        request->ids[1],
                        present_position
                );

                response->positions.push_back(present_position);
            };

    get_position_server_ = create_service<GetPosition>("get_position", get_present_position);
}

PositionControlNode::~PositionControlNode()
{
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

    setupDynamixel(1);
    setupDynamixel(2);

    // Open Serial Port
    dxl_comm_result = portHandler->openPort();
    if (dxl_comm_result == false) {
        RCLCPP_ERROR(rclcpp::get_logger("position_control_node"), "Failed to open the port!");
        return -1;
    } else {
        RCLCPP_INFO(rclcpp::get_logger("position_control_node"), "Succeeded to open the port.");
    }

    // Set the baudrate of the serial port (use DYNAMIXEL Baudrate)
    dxl_comm_result = portHandler->setBaudRate(BAUDRATE);
    if (dxl_comm_result == false) {
        RCLCPP_ERROR(rclcpp::get_logger("position_control_node"), "Failed to set the baudrate!");
        return -1;
    } else {
        RCLCPP_INFO(rclcpp::get_logger("position_control_node"), "Succeeded to set the baudrate.");
    }

    setupDynamixel(BROADCAST_ID);

    rclcpp::init(argc, argv);

    auto positioncontrolnode = std::make_shared<PositionControlNode>();
    rclcpp::spin(positioncontrolnode);
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