// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from dynamixel_sdk_custom_interfaces:msg/Status.idl
// generated code does not contain a copyright notice

#ifndef DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__STATUS__BUILDER_HPP_
#define DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "dynamixel_sdk_custom_interfaces/msg/detail/status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace dynamixel_sdk_custom_interfaces
{

namespace msg
{

namespace builder
{

class Init_Status_values
{
public:
  explicit Init_Status_values(::dynamixel_sdk_custom_interfaces::msg::Status & msg)
  : msg_(msg)
  {}
  ::dynamixel_sdk_custom_interfaces::msg::Status values(::dynamixel_sdk_custom_interfaces::msg::Status::_values_type arg)
  {
    msg_.values = std::move(arg);
    return std::move(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::Status msg_;
};

class Init_Status_ids
{
public:
  Init_Status_ids()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Status_values ids(::dynamixel_sdk_custom_interfaces::msg::Status::_ids_type arg)
  {
    msg_.ids = std::move(arg);
    return Init_Status_values(msg_);
  }

private:
  ::dynamixel_sdk_custom_interfaces::msg::Status msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::dynamixel_sdk_custom_interfaces::msg::Status>()
{
  return dynamixel_sdk_custom_interfaces::msg::builder::Init_Status_ids();
}

}  // namespace dynamixel_sdk_custom_interfaces

#endif  // DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__STATUS__BUILDER_HPP_
