// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from dynamixel_sdk_custom_interfaces:srv/GetVelocity.idl
// generated code does not contain a copyright notice

#ifndef DYNAMIXEL_SDK_CUSTOM_INTERFACES__SRV__DETAIL__GET_VELOCITY__TRAITS_HPP_
#define DYNAMIXEL_SDK_CUSTOM_INTERFACES__SRV__DETAIL__GET_VELOCITY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "dynamixel_sdk_custom_interfaces/srv/detail/get_velocity__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace dynamixel_sdk_custom_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetVelocity_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: ids
  {
    if (msg.ids.size() == 0) {
      out << "ids: []";
    } else {
      out << "ids: [";
      size_t pending_items = msg.ids.size();
      for (auto item : msg.ids) {
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
  const GetVelocity_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: ids
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.ids.size() == 0) {
      out << "ids: []\n";
    } else {
      out << "ids:\n";
      for (auto item : msg.ids) {
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

inline std::string to_yaml(const GetVelocity_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace dynamixel_sdk_custom_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use dynamixel_sdk_custom_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  dynamixel_sdk_custom_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use dynamixel_sdk_custom_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request & msg)
{
  return dynamixel_sdk_custom_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request>()
{
  return "dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request";
}

template<>
inline const char * name<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request>()
{
  return "dynamixel_sdk_custom_interfaces/srv/GetVelocity_Request";
}

template<>
struct has_fixed_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace dynamixel_sdk_custom_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GetVelocity_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: velocities
  {
    if (msg.velocities.size() == 0) {
      out << "velocities: []";
    } else {
      out << "velocities: [";
      size_t pending_items = msg.velocities.size();
      for (auto item : msg.velocities) {
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
  const GetVelocity_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: velocities
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.velocities.size() == 0) {
      out << "velocities: []\n";
    } else {
      out << "velocities:\n";
      for (auto item : msg.velocities) {
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

inline std::string to_yaml(const GetVelocity_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace dynamixel_sdk_custom_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use dynamixel_sdk_custom_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  dynamixel_sdk_custom_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use dynamixel_sdk_custom_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response & msg)
{
  return dynamixel_sdk_custom_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response>()
{
  return "dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response";
}

template<>
inline const char * name<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response>()
{
  return "dynamixel_sdk_custom_interfaces/srv/GetVelocity_Response";
}

template<>
struct has_fixed_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<dynamixel_sdk_custom_interfaces::srv::GetVelocity>()
{
  return "dynamixel_sdk_custom_interfaces::srv::GetVelocity";
}

template<>
inline const char * name<dynamixel_sdk_custom_interfaces::srv::GetVelocity>()
{
  return "dynamixel_sdk_custom_interfaces/srv/GetVelocity";
}

template<>
struct has_fixed_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity>
  : std::integral_constant<
    bool,
    has_fixed_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request>::value &&
    has_fixed_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response>::value
  >
{
};

template<>
struct has_bounded_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity>
  : std::integral_constant<
    bool,
    has_bounded_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request>::value &&
    has_bounded_size<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response>::value
  >
{
};

template<>
struct is_service<dynamixel_sdk_custom_interfaces::srv::GetVelocity>
  : std::true_type
{
};

template<>
struct is_service_request<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Request>
  : std::true_type
{
};

template<>
struct is_service_response<dynamixel_sdk_custom_interfaces::srv::GetVelocity_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // DYNAMIXEL_SDK_CUSTOM_INTERFACES__SRV__DETAIL__GET_VELOCITY__TRAITS_HPP_
