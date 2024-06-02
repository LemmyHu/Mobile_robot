// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from joystick_listener:msg/JoystickCommand.idl
// generated code does not contain a copyright notice

#ifndef JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__TRAITS_HPP_
#define JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "joystick_listener/msg/detail/joystick_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace joystick_listener
{

namespace msg
{

inline void to_flow_style_yaml(
  const JoystickCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: mode
  {
    out << "mode: ";
    rosidl_generator_traits::value_to_yaml(msg.mode, out);
    out << ", ";
  }

  // member: values
  {
    if (msg.values.size() == 0) {
      out << "values: []";
    } else {
      out << "values: [";
      size_t pending_items = msg.values.size();
      for (auto item : msg.values) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const JoystickCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "mode: ";
    rosidl_generator_traits::value_to_yaml(msg.mode, out);
    out << "\n";
  }

  // member: values
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.values.size() == 0) {
      out << "values: []\n";
    } else {
      out << "values:\n";
      for (auto item : msg.values) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const JoystickCommand & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace joystick_listener

namespace rosidl_generator_traits
{

[[deprecated("use joystick_listener::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const joystick_listener::msg::JoystickCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  joystick_listener::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use joystick_listener::msg::to_yaml() instead")]]
inline std::string to_yaml(const joystick_listener::msg::JoystickCommand & msg)
{
  return joystick_listener::msg::to_yaml(msg);
}

template<>
inline const char * data_type<joystick_listener::msg::JoystickCommand>()
{
  return "joystick_listener::msg::JoystickCommand";
}

template<>
inline const char * name<joystick_listener::msg::JoystickCommand>()
{
  return "joystick_listener/msg/JoystickCommand";
}

template<>
struct has_fixed_size<joystick_listener::msg::JoystickCommand>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<joystick_listener::msg::JoystickCommand>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<joystick_listener::msg::JoystickCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__TRAITS_HPP_
