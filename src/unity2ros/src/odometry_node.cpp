#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

// Robot parameters
const double WHEEL_DIAMETER = 0.065;  // meters
const double WHEEL_BASE = 0.1475;  // meters
const double TICKS_PER_REVOLUTION = 4095;
const double WHEEL_CIRCUMFERENCE = M_PI * WHEEL_DIAMETER;
const double METERS_PER_TICK = WHEEL_CIRCUMFERENCE / TICKS_PER_REVOLUTION;

class OdometryNode : public rclcpp::Node
{
public:
    OdometryNode()
            : Node("odometry_node")
    {
        odom_publisher_ = this->create_publisher<nav_msgs::msg::Odometry>("odom", 10);

        // Subscribe to present_position topic
        position_subscription_ = this->create_subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>(
                "present_position", 10, std::bind(&OdometryNode::position_callback, this, std::placeholders::_1));

        last_time_ = this->now();
    }

private:
    void position_callback(const dynamixel_sdk_custom_interfaces::msg::PresentPosition::SharedPtr msg)
    {
        if (msg->ids.size() < 2) {
            RCLCPP_ERROR(this->get_logger(), "Not enough motor positions received.");
            return;
        }

        auto current_time = this->now();
        double dt = (current_time - last_time_).seconds();

        // Extract motor positions
        double left_wheel_position = msg->positions[0];
        double right_wheel_position = msg->positions[1];

        // Calculate wheel displacements in meters
        double left_wheel_displacement = left_wheel_position * METERS_PER_TICK;
        double right_wheel_displacement = right_wheel_position * METERS_PER_TICK;

        // Calculate the average displacement and the change in orientation
        double distance = (left_wheel_displacement + right_wheel_displacement) / 2.0;
        double delta_theta = (right_wheel_displacement - left_wheel_displacement) / WHEEL_BASE;

        // Calculate the change in pose
        double delta_x = distance * cos(theta_ + delta_theta / 2.0);
        double delta_y = distance * sin(theta_ + delta_theta / 2.0);

        // Update the pose
        x_ += delta_x;
        y_ += delta_y;
        theta_ += delta_theta;

        // Normalize theta to the range [-pi, pi]
        theta_ = atan2(sin(theta_), cos(theta_));

        // Publish odometry message
        auto odom_msg = nav_msgs::msg::Odometry();
        odom_msg.header.stamp = current_time;
        odom_msg.header.frame_id = "odom";

        // Set the position
        odom_msg.pose.pose.position.x = x_;
        odom_msg.pose.pose.position.y = y_;
        tf2::Quaternion q;
        q.setRPY(0, 0, theta_);
        odom_msg.pose.pose.orientation = tf2::toMsg(q);

        // Set the velocity
        odom_msg.child_frame_id = "base_link";
        odom_msg.twist.twist.linear.x = distance / dt;
        odom_msg.twist.twist.angular.z = delta_theta / dt;

        odom_publisher_->publish(odom_msg);
        last_time_ = current_time;
    }

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publisher_;
    rclcpp::Subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>::SharedPtr position_subscription_;
    rclcpp::Time last_time_;

    double x_ = 0.0;
    double y_ = 0.0;
    double theta_ = 0.0;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OdometryNode>());
    rclcpp::shutdown();
    return 0;
}
