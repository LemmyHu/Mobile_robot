from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='dynamixel_sdk_examples',
            executable='position_control_node',
            name='position_control_node',
            output='screen'
        ),
        Node(
            package='wheels_control',
            executable='wheels_control',
            name='motor_higher_controller',
            output='screen'
        ),
    ])
