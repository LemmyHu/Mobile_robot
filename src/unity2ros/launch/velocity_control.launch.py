#!/usr/bin/env python3
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='dynamixel_sdk_examples',
            executable='unity2ros_velocity_control_node',
            name='unity2ros_velocity_control_node'
        ),
        Node(
            package='unity2ros',
            executable='twisttounity',
            name='twisttounity'
        ),
        Node(
            package='unity2ros',
            executable='velocity_controller',
            name='velocity_controller'
        ),
    ])
