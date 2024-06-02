// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from dynamixel_sdk_custom_interfaces:msg/SetPosition.idl
// generated code does not contain a copyright notice
#include "dynamixel_sdk_custom_interfaces/msg/detail/set_position__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `ids`
// Member `positions`
// Member `profile_velocities`
// Member `goal_pwms`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
dynamixel_sdk_custom_interfaces__msg__SetPosition__init(dynamixel_sdk_custom_interfaces__msg__SetPosition * msg)
{
  if (!msg) {
    return false;
  }
  // ids
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->ids, 0)) {
    dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(msg);
    return false;
  }
  // positions
  if (!rosidl_runtime_c__int32__Sequence__init(&msg->positions, 0)) {
    dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(msg);
    return false;
  }
  // profile_velocities
  if (!rosidl_runtime_c__int32__Sequence__init(&msg->profile_velocities, 0)) {
    dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(msg);
    return false;
  }
  // goal_pwms
  if (!rosidl_runtime_c__int32__Sequence__init(&msg->goal_pwms, 0)) {
    dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(msg);
    return false;
  }
  return true;
}

void
dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(dynamixel_sdk_custom_interfaces__msg__SetPosition * msg)
{
  if (!msg) {
    return;
  }
  // ids
  rosidl_runtime_c__uint8__Sequence__fini(&msg->ids);
  // positions
  rosidl_runtime_c__int32__Sequence__fini(&msg->positions);
  // profile_velocities
  rosidl_runtime_c__int32__Sequence__fini(&msg->profile_velocities);
  // goal_pwms
  rosidl_runtime_c__int32__Sequence__fini(&msg->goal_pwms);
}

bool
dynamixel_sdk_custom_interfaces__msg__SetPosition__are_equal(const dynamixel_sdk_custom_interfaces__msg__SetPosition * lhs, const dynamixel_sdk_custom_interfaces__msg__SetPosition * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // ids
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->ids), &(rhs->ids)))
  {
    return false;
  }
  // positions
  if (!rosidl_runtime_c__int32__Sequence__are_equal(
      &(lhs->positions), &(rhs->positions)))
  {
    return false;
  }
  // profile_velocities
  if (!rosidl_runtime_c__int32__Sequence__are_equal(
      &(lhs->profile_velocities), &(rhs->profile_velocities)))
  {
    return false;
  }
  // goal_pwms
  if (!rosidl_runtime_c__int32__Sequence__are_equal(
      &(lhs->goal_pwms), &(rhs->goal_pwms)))
  {
    return false;
  }
  return true;
}

bool
dynamixel_sdk_custom_interfaces__msg__SetPosition__copy(
  const dynamixel_sdk_custom_interfaces__msg__SetPosition * input,
  dynamixel_sdk_custom_interfaces__msg__SetPosition * output)
{
  if (!input || !output) {
    return false;
  }
  // ids
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->ids), &(output->ids)))
  {
    return false;
  }
  // positions
  if (!rosidl_runtime_c__int32__Sequence__copy(
      &(input->positions), &(output->positions)))
  {
    return false;
  }
  // profile_velocities
  if (!rosidl_runtime_c__int32__Sequence__copy(
      &(input->profile_velocities), &(output->profile_velocities)))
  {
    return false;
  }
  // goal_pwms
  if (!rosidl_runtime_c__int32__Sequence__copy(
      &(input->goal_pwms), &(output->goal_pwms)))
  {
    return false;
  }
  return true;
}

dynamixel_sdk_custom_interfaces__msg__SetPosition *
dynamixel_sdk_custom_interfaces__msg__SetPosition__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dynamixel_sdk_custom_interfaces__msg__SetPosition * msg = (dynamixel_sdk_custom_interfaces__msg__SetPosition *)allocator.allocate(sizeof(dynamixel_sdk_custom_interfaces__msg__SetPosition), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(dynamixel_sdk_custom_interfaces__msg__SetPosition));
  bool success = dynamixel_sdk_custom_interfaces__msg__SetPosition__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
dynamixel_sdk_custom_interfaces__msg__SetPosition__destroy(dynamixel_sdk_custom_interfaces__msg__SetPosition * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence__init(dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dynamixel_sdk_custom_interfaces__msg__SetPosition * data = NULL;

  if (size) {
    data = (dynamixel_sdk_custom_interfaces__msg__SetPosition *)allocator.zero_allocate(size, sizeof(dynamixel_sdk_custom_interfaces__msg__SetPosition), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = dynamixel_sdk_custom_interfaces__msg__SetPosition__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(&data[i - 1]);
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
dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence__fini(dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence * array)
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
      dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(&array->data[i]);
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

dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence *
dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence * array = (dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence *)allocator.allocate(sizeof(dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence__destroy(dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence__are_equal(const dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence * lhs, const dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!dynamixel_sdk_custom_interfaces__msg__SetPosition__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence__copy(
  const dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence * input,
  dynamixel_sdk_custom_interfaces__msg__SetPosition__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(dynamixel_sdk_custom_interfaces__msg__SetPosition);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    dynamixel_sdk_custom_interfaces__msg__SetPosition * data =
      (dynamixel_sdk_custom_interfaces__msg__SetPosition *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!dynamixel_sdk_custom_interfaces__msg__SetPosition__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          dynamixel_sdk_custom_interfaces__msg__SetPosition__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!dynamixel_sdk_custom_interfaces__msg__SetPosition__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
