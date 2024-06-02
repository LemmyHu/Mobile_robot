// Copyright 2021 ROBOTIS CO., LTD.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef POSITION_CONTROL_NODE_HPP_
#define POSITION_CONTROL_NODE_HPP_

#include <cstdio>
#include <memory>
#include <string>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"
#include "dynamixel_sdk/dynamixel_sdk.h"
#include "dynamixel_sdk_custom_interfaces/msg/set_position.hpp"
#include "dynamixel_sdk_custom_interfaces/srv/get_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/moving_status.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_pwm.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_load.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_velocity.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/profile_velocity.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/position_pid_gain.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/velocity_trajectory.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/position_trajectory.hpp"
#include "dynamixel_sdk_custom_interfaces/srv/get_pid_params.hpp"
#include "dynamixel_sdk_custom_interfaces/srv/set_pid_params.hpp"


class PositionControlNode : public rclcpp::Node
{
public:
    using SetPosition = dynamixel_sdk_custom_interfaces::msg::SetPosition;
    using MovingStatus = dynamixel_sdk_custom_interfaces::msg::MovingStatus;
    using PresentPWM = dynamixel_sdk_custom_interfaces::msg::PresentPWM;
    using PresentLoad = dynamixel_sdk_custom_interfaces::msg::PresentLoad;
    using PresentPosition = dynamixel_sdk_custom_interfaces::msg::PresentPosition;
    using PresentVelocity = dynamixel_sdk_custom_interfaces::msg::PresentVelocity;
    using ProfileVelocity = dynamixel_sdk_custom_interfaces::msg::ProfileVelocity;
    using PositionPIDGain = dynamixel_sdk_custom_interfaces::msg::PositionPIDGain;
    using VelocityTrajectory = dynamixel_sdk_custom_interfaces::msg::VelocityTrajectory;
    using PositionTrajectory = dynamixel_sdk_custom_interfaces::msg::PositionTrajectory;
    using GetPosition = dynamixel_sdk_custom_interfaces::srv::GetPosition;
    using GetPIDParams = dynamixel_sdk_custom_interfaces::srv::GetPIDParams;
    using SetPIDParams = dynamixel_sdk_custom_interfaces::srv::SetPIDParams;

    PositionControlNode();
    virtual ~PositionControlNode();

    //void update_motor_status(uint8_t id, uint32_t position);  // Declaration of update_motor_status
    void periodicStatusUpdate();
    void publishMovingStatus(const std::vector<int32_t>& ids, const std::vector<uint8_t>& moving_statuses);
    void publishPresentPWM(const std::vector<int32_t>& ids, const std::vector<int32_t>& pwms);
    void publishPresentLoad(const std::vector<int32_t>& ids, const std::vector<int32_t>& loads);
    void publishPresentPosition(const std::vector<int32_t>& ids, const std::vector<int32_t>& positions);
    void publishPresentVelocity(const std::vector<int32_t>& ids, const std::vector<int32_t>& velocities);
    void publishProfileVelocity(const std::vector<int32_t>& ids, const std::vector<int32_t>& profile_velocities);
    void publishPositionPIDGain(const std::vector<int32_t>& ids, const std::vector<uint16_t>& position_p_gains, const std::vector<uint16_t>& position_i_gains, const std::vector<uint16_t>& position_d_gains);
    void publishVelocityTrajectory(const std::vector<int32_t>& ids, const std::vector<int32_t>& velocity_trajectories);
    void publishPositionTrajectory(const std::vector<int32_t>& ids, const std::vector<int32_t>& position_trajectories);
    void send_request();
    void getPIDParams(const std::shared_ptr<GetPIDParams::Request> request, std::shared_ptr<GetPIDParams::Response> response);
    void setPIDParams(const std::shared_ptr<SetPIDParams::Request> request, std::shared_ptr<SetPIDParams::Response> response);


private:
    rclcpp::Subscription<SetPosition>::SharedPtr set_position_subscriber_;
    rclcpp::Service<GetPosition>::SharedPtr get_position_server_;
    rclcpp::Service<GetPIDParams>::SharedPtr get_pid_params_service_;
    rclcpp::Service<SetPIDParams>::SharedPtr set_pid_params_service_;
    rclcpp::Client<GetPosition>::SharedPtr get_position_client_;
    rclcpp::TimerBase::SharedPtr status_update_timer_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::MovingStatus>::SharedPtr moving_status_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentPWM>::SharedPtr present_pwm_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentLoad>::SharedPtr present_load_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentPosition>::SharedPtr present_position_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>::SharedPtr present_velocity_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::ProfileVelocity>::SharedPtr profile_velocity_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PositionPIDGain>::SharedPtr position_pid_gain_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::VelocityTrajectory>::SharedPtr velocity_trajectory_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PositionTrajectory>::SharedPtr position_trajectory_publisher_;

    dynamixel::GroupSyncWrite *groupSyncWriteGoalPosition; // Synchronized goal position writer
    dynamixel::GroupSyncWrite *groupSyncWriteProfileVelocity; // Synchronized profile velocity writer
    dynamixel::GroupSyncWrite *groupSyncWriteGOALPWM; // Synchronized PWM
    dynamixel::GroupSyncRead *groupSyncReadPosition;       // Synchronized position reader

    void handle_response(const std::shared_ptr<dynamixel_sdk_custom_interfaces::srv::GetPosition_Response>& response);

    int present_position;
};

#endif  // POSITION_CONTROL_NODE_HPP_