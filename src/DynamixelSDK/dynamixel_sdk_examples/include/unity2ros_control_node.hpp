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
#include <vector>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"
#include "dynamixel_sdk/dynamixel_sdk.h"
#include "dynamixel_sdk_custom_interfaces/msg/present_pwm.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_load.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_velocity.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/profile_velocity.hpp"


class PositionControlNode : public rclcpp::Node
{
public:
    using PresentPWM = dynamixel_sdk_custom_interfaces::msg::PresentPWM;
    using PresentLoad = dynamixel_sdk_custom_interfaces::msg::PresentLoad;
    using PresentPosition = dynamixel_sdk_custom_interfaces::msg::PresentPosition;
    using PresentVelocity = dynamixel_sdk_custom_interfaces::msg::PresentVelocity;
    using ProfileVelocity = dynamixel_sdk_custom_interfaces::msg::ProfileVelocity;

    PositionControlNode();
    virtual ~PositionControlNode();

    void periodicStatusUpdate();
    void publishPresentPWM(const std::vector<int32_t>& ids, const std::vector<int32_t>& pwms);
    void publishPresentLoad(const std::vector<int32_t>& ids, const std::vector<int32_t>& loads);
    void publishPresentPosition(const std::vector<int32_t>& ids, const std::vector<int32_t>& positions);
    void publishPresentVelocity(const std::vector<int32_t>& ids, const std::vector<int32_t>& velocities);


private:
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscriber_;
    rclcpp::TimerBase::SharedPtr status_update_timer_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentPWM>::SharedPtr present_pwm_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentLoad>::SharedPtr present_load_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentPosition>::SharedPtr present_position_publisher_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>::SharedPtr present_velocity_publisher_;

    dynamixel::GroupSyncWrite *groupSyncWriteGoalPosition; // Synchronized goal position writer
    dynamixel::GroupSyncWrite *groupSyncWriteProfileVelocity; // Synchronized profile velocity writer
    dynamixel::GroupSyncWrite *groupSyncWriteGOALPWM; // Synchronized PWM
    dynamixel::GroupSyncRead *groupSyncReadPosition;       // Synchronized position reader


    int present_position;
};

#endif  // POSITION_CONTROL_NODE_HPP_