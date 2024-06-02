#!/usr/bin/env python3

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
            package='dynamixel_sdk_examples',
            executable='velocity_control_node',
            name='velocity_control_node',
            output='screen'
        ),
        Node(
            package='joystick_listener',
            executable='joystick_control.py',
            name='command_controller',
            output='screen'
        )
    ])