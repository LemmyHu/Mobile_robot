// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from dynamixel_sdk_custom_interfaces:srv/GetVelocity.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "dynamixel_sdk_custom_interfaces/srv/detail/get_velocity__rosidl_typesupport_introspection_c.h"
#include "dynamixel_sdk_custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "dynamixel_sdk_custom_interfaces/srv/detail/get_velocity__functions.h"
#include "dynamixel_sdk_custom_interfaces/srv/detail/get_velocity__struct.h"


// Include directives for member types
// Member `ids`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__init(message_memory);
}

void dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_fini_function(void * message_memory)
{
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__fini(message_memory);
}

size_t dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__size_function__GetVelocity_Request__ids(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__get_const_function__GetVelocity_Request__ids(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__get_function__GetVelocity_Request__ids(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__fetch_function__GetVelocity_Request__ids(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__get_const_function__GetVelocity_Request__ids(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__assign_function__GetVelocity_Request__ids(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__get_function__GetVelocity_Request__ids(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__resize_function__GetVelocity_Request__ids(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_message_member_array[1] = {
  {
    "ids",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request, ids),  // bytes offset in struct
    NULL,  // default value
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__size_function__GetVelocity_Request__ids,  // size() function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__get_const_function__GetVelocity_Request__ids,  // get_const(index) function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__get_function__GetVelocity_Request__ids,  // get(index) function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__fetch_function__GetVelocity_Request__ids,  // fetch(index, &value) function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__assign_function__GetVelocity_Request__ids,  // assign(index, value) function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__resize_function__GetVelocity_Request__ids  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_message_members = {
  "dynamixel_sdk_custom_interfaces__srv",  // message namespace
  "GetVelocity_Request",  // message name
  1,  // number of fields
  sizeof(dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request),
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_message_member_array,  // message members
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_message_type_support_handle = {
  0,
  &dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_dynamixel_sdk_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dynamixel_sdk_custom_interfaces, srv, GetVelocity_Request)() {
  if (!dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_message_type_support_handle.typesupport_identifier) {
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &dynamixel_sdk_custom_interfaces__srv__GetVelocity_Request__rosidl_typesupport_introspection_c__GetVelocity_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "dynamixel_sdk_custom_interfaces/srv/detail/get_velocity__rosidl_typesupport_introspection_c.h"
// already included above
// #include "dynamixel_sdk_custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "dynamixel_sdk_custom_interfaces/srv/detail/get_velocity__functions.h"
// already included above
// #include "dynamixel_sdk_custom_interfaces/srv/detail/get_velocity__struct.h"


// Include directives for member types
// Member `velocities`
// already included above
// #include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__init(message_memory);
}

void dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_fini_function(void * message_memory)
{
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__fini(message_memory);
}

size_t dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__size_function__GetVelocity_Response__velocities(
  const void * untyped_member)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return member->size;
}

const void * dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__get_const_function__GetVelocity_Response__velocities(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__int32__Sequence * member =
    (const rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void * dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__get_function__GetVelocity_Response__velocities(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  return &member->data[index];
}

void dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__fetch_function__GetVelocity_Response__velocities(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const int32_t * item =
    ((const int32_t *)
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__get_const_function__GetVelocity_Response__velocities(untyped_member, index));
  int32_t * value =
    (int32_t *)(untyped_value);
  *value = *item;
}

void dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__assign_function__GetVelocity_Response__velocities(
  void * untyped_member, size_t index, const void * untyped_value)
{
  int32_t * item =
    ((int32_t *)
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__get_function__GetVelocity_Response__velocities(untyped_member, index));
  const int32_t * value =
    (const int32_t *)(untyped_value);
  *item = *value;
}

bool dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__resize_function__GetVelocity_Response__velocities(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__int32__Sequence * member =
    (rosidl_runtime_c__int32__Sequence *)(untyped_member);
  rosidl_runtime_c__int32__Sequence__fini(member);
  return rosidl_runtime_c__int32__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_message_member_array[1] = {
  {
    "velocities",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response, velocities),  // bytes offset in struct
    NULL,  // default value
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__size_function__GetVelocity_Response__velocities,  // size() function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__get_const_function__GetVelocity_Response__velocities,  // get_const(index) function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__get_function__GetVelocity_Response__velocities,  // get(index) function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__fetch_function__GetVelocity_Response__velocities,  // fetch(index, &value) function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__assign_function__GetVelocity_Response__velocities,  // assign(index, value) function pointer
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__resize_function__GetVelocity_Response__velocities  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_message_members = {
  "dynamixel_sdk_custom_interfaces__srv",  // message namespace
  "GetVelocity_Response",  // message name
  1,  // number of fields
  sizeof(dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response),
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_message_member_array,  // message members
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_message_type_support_handle = {
  0,
  &dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_dynamixel_sdk_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dynamixel_sdk_custom_interfaces, srv, GetVelocity_Response)() {
  if (!dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_message_type_support_handle.typesupport_identifier) {
    dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &dynamixel_sdk_custom_interfaces__srv__GetVelocity_Response__rosidl_typesupport_introspection_c__GetVelocity_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "dynamixel_sdk_custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "dynamixel_sdk_custom_interfaces/srv/detail/get_velocity__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_service_members = {
  "dynamixel_sdk_custom_interfaces__srv",  // service namespace
  "GetVelocity",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_Request_message_type_support_handle,
  NULL  // response message
  // dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_Response_message_type_support_handle
};

static rosidl_service_type_support_t dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_service_type_support_handle = {
  0,
  &dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dynamixel_sdk_custom_interfaces, srv, GetVelocity_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dynamixel_sdk_custom_interfaces, srv, GetVelocity_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_dynamixel_sdk_custom_interfaces
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dynamixel_sdk_custom_interfaces, srv, GetVelocity)() {
  if (!dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_service_type_support_handle.typesupport_identifier) {
    dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dynamixel_sdk_custom_interfaces, srv, GetVelocity_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dynamixel_sdk_custom_interfaces, srv, GetVelocity_Response)()->data;
  }

  return &dynamixel_sdk_custom_interfaces__srv__detail__get_velocity__rosidl_typesupport_introspection_c__GetVelocity_service_type_support_handle;
}
