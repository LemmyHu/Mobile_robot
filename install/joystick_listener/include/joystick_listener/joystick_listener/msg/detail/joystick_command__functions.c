// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from joystick_listener:msg/JoystickCommand.idl
// generated code does not contain a copyright notice
#include "joystick_listener/msg/detail/joystick_command__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `values`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
joystick_listener__msg__JoystickCommand__init(joystick_listener__msg__JoystickCommand * msg)
{
  if (!msg) {
    return false;
  }
  // mode
  // values
  if (!rosidl_runtime_c__int32__Sequence__init(&msg->values, 0)) {
    joystick_listener__msg__JoystickCommand__fini(msg);
    return false;
  }
  return true;
}

void
joystick_listener__msg__JoystickCommand__fini(joystick_listener__msg__JoystickCommand * msg)
{
  if (!msg) {
    return;
  }
  // mode
  // values
  rosidl_runtime_c__int32__Sequence__fini(&msg->values);
}

bool
joystick_listener__msg__JoystickCommand__are_equal(const joystick_listener__msg__JoystickCommand * lhs, const joystick_listener__msg__JoystickCommand * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // mode
  if (lhs->mode != rhs->mode) {
    return false;
  }
  // values
  if (!rosidl_runtime_c__int32__Sequence__are_equal(
      &(lhs->values), &(rhs->values)))
  {
    return false;
  }
  return true;
}

bool
joystick_listener__msg__JoystickCommand__copy(
  const joystick_listener__msg__JoystickCommand * input,
  joystick_listener__msg__JoystickCommand * output)
{
  if (!input || !output) {
    return false;
  }
  // mode
  output->mode = input->mode;
  // values
  if (!rosidl_runtime_c__int32__Sequence__copy(
      &(input->values), &(output->values)))
  {
    return false;
  }
  return true;
}

joystick_listener__msg__JoystickCommand *
joystick_listener__msg__JoystickCommand__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  joystick_listener__msg__JoystickCommand * msg = (joystick_listener__msg__JoystickCommand *)allocator.allocate(sizeof(joystick_listener__msg__JoystickCommand), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(joystick_listener__msg__JoystickCommand));
  bool success = joystick_listener__msg__JoystickCommand__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
joystick_listener__msg__JoystickCommand__destroy(joystick_listener__msg__JoystickCommand * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    joystick_listener__msg__JoystickCommand__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
joystick_listener__msg__JoystickCommand__Sequence__init(joystick_listener__msg__JoystickCommand__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  joystick_listener__msg__JoystickCommand * data = NULL;

  if (size) {
    data = (joystick_listener__msg__JoystickCommand *)allocator.zero_allocate(size, sizeof(joystick_listener__msg__JoystickCommand), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = joystick_listener__msg__JoystickCommand__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        joystick_listener__msg__JoystickCommand__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
joystick_listener__msg__JoystickCommand__Sequence__fini(joystick_listener__msg__JoystickCommand__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      joystick_listener__msg__JoystickCommand__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

joystick_listener__msg__JoystickCommand__Sequence *
joystick_listener__msg__JoystickCommand__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  joystick_listener__msg__JoystickCommand__Sequence * array = (joystick_listener__msg__JoystickCommand__Sequence *)allocator.allocate(sizeof(joystick_listener__msg__JoystickCommand__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = joystick_listener__msg__JoystickCommand__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
joystick_listener__msg__JoystickCommand__Sequence__destroy(joystick_listener__msg__JoystickCommand__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    joystick_listener__msg__JoystickCommand__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
joystick_listener__msg__JoystickCommand__Sequence__are_equal(const joystick_listener__msg__JoystickCommand__Sequence * lhs, const joystick_listener__msg__JoystickCommand__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!joystick_listener__msg__JoystickCommand__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
joystick_listener__msg__JoystickCommand__Sequence__copy(
  const joystick_listener__msg__JoystickCommand__Sequence * input,
  joystick_listener__msg__JoystickCommand__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(joystick_listener__msg__JoystickCommand);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    joystick_listener__msg__JoystickCommand * data =
      (joystick_listener__msg__JoystickCommand *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!joystick_listener__msg__JoystickCommand__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          joystick_listener__msg__JoystickCommand__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!joystick_listener__msg__JoystickCommand__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
