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

#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"
#include "dynamixel_sdk/dynamixel_sdk.h"
#include "dynamixel_sdk_custom_interfaces/msg/set_position.hpp"
#include "dynamixel_sdk_custom_interfaces/srv/get_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/moving_status.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_load.hpp"



class PositionControlNode : public rclcpp::Node
{
public:
  using SetPosition = dynamixel_sdk_custom_interfaces::msg::SetPosition;
  using MovingStatus = dynamixel_sdk_custom_interfaces::msg::MovingStatus;
  using PresentLoad = dynamixel_sdk_custom_interfaces::msg::PresentLoad;
  using GetPosition = dynamixel_sdk_custom_interfaces::srv::GetPosition;

  PositionControlNode();
  virtual ~PositionControlNode();

  //void update_motor_status(uint8_t id, uint32_t position);  // Declaration of update_motor_status
  void handle_position(const dynamixel_sdk_custom_interfaces::msg::SetPosition::SharedPtr msg);
  void publishMovingStatus(const std::vector<int32_t>& ids, const std::vector<uint8_t>& moving_statuses);
  void publishPresentLoad(const std::vector<int32_t>& ids, const std::vector<int32_t>& loads);


private:
  rclcpp::Subscription<SetPosition>::SharedPtr set_position_subscriber_;
  rclcpp::Service<GetPosition>::SharedPtr get_position_server_;
  rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::MovingStatus>::SharedPtr moving_status_publisher_;
  rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::PresentLoad>::SharedPtr present_load_publisher_;

  int present_position;
};

#endif  // POSITION_CONTROL_NODE_HPP_
