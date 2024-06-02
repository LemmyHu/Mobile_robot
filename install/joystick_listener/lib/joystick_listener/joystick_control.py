#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import socket
from dynamixel_sdk_custom_interfaces.msg import SetPosition, SetVelocity
from joystick_listener.msg import JoystickCommand


class CommandController(Node):
    def __init__(self):
        super().__init__('command_controller')
        self.position_publisher = self.create_publisher(SetPosition, 'set_position', 1)
        self.velocity_publisher = self.create_publisher(SetVelocity, 'set_velocity', 1)

        # Setup UDP Socket
        self.udp_ip = '0.0.0.0'
        self.udp_port = 4210
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((self.udp_ip, self.udp_port))
        self.create_timer(0.1, self.listen_udp)

    def listen_udp(self):
        data, addr = self.sock.recvfrom(1024)  # buffer size is 1024 bytes
        msg = self.parse_joystick_data(data.decode())
        self.process_joystick_input(msg)

    def parse_joystick_data(self, data):
        # Parse the incoming data into a JoystickCommand message
        xValue, yValue, mode = map(int, data.split(','))
        msg = JoystickCommand()
        msg.mode = mode
        msg.values = [xValue, yValue]  # Store x and y values in the list
        return msg

    def process_joystick_input(self, msg):
        if msg.mode == 0:  # Position mode
            position_command = SetPosition()
            position_command.positions = msg.values
            self.position_publisher.publish(position_command)
        elif msg.mode == 1:  # Velocity mode
            velocity_command = SetVelocity()
            velocity_command.velocities = msg.values
            self.velocity_publisher.publish(velocity_command)


def main(args=None):
    rclpy.init(args=args)
    joystick_controller = CommandController()
    rclpy.spin(joystick_controller)
    joystick_controller.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
