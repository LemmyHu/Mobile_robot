// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from joystick_listener:msg/JoystickCommand.idl
// generated code does not contain a copyright notice

#ifndef JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__STRUCT_HPP_
#define JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__joystick_listener__msg__JoystickCommand __attribute__((deprecated))
#else
# define DEPRECATED__joystick_listener__msg__JoystickCommand __declspec(deprecated)
#endif

namespace joystick_listener
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct JoystickCommand_
{
  using Type = JoystickCommand_<ContainerAllocator>;

  explicit JoystickCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->mode = 0l;
    }
  }

  explicit JoystickCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->mode = 0l;
    }
  }

  // field types and members
  using _mode_type =
    int32_t;
  _mode_type mode;
  using _values_type =
    std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>>;
  _values_type values;

  // setters for named parameter idiom
  Type & set__mode(
    const int32_t & _arg)
  {
    this->mode = _arg;
    return *this;
  }
  Type & set__values(
    const std::vector<int32_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<int32_t>> & _arg)
  {
    this->values = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    joystick_listener::msg::JoystickCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const joystick_listener::msg::JoystickCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<joystick_listener::msg::JoystickCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<joystick_listener::msg::JoystickCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      joystick_listener::msg::JoystickCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<joystick_listener::msg::JoystickCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      joystick_listener::msg::JoystickCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<joystick_listener::msg::JoystickCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<joystick_listener::msg::JoystickCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<joystick_listener::msg::JoystickCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__joystick_listener__msg__JoystickCommand
    std::shared_ptr<joystick_listener::msg::JoystickCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__joystick_listener__msg__JoystickCommand
    std::shared_ptr<joystick_listener::msg::JoystickCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const JoystickCommand_ & other) const
  {
    if (this->mode != other.mode) {
      return false;
    }
    if (this->values != other.values) {
      return false;
    }
    return true;
  }
  bool operator!=(const JoystickCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct JoystickCommand_

// alias to use template instance with default allocator
using JoystickCommand =
  joystick_listener::msg::JoystickCommand_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace joystick_listener

#endif  // JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__STRUCT_HPP_
