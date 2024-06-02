// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from dynamixel_sdk_custom_interfaces:msg/MovingStatus.idl
// generated code does not contain a copyright notice

#ifndef DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__MOVING_STATUS__BUILDER_HPP_
#define DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__MOVING_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "dynamixel_sdk_custom_interfaces/msg/detail/moving_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace dynamixel_sdk_custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_MovingStatus_moving_statuses
{
public:
  explicit Init_MovingStatus_moving_statuses(::dynamixel_sdk_custom_interfaces::msg::MovingStatus & msg)
  : msg_(msg)
  {}
  ::dynamixel_sdk_custom_interfaces::msg::MovingStatus moving_statuses(::dynamixel_sdk_custom_interfaces::msg::MovingStatus::_moving_statuses_type arg)
  {
    msg_.moving_statuses = std::move(arg);
    return std::move(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::MovingStatus msg_;
};

class Init_MovingStatus_ids
{
public:
  Init_MovingStatus_ids()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MovingStatus_moving_statuses ids(::dynamixel_sdk_custom_interfaces::msg::MovingStatus::_ids_type arg)
  {
    msg_.ids = std::move(arg);
    return Init_MovingStatus_moving_statuses(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::MovingStatus msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::dynamixel_sdk_custom_interfaces::msg::MovingStatus>()
{
  return dynamixel_sdk_custom_interfaces::msg::builder::Init_MovingStatus_ids();
}

}  // namespace dynamixel_sdk_custom_interfaces

#endif  // DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__MOVING_STATUS__BUILDER_HPP_
