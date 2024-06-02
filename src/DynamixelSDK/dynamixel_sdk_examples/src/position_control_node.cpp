#include <cstdio>
#include <memory>
#include <string>
#include <chrono>

#include "dynamixel_sdk/dynamixel_sdk.h"
#include "dynamixel_sdk_custom_interfaces/msg/set_position.hpp"
#include "dynamixel_sdk_custom_interfaces/srv/get_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/moving_status.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_load.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_velocity.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"

#include "position_control_node.hpp"

// Control table address for X series (except XL-320)
#define ADDR_OPERATING_MODE 11
#define ADDR_TORQUE_ENABLE 64
#define ADDR_POSITION_D_GAIN 80
#define ADDR_POSITION_I_GAIN 82
#define ADDR_POSITION_P_GAIN 84
#define ADDR_GOAL_PWM 100
#define ADDR_PROFILE_VELOCITY 112
#define ADDR_GOAL_POSITION 116
#define ADDR_MOVING_STATUS 123
#define ADDR_PRESENT_PWM 124
#define ADDR_PRESENT_LOAD 126
#define ADDR_PRESENT_VELOCITY 128
#define ADDR_PRESENT_POSITION 132
#define ADDR_VELOCITY_TRAJECTORY 136
#define ADDR_POSITION_TRAJECTORY 140
#define LEN_GOAL_POSITION 4 // Length of the goal position data
#define LEN_PRESENT_POSITION 4 // Length of the present position data 32 Byte
#define LEN_PROFILE_VELOCITY 4 // Length of the profile velocity data
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
dynamixel::GroupSyncRead * groupSyncRead;

using namespace std::chrono_literals;

uint8_t dxl_error = 0;
int dxl_comm_result = COMM_TX_FAIL;

