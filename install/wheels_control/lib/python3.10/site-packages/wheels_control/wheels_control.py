#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from dynamixel_sdk_custom_interfaces.msg import SetPosition
import time
import math

class MotorController(Node):
    def __init__(self):
        super().__init__('motor_higher_controller')
        self.publisher_ = self.create_publisher(SetPosition, 'set_position', 10)
        self.timer = self.create_timer(5, self.control_loop)  # Run every 5 seconds
        self.wheel_diameter_cm = 6.5  # in cm
        self.motor_max_position = 4095  # Max positional value for one full revolution

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
        self.publisher_.publish(msg)

    def control_loop(self):
        # Move forward
        self.get_logger().info("Moving forward 10cm")
        self.move_motor(10)
        time.sleep(3)

        # Move backward
        self.get_logger().info("Moving backward 10cm")
        self.move_motor(0)
        time.sleep(3)

def main(args=None):
    rclpy.init(args=args)
    wheels_controller = MotorController()
    rclpy.spin(wheels_controller)
    wheels_controller.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
