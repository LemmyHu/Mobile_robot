#!/usr/bin/env python3
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='dynamixel_sdk_examples',
            executable='unity2ros_control_node',
            name='unity2ros_control_node'
        ),
        Node(
            package='unity2ros',
            executable='odometry_node',
            name='odometry_node'
        ),
        Node(
            package='unity2ros',
            executable='pose_controller',
            name='pose_controller'
        ),
    ])
