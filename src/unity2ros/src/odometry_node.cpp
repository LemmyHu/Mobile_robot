#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

// Robot parameters
const double WHEEL_DIAMETER = 0.065;  // meters
const double WHEEL_RADIUS = WHEEL_DIAMETER / 2;
const double WHEEL_BASE = 0.1475;  // meters
const double TICKS_PER_REVOLUTION = 4095;
const double WHEEL_CIRCUMFERENCE = M_PI * WHEEL_DIAMETER;
const double METERS_PER_TICK = WHEEL_CIRCUMFERENCE / TICKS_PER_REVOLUTION;

using namespace std::chrono_literals;

class OdometryNode : public rclcpp::Node
{
public:
    OdometryNode()
            : Node("odometry_node")
    {
        odom_publisher_ = this->create_publisher<nav_msgs::msg::Odometry>("odom", 10);
        tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

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
        //RCLCPP_INFO(this->get_logger(), "delta_theta: %f", delta_theta);

        // Calculate the change in pose
        double delta_x = distance * cos(theta_ + delta_theta / 2.0);
        double delta_y = distance * sin(theta_ + delta_theta / 2.0);

        //RCLCPP_INFO(this->get_logger(), "distance: %f", distance);
        //RCLCPP_INFO(this->get_logger(), "theta_: %f", theta_);
        //RCLCPP_INFO(this->get_logger(), "delta_theta: %f", delta_theta);
        //RCLCPP_INFO(this->get_logger(), "delta_x: %f", delta_x);
        //RCLCPP_INFO(this->get_logger(), "delta_y: %f", delta_y);

        // Update the pose
        x_ += delta_x;
        y_ += delta_y;
        theta_ += delta_theta;
        //RCLCPP_INFO(this->get_logger(), "theta_: %f", theta_);

        // Normalize theta to the range [-pi, pi]
        //theta_ = atan2(sin(theta_), cos(theta_));
        //RCLCPP_INFO(this->get_logger(), "theta_: %f", theta_);

        // Publish odometry message
        auto odom_msg = nav_msgs::msg::Odometry();
        odom_msg.header.stamp = current_time;
        odom_msg.header.frame_id = "odom";

        // Set the position
        odom_msg.pose.pose.position.x = x_;
        odom_msg.pose.pose.position.y = y_;
        odom_msg.pose.pose.position.z = 0.0;
        // Set teh orientation
        geometry_msgs::msg::Quaternion q;
        q.w = std::cos(theta_ / 2.0);
        q.x = 0.0;
        q.y = 0.0;
        q.z = std::sin(theta_ / 2.0);
        odom_msg.pose.pose.orientation = q;

        // Set the velocity
        odom_msg.child_frame_id = "base_link";
        odom_msg.twist.twist.linear.x = delta_x / dt;
        odom_msg.twist.twist.linear.y = delta_y / dt;
        odom_msg.twist.twist.angular.z = delta_theta / dt;

        odom_publisher_->publish(odom_msg);

        geometry_msgs::msg::TransformStamped odom_tf;
        odom_tf.header.stamp = current_time;
        odom_tf.header.frame_id = "odom";
        odom_tf.child_frame_id = "base_link";
        odom_tf.transform.translation.x = x_;
        odom_tf.transform.translation.y = y_;
        odom_tf.transform.translation.z = 0.0;
        odom_tf.transform.rotation = q;

        tf_broadcaster_->sendTransform(odom_tf);
        last_time_ = current_time;
    }

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publisher_;
    rclcpp::Subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>::SharedPtr position_subscription_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
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
