// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from joystick_listener:msg/JoystickCommand.idl
// generated code does not contain a copyright notice

#ifndef JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "joystick_listener/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "joystick_listener/msg/detail/joystick_command__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace joystick_listener
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_joystick_listener
cdr_serialize(
  const joystick_listener::msg::JoystickCommand & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_joystick_listener
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  joystick_listener::msg::JoystickCommand & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_joystick_listener
get_serialized_size(
  const joystick_listener::msg::JoystickCommand & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_joystick_listener
max_serialized_size_JoystickCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace joystick_listener

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_joystick_listener
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, joystick_listener, msg, JoystickCommand)();

#ifdef __cplusplus
}
#endif

#endif  // JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
