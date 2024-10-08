cmake_minimum_required(VERSION 3.8)
project(unity2ros)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(dynamixel_sdk_custom_interfaces REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(pose_controller src/pose_controller.cpp)
add_executable(odometry_node src/odometry_node.cpp)
add_executable(twisttounity src/twisttounity.cpp)
add_executable(vector3tounity src/vector3tounity.cpp)
add_executable(velocity_controller src/velocity_controller.cpp)

ament_target_dependencies(pose_controller
        rclcpp
        geometry_msgs
        std_msgs
        tf2
        tf2_ros
        tf2_geometry_msgs
        nav_msgs
)
ament_target_dependencies(odometry_node
        dynamixel_sdk_custom_interfaces
        nav_msgs
        std_msgs
        rclcpp
        tf2
        tf2_geometry_msgs
)
ament_target_dependencies(twisttounity
        dynamixel_sdk_custom_interfaces
        geometry_msgs
        std_msgs
        rclcpp
)
ament_target_dependencies(vector3tounity
        dynamixel_sdk_custom_interfaces
        geometry_msgs
        std_msgs
        rclcpp
        tf2
        tf2_geometry_msgs
)
ament_target_dependencies(velocity_controller
        dynamixel_sdk_custom_interfaces
        geometry_msgs
        std_msgs
        rclcpp
)

include_directories(
        ${dynamixel_sdk_custom_interfaces_INCLUDE_DIRS})

install(TARGETS
        pose_controller
        odometry_node
        twisttounity
        vector3tounity
        velocity_controller
        DESTINATION lib/${PROJECT_NAME})
        
install(DIRECTORY
        launch
        DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
