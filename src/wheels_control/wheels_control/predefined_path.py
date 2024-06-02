#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from dynamixel_sdk_custom_interfaces.msg import SetPosition
import math
import time

class MotorController(Node):
    def __init__(self):
        super().__init__('predefined_path')
        self.publisher_ = self.create_publisher(SetPosition, 'set_position', 10)
        self.wheel_diameter_cm = 6.5  # Wheel diameter in cm
        self.track_width_cm = 14.75  # Distance between the two wheels of the robot
        self.motor_max_position = 4095  # Max positional value for full revolution
        self.state = 0  # Initial state of the state machine
        self.current_position_right = 0  # Current position along the movement axis in motor steps
        self.current_position_left = 0
        self.home_position_right = 0  # Home position
        self.home_position_left = 0

        # Timer initialization, executes every 3 seconds
        self.action_timer = self.create_timer(3, self.execute_action)



    def calculate_motor_position(self, distance_cm):
        circumference = math.pi * self.wheel_diameter_cm
        fraction_of_revolution = distance_cm / circumference
        motor_position = int(fraction_of_revolution * self.motor_max_position)
        return motor_position

    def return_home(self):
        msg = SetPosition()
        msg.ids = [1, 2]
        msg.positions = [self.home_position_right, self.home_position_left]
        self.publisher_.publish(msg)
        self.current_position_right = self.home_position_right  # Reset current positions
        self.current_position_left = self.home_position_left
        self.get_logger().info('Returned to home position.')

    def move_motor(self, distance_cm):
        additional_steps = self.calculate_motor_position(distance_cm)
        self.current_position_right += additional_steps  # Update current position in motor steps
        self.current_position_left += additional_steps
        msg = SetPosition()
        msg.ids = [1, 2]  # Assuming two motors move the same for forward motion
        msg.positions = [self.current_position_right, self.current_position_left]
        self.publisher_.publish(msg)

    def rotate(self, degrees):
        track_circumference = math.pi * self.track_width_cm
        rotation_distance_cm = (track_circumference * degrees) / 360.0
        rotation_steps = self.calculate_motor_position(rotation_distance_cm)

        if degrees > 0:
            # Clockwise rotation
            new_right_position = self.current_position_right + rotation_steps
            new_left_position = self.current_position_left - rotation_steps
        elif degrees < 0:
            # Counterclockwise rotation
            new_right_position = self.current_position_right - rotation_steps
            new_left_position = self.current_position_left + rotation_steps
        else:
            # No rotation (degrees == 0), positions remain unchanged
            new_right_position = self.current_position_right
            new_left_position = self.current_position_left

        # Update the current positions to reflect the rotation
        self.current_position_right = new_right_position
        self.current_position_left = new_left_position

        # Create and publish the SetPosition message
        msg = SetPosition()
        msg.ids = [1, 2]
        msg.positions = [self.current_position_right, self.current_position_left]
        self.publisher_.publish(msg)


    def execute_action(self):
        if self.state == 0:
            self.get_logger().info('Moving forward 50cm')
            self.move_motor(50)
            self.state = 1
        elif self.state == 1:
            self.get_logger().info('Rotating 90 degrees')
            self.rotate(90)
            self.state = 2
        elif self.state == 2:
            self.get_logger().info('Moving forward 30cm')
            self.move_motor(30)
            self.state = 3
        elif self.state == 3:
            self.get_logger().info('Rotating 90 degrees')
            self.rotate(90)
            self.state = 4
        elif self.state == 4:
            self.get_logger().info('Moving forward 40cm')
            self.move_motor(37.6)
            self.state = 5
        elif self.state == 5:
            self.get_logger().info('Rotating -90 degrees')
            self.rotate(270)
            self.state = 6
        elif self.state == 6:
            self.get_logger().info('Moving forward 20cm')
            self.move_motor(20)
            self.state = 7
        elif self.state == 7:
            self.get_logger().info('Rotating -90 degrees')
            self.rotate(270)
            self.state = 8
        elif self.state == 8:
            self.get_logger().info('Moving forward 80cm')
            self.move_motor(80)
            time.sleep(5)
            self.state = 9
        elif self.state == 9:
            self.get_logger().info('Moving backward 80cm')
            self.move_motor(-80)
            self.state = 10
        elif self.state == 10:
            self.get_logger().info('Rotating -90 degrees')
            self.rotate(90)
            self.state = 11
        elif self.state == 11:
            self.get_logger().info('Moving forward 20cm')
            self.move_motor(-20)
            self.state = 12
        elif self.state == 12:
            self.get_logger().info('Rotating -90 degrees')
            self.rotate(90)
            self.state = 13
        elif self.state == 13:
            self.get_logger().info('Moving forward 20cm')
            self.move_motor(-37.6)
            self.state = 14
        elif self.state == 14:
            self.get_logger().info('Rotating -90 degrees')
            self.rotate(270)
            self.state = 15
        elif self.state == 15:
            self.get_logger().info('Moving forward 20cm')
            self.move_motor(-30)
            self.state = 16
        elif self.state == 16:
            self.get_logger().info('Rotating -90 degrees')
            self.rotate(270)
            self.state = 17
        elif self.state == 17:
            self.get_logger().info('Moving forward 20cm')
            self.move_motor(-50)
            self.action_timer.cancel()  # Cancel the current timer
            self.action_timer = self.create_timer(2, self.return_home)
            self.state = 17
        elif self.state == 18:
            self.get_logger().info('Returning to home position')
            self.return_home()
            self.state = 0



def main(args=None):
    rclpy.init(args=args)
    predefined_path = MotorController()
    rclpy.spin(predefined_path)
    predefined_path.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()