PositionControlNode::PositionControlNode()
        : Node("position_control_node")
{
    RCLCPP_INFO(this->get_logger(), "Run position control node");

    // Initialize GroupSyncWrite for goal position
    groupSyncWritePosition = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_GOAL_POSITION, LEN_GOAL_POSITION);
    // Initialize GroupSyncWrite for profile velocity
    groupSyncWriteVelocity = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_PROFILE_VELOCITY, LEN_PROFILE_VELOCITY);
    // Initialize GroupSyncWrite for goal PWM
    groupSyncWritePWM = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_GOAL_PWM, LEN_GOAL_PWM);
    // Initialize GroupSyncRead for present position
    groupSyncRead = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);

    this->declare_parameter("qos_depth", 10);
    int8_t qos_depth = 0;
    this->get_parameter("qos_depth", qos_depth);

    const auto QOS_RKL10V =
            rclcpp::QoS(rclcpp::KeepLast(qos_depth)).reliable().durability_volatile();

    moving_status_publisher_ = this->create_publisher<MovingStatus>("moving_status", QOS_RKL10V);
    present_pwm_publisher_ = this->create_publisher<PresentPWM>("present_pwm", QOS_RKL10V);
    present_load_publisher_ = this->create_publisher<PresentLoad>("present_load", QOS_RKL10V);
    present_position_publisher_ = this->create_publisher<PresentPosition>("present_position", QOS_RKL10V);
    present_velocity_publisher_ = this->create_publisher<PresentVelocity>("present_velocity", QOS_RKL10V);
    profile_velocity_publisher_ = this->create_publisher<ProfileVelocity>("profile_velocity", QOS_RKL10V);
    position_pid_gain_publisher_ = this->create_publisher<PositionPIDGain>("position_pid_gain", QOS_RKL10V);
    velocity_trajectory_publisher_ = this->create_publisher<VelocityTrajectory>("velocity_trajectory", QOS_RKL10V);
    position_trajectory_publisher_ = this->create_publisher<PositionTrajectory>("position_trajectory", QOS_RKL10V);

    // Initialize the service client
    get_position_client_ = this->create_client<GetPosition>("get_position");

    get_pid_params_service_ = this->create_service<GetPIDParams>(
            "get_pid_params", std::bind(&PositionControlNode::getPIDParams, this, std::placeholders::_1, std::placeholders::_2));
    set_pid_params_service_ = this->create_service<SetPIDParams>(
            "set_pid_params", std::bind(&PositionControlNode::setPIDParams, this, std::placeholders::_1, std::placeholders::_2));

    status_update_timer_ = this->create_wall_timer(
            500ms,  // Adjust the period according to your needs
            std::bind(&PositionControlNode::periodicStatusUpdate, this));


    set_position_subscriber_ =
            this->create_subscription<SetPosition>(
                    "set_position",
                    QOS_RKL10V,
                    [this](const dynamixel_sdk_custom_interfaces::msg::SetPosition::SharedPtr msg) -> void
                    {
                        std::vector<uint8_t> param_goal_position(LEN_GOAL_POSITION);
                        std::vector<uint8_t> param_profile_velocity(LEN_PROFILE_VELOCITY);
                        std::vector<uint8_t> param_goal_pwm(LEN_GOAL_PWM);
                        // Convert and add each position to sync write parameter storage
                        for (size_t i = 0; i < msg->ids.size(); ++i) {
                            uint32_t position = msg->positions[i];
                            uint32_t profile_velocity = msg->profile_velocities[i];
                            uint16_t goal_pwm = msg->goal_pwms[i];
                            param_goal_position[0] = DXL_LOBYTE(DXL_LOWORD(position));
                            param_goal_position[1] = DXL_HIBYTE(DXL_LOWORD(position));
                            param_goal_position[2] = DXL_LOBYTE(DXL_HIWORD(position));
                            param_goal_position[3] = DXL_HIBYTE(DXL_HIWORD(position));

                            param_profile_velocity[0] = DXL_LOBYTE(DXL_LOWORD(profile_velocity));
                            param_profile_velocity[1] = DXL_HIBYTE(DXL_LOWORD(profile_velocity));
                            param_profile_velocity[2] = DXL_LOBYTE(DXL_HIWORD(profile_velocity));
                            param_profile_velocity[3] = DXL_HIBYTE(DXL_HIWORD(profile_velocity));

                            param_goal_pwm[0] = DXL_LOBYTE(DXL_LOWORD(goal_pwm));
                            param_goal_pwm[1] = DXL_HIBYTE(DXL_LOWORD(goal_pwm));

                            groupSyncWriteVelocity->addParam(msg->ids[i], param_profile_velocity.data());
                            groupSyncWritePosition->addParam(msg->ids[i], param_goal_position.data());
                            //groupSyncWritePWM->addParam(msg->ids[i], param_goal_pwm.data());
                        }

                        // Transmit sync write packet for velocity
                        dxl_comm_result = groupSyncWriteVelocity->txPacket();
                        if (dxl_comm_result != COMM_SUCCESS) {
                            RCLCPP_ERROR(this->get_logger(), "SyncWrite Velocity Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
                        }
                        groupSyncWriteVelocity->clearParam();  // Clear sync write parameter storage after transmission

                        // Transmit sync write packet for position
                        dxl_comm_result = groupSyncWritePosition->txPacket();
                        if (dxl_comm_result != COMM_SUCCESS) {
                            RCLCPP_ERROR(this->get_logger(), "SyncWrite Position Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
                        }
                        groupSyncWritePosition->clearParam();

                        dxl_comm_result = groupSyncWritePWM->txPacket();
                        if (dxl_comm_result != COMM_SUCCESS) {
                            RCLCPP_ERROR(this->get_logger(), "SyncWrite PWM Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
                        }
                        groupSyncWritePWM->clearParam();
                    }
            );

    auto get_present_position =
            [this](
                    const std::shared_ptr<GetPosition::Request> request,
                    std::shared_ptr<GetPosition::Response> response) -> void
            {
                // Add parameter storage for present position read
                for (auto id : request->ids) {
                    groupSyncRead->addParam(id);
                }

                // Transmit sync read packet
                dxl_comm_result = groupSyncRead->txRxPacket();
                if (dxl_comm_result != COMM_SUCCESS) {
                    RCLCPP_ERROR(this->get_logger(), "SyncRead Error: %s", packetHandler->getTxRxResult(dxl_comm_result));
                    return;
                }

                // Retrieve data
                for (auto id : request->ids) {
                    if (groupSyncRead->isAvailable(id, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION)) {
                        uint32_t present_position = groupSyncRead->getData(id, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
                        response->positions.push_back(present_position);
                        RCLCPP_INFO(this->get_logger(), "ID: %d Position: %d", id, present_position);
                    } else {
                        RCLCPP_ERROR(this->get_logger(), "Failed to read position for ID %d", id);
                    }
                }
                groupSyncRead->clearParam();  // Clear sync read parameter storage after use
            };

    get_position_server_ = create_service<GetPosition>("get_position", get_present_position);
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
    if (groupSyncRead) {
        delete groupSyncRead;
        groupSyncRead = nullptr;
    }
}

void PositionControlNode::periodicStatusUpdate() {
    RCLCPP_INFO(this->get_logger(), "Periodic status update called");
    // List of IDs are monitored regularly
    std::vector<int32_t> ids_to_monitor = {1, 2};
    std::vector<uint8_t> moving_statuses;
    std::vector<int32_t> pwms;
    std::vector<int32_t> loads;
    std::vector<int32_t> positions;
    std::vector<int32_t> velocities;
    std::vector<int32_t> profile_velocities;
    std::vector<uint16_t> position_p_gains;
    std::vector<uint16_t> position_i_gains;
    std::vector<uint16_t> position_d_gains;
    std::vector<int32_t> velocity_trajectories;
    std::vector<int32_t> position_trajectories;

    for (int32_t id : ids_to_monitor) {
        uint8_t moving_status = 0;
        int32_t present_pwm = 0;
        int32_t present_load = 0;
        int32_t present_position = 0;
        int32_t present_velocity = 0;
        int32_t profile_velocity = 0;
        uint16_t position_p_gain = 0;
        uint16_t position_i_gain = 0;
        uint16_t position_d_gain = 0;
        int32_t velocity_trajectory = 0;
        int32_t position_trajectory = 0;

        packetHandler->read1ByteTxRx(portHandler, id, ADDR_MOVING_STATUS, &moving_status, &dxl_error);
        moving_statuses.push_back(moving_status);

        packetHandler->read2ByteTxRx(portHandler, id, ADDR_PRESENT_PWM, reinterpret_cast<uint16_t*>(&present_pwm), &dxl_error);
        pwms.push_back(present_pwm);

        packetHandler->read2ByteTxRx(portHandler, id, ADDR_PRESENT_LOAD, reinterpret_cast<uint16_t*>(&present_load), &dxl_error);
        loads.push_back(present_load);

        packetHandler->read4ByteTxRx(portHandler, id, ADDR_PRESENT_POSITION, reinterpret_cast<uint32_t*>(&present_position), &dxl_error);
        positions.push_back(present_position);

        packetHandler->read4ByteTxRx(portHandler, id, ADDR_PRESENT_VELOCITY, reinterpret_cast<uint32_t*>(&present_velocity), &dxl_error);
        velocities.push_back(present_velocity);

        packetHandler->read4ByteTxRx(portHandler, id, ADDR_PROFILE_VELOCITY, reinterpret_cast<uint32_t*>(&profile_velocity), &dxl_error);
        profile_velocities.push_back(profile_velocity);

        packetHandler->read2ByteTxRx(portHandler, id, ADDR_POSITION_P_GAIN, &position_p_gain, &dxl_error);
        position_p_gains.push_back(position_p_gain);

        packetHandler->read2ByteTxRx(portHandler, id, ADDR_POSITION_I_GAIN, &position_i_gain, &dxl_error);
        position_i_gains.push_back(position_i_gain);

        packetHandler->read2ByteTxRx(portHandler, id, ADDR_POSITION_D_GAIN, &position_d_gain, &dxl_error);
        position_d_gains.push_back(position_d_gain);

        packetHandler->read4ByteTxRx(portHandler, id, ADDR_VELOCITY_TRAJECTORY, reinterpret_cast<uint32_t*>(&velocity_trajectory), &dxl_error);
        velocity_trajectories.push_back(velocity_trajectory);

        packetHandler->read4ByteTxRx(portHandler, id, ADDR_POSITION_TRAJECTORY, reinterpret_cast<uint32_t*>(&position_trajectory), &dxl_error);
        position_trajectories.push_back(position_trajectory);
    }

    publishMovingStatus(ids_to_monitor, moving_statuses);
    publishPresentPWM(ids_to_monitor, pwms);
    publishPresentLoad(ids_to_monitor, loads);
    publishPresentPosition(ids_to_monitor, positions);
    publishPresentVelocity(ids_to_monitor, velocities);
    publishProfileVelocity(ids_to_monitor, profile_velocities);
    publishPositionPIDGain(ids_to_monitor, position_p_gains, position_i_gains, position_d_gains);
    publishVelocityTrajectory(ids_to_monitor, velocity_trajectories);
    publishPositionTrajectory(ids_to_monitor, position_trajectories);
}

void PositionControlNode::publishMovingStatus(const std::vector<int32_t>& ids, const std::vector<uint8_t>& moving_statuses) {
    auto message = MovingStatus();
    message.ids = ids;
    message.moving_statuses = moving_statuses;
    moving_status_publisher_->publish(message);
    //RCLCPP_INFO(this->get_logger(), "Published Moving Status: %zu IDs", ids.size());
    //for (size_t i = 0; i < ids.size(); ++i) {
        //RCLCPP_INFO(this->get_logger(), "ID: %d, Status: %u", ids[i], moving_statuses[i]);
    //}
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
    RCLCPP_INFO(this->get_logger(), "Published Present PWM: %zu IDs", ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        RCLCPP_INFO(this->get_logger(), "ID: %d, PWM: %d", ids[i], corrected_pwms[i]);
    }
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
    RCLCPP_INFO(this->get_logger(), "Published Present Load: %zu IDs", ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        RCLCPP_INFO(this->get_logger(), "ID: %d, Load: %d", ids[i], corrected_loads[i]);
    }
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
    RCLCPP_INFO(this->get_logger(), "Published Present Velocity: %zu IDs", ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        RCLCPP_INFO(this->get_logger(), "ID: %d, Velocity: %d", ids[i], velocities[i]);
    }
}

void PositionControlNode::publishProfileVelocity(const std::vector<int32_t>& ids, const std::vector<int32_t>& profile_velocities) {
    auto message = ProfileVelocity();
    message.ids = ids;
    message.profile_velocities = profile_velocities;
    profile_velocity_publisher_->publish(message);
    //RCLCPP_INFO(this->get_logger(), "Published Profile Velocity: %zu IDs", ids.size());
    //for (size_t i = 0; i < ids.size(); ++i) {
        //RCLCPP_INFO(this->get_logger(), "ID: %d, Profile Velocity: %d", ids[i], profile_velocities[i]);
    //}
}

void PositionControlNode::publishPositionPIDGain(const std::vector<int32_t>& ids, const std::vector<uint16_t>& position_p_gains, const std::vector<uint16_t>& position_i_gains, const std::vector<uint16_t>& position_d_gains) {
    auto message = PositionPIDGain();
    message.ids = ids;
    message.position_p_gains = position_p_gains;
    message.position_i_gains = position_i_gains;
    message.position_d_gains = position_d_gains;
    position_pid_gain_publisher_->publish(message);
    RCLCPP_INFO(this->get_logger(), "Published Position PID Gain: %zu IDs", ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        RCLCPP_INFO(this->get_logger(), "ID: %d, Position P Gain: %d, Position I Gain: %d, Position D Gain: %d", ids[i], position_p_gains[i], position_i_gains[i] ,position_d_gains[i]);
    }
}

void PositionControlNode::publishVelocityTrajectory(const std::vector<int32_t>& ids, const std::vector<int32_t>& velocity_trajectories) {
    auto message = VelocityTrajectory();
    message.ids = ids;
    message.velocity_trajectories = velocity_trajectories;
    velocity_trajectory_publisher_->publish(message);
    RCLCPP_INFO(this->get_logger(), "Published Velocity Trajectory: %zu IDs", ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        RCLCPP_INFO(this->get_logger(), "ID: %d, Velocity Trajectory: %d", ids[i], velocity_trajectories[i]);
    }
}

void PositionControlNode::publishPositionTrajectory(const std::vector<int32_t>& ids, const std::vector<int32_t>& position_trajectories) {
    auto message = PositionTrajectory();
    message.ids = ids;
    message.position_trajectories = position_trajectories;
    position_trajectory_publisher_->publish(message);
    RCLCPP_INFO(this->get_logger(), "Published Position Trajectory: %zu IDs", ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        RCLCPP_INFO(this->get_logger(), "ID: %d, Position Trajectory: %d", ids[i], position_trajectories[i]);
    }
}

void PositionControlNode::getPIDParams(const std::shared_ptr<GetPIDParams::Request> request,
                                       std::shared_ptr<GetPIDParams::Response> response) {
    for (const auto& id : request->ids) {
        uint16_t p_gain, i_gain, d_gain;
        int dxl_comm_result;

        dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_POSITION_P_GAIN, &p_gain, &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "Failed to read P gain for ID %d", id);
            continue;
        }

        dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_POSITION_I_GAIN, &i_gain, &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "Failed to read I gain for ID %d", id);
            continue;
        }

        dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_POSITION_D_GAIN, &d_gain, &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "Failed to read D gain for ID %d", id);
            continue;
        }

        response->p_gains.push_back(p_gain);
        response->i_gains.push_back(i_gain);
        response->d_gains.push_back(d_gain);
    }
}

