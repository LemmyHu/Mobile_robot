// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from joystick_listener:msg/JoystickCommand.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "joystick_listener/msg/detail/joystick_command__struct.h"
#include "joystick_listener/msg/detail/joystick_command__type_support.h"
#include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/message_type_support_dispatch.h"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_c/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace joystick_listener
{

namespace msg
{

namespace rosidl_typesupport_c
{

typedef struct _JoystickCommand_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _JoystickCommand_type_support_ids_t;

static const _JoystickCommand_type_support_ids_t _JoystickCommand_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _JoystickCommand_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _JoystickCommand_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _JoystickCommand_type_support_symbol_names_t _JoystickCommand_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, joystick_listener, msg, JoystickCommand)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, joystick_listener, msg, JoystickCommand)),
  }
};

typedef struct _JoystickCommand_type_support_data_t
{
  void * data[2];
} _JoystickCommand_type_support_data_t;

static _JoystickCommand_type_support_data_t _JoystickCommand_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _JoystickCommand_message_typesupport_map = {
  2,
  "joystick_listener",
  &_JoystickCommand_message_typesupport_ids.typesupport_identifier[0],
  &_JoystickCommand_message_typesupport_symbol_names.symbol_name[0],
  &_JoystickCommand_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t JoystickCommand_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_JoystickCommand_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_c

}  // namespace msg

}  // namespace joystick_listener

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, joystick_listener, msg, JoystickCommand)() {
  return &::joystick_listener::msg::rosidl_typesupport_c::JoystickCommand_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
