#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from dynamixel_sdk_custom_interfaces.msg import SetPosition
import math

class MotorController(Node):
    def __init__(self):
        super().__init__('motor_higher_controller')
        self.publisher_ = self.create_publisher(SetPosition, 'set_position', 10)
        self.wheel_diameter_cm = 6.5  # in cm
        self.motor_max_position = 4095  # Max positional value for one full revolution
        self.profile_velocity = 270
        self.goal_pwm = 855
        # Timer initialization
        self.action_timer = self.create_timer(1, self.move_forward)  # Initial delay before starting the loop

    def calculate_motor_position(self, distance_cm):
        circumference = math.pi * self.wheel_diameter_cm
        fraction_of_revolution = distance_cm / circumference
        motor_position = int(fraction_of_revolution * self.motor_max_position)
        return motor_position

    def move_motor(self, distance_cm):
        position = self.calculate_motor_position(distance_cm)
        msg = SetPosition()
        msg.ids = [1, 2]  # Control two motors
        msg.positions = [position, position]
        msg.profile_velocities = [self.profile_velocity, self.profile_velocity]
        msg.goal_pwms = [self.goal_pwm, self.goal_pwm]
        self.publisher_.publish(msg)

    def move_forward(self):
        self.get_logger().info('Moving forward 50cm')
        self.move_motor(50)
        self.action_timer.cancel()  # Cancel the current timer
        self.action_timer = self.create_timer(6, self.move_backward)  # Set a timer for the backward action

    def move_backward(self):
        self.get_logger().info('Moving backward 50cm')
        self.move_motor(0)
        self.action_timer.cancel()  # Cancel the current timer
        self.action_timer = self.create_timer(6, self.move_forward)  # Set the timer to move forward again


def main(args=None):
    rclpy.init(args=args)
    wheels_controller = MotorController()
    rclpy.spin(wheels_controller)
    wheels_controller.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
