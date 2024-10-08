cmake_minimum_required(VERSION 3.5)
project(dynamixel_sdk_examples)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(dynamixel_sdk REQUIRED)
find_package(dynamixel_sdk_custom_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)

# Build
add_executable(velocity_control_node src/velocity_control_node.cpp)
ament_target_dependencies(velocity_control_node
  dynamixel_sdk_custom_interfaces
  dynamixel_sdk
  rclcpp
)
add_executable(position_control_node src/position_control_node.cpp)
ament_target_dependencies(position_control_node
        dynamixel_sdk_custom_interfaces
        dynamixel_sdk
        rclcpp
        geometry_msgs
)
add_executable(unity2ros_control_node src/unity2ros_control_node.cpp)
ament_target_dependencies(unity2ros_control_node
        dynamixel_sdk_custom_interfaces
        dynamixel_sdk
        rclcpp
        geometry_msgs
)
add_executable(unity2ros_velocity_control_node src/unity2ros_velocity_control_node.cpp)
ament_target_dependencies(unity2ros_velocity_control_node
        dynamixel_sdk_custom_interfaces
        dynamixel_sdk
        rclcpp
        geometry_msgs
)
add_executable(unity2ros_xdrive_control_node src/unity2ros_xdrive_control_node.cpp)
ament_target_dependencies(unity2ros_xdrive_control_node
        dynamixel_sdk_custom_interfaces
        dynamixel_sdk
        rclcpp
        std_msgs
)
add_executable(pid_client_node src/pid_client_node.cpp)
ament_target_dependencies(pid_client_node
        dynamixel_sdk_custom_interfaces
        rclcpp
)

# Install
install(TARGETS
  velocity_control_node
  position_control_node
  unity2ros_control_node
  unity2ros_velocity_control_node
  unity2ros_xdrive_control_node
  pid_client_node
  DESTINATION lib/${PROJECT_NAME}
)

# Test
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
