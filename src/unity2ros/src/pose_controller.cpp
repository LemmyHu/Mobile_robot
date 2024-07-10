#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "nav_msgs/msg/odometry.hpp"
#include "std_msgs/msg/float64.hpp"
#include "geometry_msgs/msg/vector3_stamped.hpp"

class PoseController : public rclcpp::Node
{
public:
    PoseController()
            : Node("pose_controller")
    {
        position_subscription_ = this->create_subscription<geometry_msgs::msg::Vector3Stamped>(
                "unity/robot_position", 10, std::bind(&PoseController::position_callback, this, std::placeholders::_1));
        rpy_subscription_ = this->create_subscription<geometry_msgs::msg::Vector3Stamped>(
                "unity/robot_rpy", 10, std::bind(&PoseController::rpy_callback, this, std::placeholders::_1));
        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        odom_subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
                "odom", 10, std::bind(&PoseController::odom_callback, this, std::placeholders::_1));
    }

private:
    void position_callback(const geometry_msgs::msg::Vector3Stamped::SharedPtr msg)
    {
        // Extract target position from message
        target_position_ = msg->vector;

        RCLCPP_INFO(this->get_logger(), "Received target_position.x: %f", target_position_.x);
        RCLCPP_INFO(this->get_logger(), "Received target_position.y: %f", target_position_.y);

        // Calculate and publish velocity commands
        calculate_and_publish_velocity();
    }

    void rpy_callback(const geometry_msgs::msg::Vector3Stamped::SharedPtr msg)
    {
        // Extract target RPY from message
        target_rpy_ = msg->vector;

        RCLCPP_INFO(this->get_logger(), "Received target_yaw: %f", target_rpy_.z);

        // Calculate and publish velocity commands
        calculate_and_publish_velocity();
    }

    void calculate_and_publish_velocity()
    {
        // Calculate the difference between the current pose and the target pose
        double error_x = target_position_.x;
        double error_y = target_position_.y;

        // Use target RPY directly
        double target_yaw = target_rpy_.z;

        // Normalize the yaw error to the range [-pi, pi]
        //target_yaw = atan2(sin(target_yaw), cos(target_yaw));

        // Calculate control commands
        double distance = std::sqrt(error_x * error_x + error_y * error_y);

        // Define control gains
        const double K_linear = 1;
        const double K_angular = 0.018;

        // Calculate velocity commands
        double linear_velocity = K_linear * distance;
        double angular_velocity = K_angular * target_yaw;

        // Publish velocity commands
        geometry_msgs::msg::Twist cmd;
        cmd.linear.x = linear_velocity;
        cmd.angular.z = angular_velocity;
        velocity_publisher_->publish(cmd);
    }

    void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        current_pose_ = msg->pose.pose;
    }

    rclcpp::Subscription<geometry_msgs::msg::Vector3Stamped>::SharedPtr position_subscription_;
    rclcpp::Subscription<geometry_msgs::msg::Vector3Stamped>::SharedPtr rpy_subscription_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
    geometry_msgs::msg::Pose current_pose_;  // Current pose of the robot from odometry

    geometry_msgs::msg::Vector3 target_position_; // Target position from Unity
    geometry_msgs::msg::Vector3 target_rpy_;      // Target RPY from Unity
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PoseController>());
    rclcpp::shutdown();
    return 0;
}
