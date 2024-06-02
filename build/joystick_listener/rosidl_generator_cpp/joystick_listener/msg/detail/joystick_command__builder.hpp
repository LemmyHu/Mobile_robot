// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from joystick_listener:msg/JoystickCommand.idl
// generated code does not contain a copyright notice

#ifndef JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__BUILDER_HPP_
#define JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "joystick_listener/msg/detail/joystick_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace joystick_listener
{

namespace msg
{

namespace builder
{

class Init_JoystickCommand_values
{
public:
  explicit Init_JoystickCommand_values(::joystick_listener::msg::JoystickCommand & msg)
  : msg_(msg)
  {}
  ::joystick_listener::msg::JoystickCommand values(::joystick_listener::msg::JoystickCommand::_values_type arg)
  {
    msg_.values = std::move(arg);
    return std::move(msg_);
  }

private:
  ::joystick_listener::msg::JoystickCommand msg_;
};

class Init_JoystickCommand_mode
{
public:
  Init_JoystickCommand_mode()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_JoystickCommand_values mode(::joystick_listener::msg::JoystickCommand::_mode_type arg)
  {
    msg_.mode = std::move(arg);
    return Init_JoystickCommand_values(msg_);
  }

private:
  ::joystick_listener::msg::JoystickCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::joystick_listener::msg::JoystickCommand>()
{
  return joystick_listener::msg::builder::Init_JoystickCommand_mode();
}

}  // namespace joystick_listener

#endif  // JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__BUILDER_HPP_
