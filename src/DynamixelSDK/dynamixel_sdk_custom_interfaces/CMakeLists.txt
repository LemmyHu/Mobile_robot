cmake_minimum_required(VERSION 3.5)
project(dynamixel_sdk_custom_interfaces)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)

set(msg_files
  "msg/SetVelocity.msg"
  "msg/SetPosition.msg"
  "msg/MovingStatus.msg"
  "msg/PresentPWM.msg"
  "msg/PresentLoad.msg"
  "msg/PresentPosition.msg"
  "msg/PresentVelocity.msg"
  "msg/ProfileVelocity.msg"
  "msg/PositionPIDGain.msg"
  "msg/PositionTrajectory.msg"
  "msg/VelocityTrajectory.msg"
  "msg/Int32Stamped.msg"
)

set(srv_files
  "srv/GetVelocity.srv"
  "srv/GetPosition.srv"
  "srv/GetPIDParams.srv"
  "srv/SetPIDParams.srv"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${srv_files}
  DEPENDENCIES builtin_interfaces
  DEPENDENCIES std_msgs
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
