// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from dynamixel_sdk_custom_interfaces:msg/SetPosition.idl
// generated code does not contain a copyright notice

#ifndef DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__SET_POSITION__BUILDER_HPP_
#define DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__SET_POSITION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "dynamixel_sdk_custom_interfaces/msg/detail/set_position__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace dynamixel_sdk_custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_SetPosition_goal_pwms
{
public:
  explicit Init_SetPosition_goal_pwms(::dynamixel_sdk_custom_interfaces::msg::SetPosition & msg)
  : msg_(msg)
  {}
  ::dynamixel_sdk_custom_interfaces::msg::SetPosition goal_pwms(::dynamixel_sdk_custom_interfaces::msg::SetPosition::_goal_pwms_type arg)
  {
    msg_.goal_pwms = std::move(arg);
    return std::move(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::SetPosition msg_;
};

class Init_SetPosition_profile_velocities
{
public:
  explicit Init_SetPosition_profile_velocities(::dynamixel_sdk_custom_interfaces::msg::SetPosition & msg)
  : msg_(msg)
  {}
  Init_SetPosition_goal_pwms profile_velocities(::dynamixel_sdk_custom_interfaces::msg::SetPosition::_profile_velocities_type arg)
  {
    msg_.profile_velocities = std::move(arg);
    return Init_SetPosition_goal_pwms(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::SetPosition msg_;
};

class Init_SetPosition_positions
{
public:
  explicit Init_SetPosition_positions(::dynamixel_sdk_custom_interfaces::msg::SetPosition & msg)
  : msg_(msg)
  {}
  Init_SetPosition_profile_velocities positions(::dynamixel_sdk_custom_interfaces::msg::SetPosition::_positions_type arg)
  {
    msg_.positions = std::move(arg);
    return Init_SetPosition_profile_velocities(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::SetPosition msg_;
};

class Init_SetPosition_ids
{
public:
  Init_SetPosition_ids()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetPosition_positions ids(::dynamixel_sdk_custom_interfaces::msg::SetPosition::_ids_type arg)
  {
    msg_.ids = std::move(arg);
    return Init_SetPosition_positions(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::SetPosition msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::dynamixel_sdk_custom_interfaces::msg::SetPosition>()
{
  return dynamixel_sdk_custom_interfaces::msg::builder::Init_SetPosition_ids();
}

}  // namespace dynamixel_sdk_custom_interfaces

#endif  // DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__SET_POSITION__BUILDER_HPP_
