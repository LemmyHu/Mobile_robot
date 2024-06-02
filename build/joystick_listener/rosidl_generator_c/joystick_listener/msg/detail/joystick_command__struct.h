// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from joystick_listener:msg/JoystickCommand.idl
// generated code does not contain a copyright notice

#ifndef JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__STRUCT_H_
#define JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'values'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/JoystickCommand in the package joystick_listener.
typedef struct joystick_listener__msg__JoystickCommand
{
  /// 0 for position, 1 for velocity
  int32_t mode;
  /// Depending on mode, these could be position or velocity values
  rosidl_runtime_c__int32__Sequence values;
} joystick_listener__msg__JoystickCommand;

// Struct for a sequence of joystick_listener__msg__JoystickCommand.
typedef struct joystick_listener__msg__JoystickCommand__Sequence
{
  joystick_listener__msg__JoystickCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} joystick_listener__msg__JoystickCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // JOYSTICK_LISTENER__MSG__DETAIL__JOYSTICK_COMMAND__STRUCT_H_
