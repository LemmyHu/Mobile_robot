#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from dynamixel_sdk_custom_interfaces.msg import SetPosition
import time

class MotorController(Node):
    def __init__(self):
        super().__init__('motor_higher_controller')
        self.publisher_ = self.create_publisher(SetPosition, 'set_position', 10)
        self.timer = self.create_timer(5, self.control_loop)  # Run every 5 seconds

    def move_motor(self, position):
        msg = SetPosition()
        msg.ids = [1, 2]
        msg.positions = [position, position]
        self.publisher_.publish(msg)

    def control_loop(self):
        # Move forward
        self.move_motor(6300)
        time.sleep(3)

        # Move backward
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
