// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from dynamixel_sdk_custom_interfaces:msg/MovingStatus.idl
// generated code does not contain a copyright notice

#ifndef DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__MOVING_STATUS__STRUCT_H_
#define DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__MOVING_STATUS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'ids'
// Member 'moving_statuses'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/MovingStatus in the package dynamixel_sdk_custom_interfaces.
/**
  * Messages
 */
typedef struct dynamixel_sdk_custom_interfaces__msg__MovingStatus
{
  rosidl_runtime_c__int32__Sequence ids;
  rosidl_runtime_c__uint8__Sequence moving_statuses;
} dynamixel_sdk_custom_interfaces__msg__MovingStatus;

// Struct for a sequence of dynamixel_sdk_custom_interfaces__msg__MovingStatus.
typedef struct dynamixel_sdk_custom_interfaces__msg__MovingStatus__Sequence
{
  dynamixel_sdk_custom_interfaces__msg__MovingStatus * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} dynamixel_sdk_custom_interfaces__msg__MovingStatus__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // DYNAMIXEL_SDK_CUSTOM_INTERFACES__MSG__DETAIL__MOVING_STATUS__STRUCT_H_