void PositionControlNode::setPIDParams(const std::shared_ptr<SetPIDParams::Request> request,
                                       std::shared_ptr<SetPIDParams::Response> response) {
    for (size_t i = 0; i < request->ids.size(); ++i) {
        int dxl_comm_result;

        dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, request->ids[i], ADDR_POSITION_P_GAIN, request->p_gains[i],
                                                        &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "Failed to write P gain for ID %d", request->ids[i]);
            response->success = false;
            return;
        }

        dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, request->ids[i], ADDR_POSITION_I_GAIN, request->i_gains[i],
                                                        &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "Failed to write I gain for ID %d", request->ids[i]);
            response->success = false;
            return;
        }

        dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, request->ids[i], ADDR_POSITION_D_GAIN, request->d_gains[i],
                                                        &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "Failed to write D gain for ID %d", request->ids[i]);
            response->success = false;
            return;
        }
    }

    response->success = true;
}

void PositionControlNode::send_request() {
    auto request = std::make_shared<GetPosition::Request>();
    request->ids = {1, 2};

    // Asynchronous call to the service
    auto result_future = get_position_client_->async_send_request(request);

    // This will block and wait until the future is completed or until a timeout
    if (rclcpp::spin_until_future_complete(this->shared_from_this(), result_future, std::chrono::seconds(5)) == rclcpp::FutureReturnCode::SUCCESS) {
        auto response = result_future.get();
        if (response) {
            this->handle_response(response);
        } else {
            RCLCPP_ERROR(this->get_logger(), "Failed to get a response");
        }
    } else {
        RCLCPP_ERROR(this->get_logger(), "Service call failed or timed out");
    }
}

void PositionControlNode::handle_response(const std::shared_ptr<dynamixel_sdk_custom_interfaces::srv::GetPosition_Response>& response) {
    RCLCPP_INFO(this->get_logger(), "Received position for ID 1: %u, ID 2: %u", response->positions[0], response->positions[1]);
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

    auto position_control_node = std::make_shared<PositionControlNode>();
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