// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from dynamixel_sdk_custom_interfaces:msg/MovingStatus.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "dynamixel_sdk_custom_interfaces/msg/detail/moving_status__rosidl_typesupport_introspection_c.h"
#include "dynamixel_sdk_custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "dynamixel_sdk_custom_interfaces/msg/detail/moving_status__functions.h"
#include "dynamixel_sdk_custom_interfaces/msg/detail/moving_status__struct.h"


// Include directives for member types
// Member `ids`
// Member `moving_statuses`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  dynamixel_sdk_custom_interfaces__msg__MovingStatus__init(message_memory);
}

void dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_fini_function(void * message_memory)
{
  dynamixel_sdk_custom_interfaces__msg__MovingStatus__fini(message_memory);
}

size_t dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__size_function__MovingStatus__ids(
  const void * untyped_member)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return member->size;
}

const void * dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_const_function__MovingStatus__ids(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void * dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_function__MovingStatus__ids(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__fetch_function__MovingStatus__ids(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int32_t * item =
    ((const int32_t *)
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_const_function__MovingStatus__ids(untyped_member, index));
  int32_t * value =
    (int32_t *)(untyped_value);
  *value = *item;
}

void dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__assign_function__MovingStatus__ids(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int32_t * item =
    ((int32_t *)
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_function__MovingStatus__ids(untyped_member, index));
  const int32_t * value =
    (const int32_t *)(untyped_value);
  *item = *value;
}

bool dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__resize_function__MovingStatus__ids(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  rosidl_runtime_c__int32__Sequence__fini(member);
  return rosidl_runtime_c__int32__Sequence__init(member, size);
}

size_t dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__size_function__MovingStatus__moving_statuses(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_const_function__MovingStatus__moving_statuses(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_function__MovingStatus__moving_statuses(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__fetch_function__MovingStatus__moving_statuses(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_const_function__MovingStatus__moving_statuses(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__assign_function__MovingStatus__moving_statuses(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_function__MovingStatus__moving_statuses(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__resize_function__MovingStatus__moving_statuses(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_message_member_array[2] = {
  {
    "ids",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dynamixel_sdk_custom_interfaces__msg__MovingStatus, ids),  // bytes offset in struct
    NULL,  // default value
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__size_function__MovingStatus__ids,  // size() function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_const_function__MovingStatus__ids,  // get_const(index) function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_function__MovingStatus__ids,  // get(index) function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__fetch_function__MovingStatus__ids,  // fetch(index, &value) function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__assign_function__MovingStatus__ids,  // assign(index, value) function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__resize_function__MovingStatus__ids  // resize(index) function pointer
  },
  {
    "moving_statuses",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dynamixel_sdk_custom_interfaces__msg__MovingStatus, moving_statuses),  // bytes offset in struct
    NULL,  // default value
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__size_function__MovingStatus__moving_statuses,  // size() function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_const_function__MovingStatus__moving_statuses,  // get_const(index) function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__get_function__MovingStatus__moving_statuses,  // get(index) function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__fetch_function__MovingStatus__moving_statuses,  // fetch(index, &value) function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__assign_function__MovingStatus__moving_statuses,  // assign(index, value) function pointer
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__resize_function__MovingStatus__moving_statuses  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_message_members = {
  "dynamixel_sdk_custom_interfaces__msg",  // message namespace
  "MovingStatus",  // message name
  2,  // number of fields
  sizeof(dynamixel_sdk_custom_interfaces__msg__MovingStatus),
  dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_message_member_array,  // message members
  dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_init_function,  // function to initialize message memory (memory has to be allocated)
  dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_message_type_support_handle = {
  0,
  &dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_dynamixel_sdk_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dynamixel_sdk_custom_interfaces, msg, MovingStatus)() {
  if (!dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_message_type_support_handle.typesupport_identifier) {
    dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &dynamixel_sdk_custom_interfaces__msg__MovingStatus__rosidl_typesupport_introspection_c__MovingStatus_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
