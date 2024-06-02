// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from dynamixel_sdk_custom_interfaces:msg/PresentLoad.idl
// generated code does not contain a copyright notice

#ifndef DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__PRESENT_LOAD__BUILDER_HPP_
#define DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__PRESENT_LOAD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "dynamixel_sdk_custom_interfaces/msg/detail/present_load__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace dynamixel_sdk_custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_PresentLoad_loads
{
public:
  explicit Init_PresentLoad_loads(::dynamixel_sdk_custom_interfaces::msg::PresentLoad & msg)
  : msg_(msg)
  {}
  ::dynamixel_sdk_custom_interfaces::msg::PresentLoad loads(::dynamixel_sdk_custom_interfaces::msg::PresentLoad::_loads_type arg)
  {
    msg_.loads = std::move(arg);
    return std::move(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::PresentLoad msg_;
};

class Init_PresentLoad_ids
{
public:
  Init_PresentLoad_ids()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PresentLoad_loads ids(::dynamixel_sdk_custom_interfaces::msg::PresentLoad::_ids_type arg)
  {
    msg_.ids = std::move(arg);
    return Init_PresentLoad_loads(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::PresentLoad msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::dynamixel_sdk_custom_interfaces::msg::PresentLoad>()
{
  return dynamixel_sdk_custom_interfaces::msg::builder::Init_PresentLoad_ids();
}

}  // namespace dynamixel_sdk_custom_interfaces

#endif  // DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__PRESENT_LOAD__BUILDER_HPP_
