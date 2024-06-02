// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from dynamixel_sdk_custom_interfaces:srv/GetVelocity.idl
// generated code does not contain a copyright notice

#ifndef DYNAMIXEL_SDK_CUSTOM_INTERFACES__SRV__DETAIL__GET_VELOCITY__STRUCT_H_
#define DYNAMIXEL_SDK_CUSTOM_INTERFACES__SRV__DETAIL__GET_VELOCITY__STRUCT_H_

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
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/GetVelocity in the package dynamixel_sdk_custom_interfaces.
typedef struct dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request
{
  rosidl_runtime_c__uint8__Sequence ids;
} dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request;

// Struct for a sequence of dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request.
typedef struct dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__Sequence
{
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'velocities'
// already included above
// #include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in srv/GetVelocity in the package dynamixel_sdk_custom_interfaces.
typedef struct dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response
{
  /// Change from position to velocity
  rosidl_runtime_c__int32__Sequence velocities;
} dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response;

// Struct for a sequence of dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response.
typedef struct dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__Sequence
{
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // DYNAMIXEL_SDK_CUSTOM_INTERFACES__SRV__DETAIL__GET_VELOCITY__STRUCT_H_
