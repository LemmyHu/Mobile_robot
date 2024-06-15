#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "nav_msgs/msg/odometry.hpp"

class PoseController : public rclcpp::Node
{
public:
    PoseController()
            : Node("pose_controller")
    {
        pose_subscription_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
                "unity/robot_pose", 10, std::bind(&PoseController::pose_callback, this, std::placeholders::_1));
        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        odom_subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
                "odom", 10, std::bind(&PoseController::odom_callback, this, std::placeholders::_1));
    }

private:
    void pose_callback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
    {
        // Extract target pose from message
        const auto& target_pose = msg->pose;

        // Calculate the difference between the current pose and the target pose
        double error_x = target_pose.position.x - current_pose_.position.x;
        double error_y = target_pose.position.y - current_pose_.position.y;

        tf2::Quaternion target_quat, current_quat;
        tf2::convert(target_pose.orientation, target_quat);
        tf2::convert(current_pose_.orientation, current_quat);

        // Extract yaw from the quaternions using tf2::Matrix3x3
        double target_roll, target_pitch, target_yaw;
        double current_roll, current_pitch, current_yaw;
        tf2::Matrix3x3(target_quat).getRPY(target_roll, target_pitch, target_yaw);
        tf2::Matrix3x3(current_quat).getRPY(current_roll, current_pitch, current_yaw);
        double error_yaw = target_yaw - current_yaw;

        // Normalize the yaw error to the range [-pi, pi]
        error_yaw = atan2(sin(error_yaw), cos(error_yaw));

        // Calculate control commands
        double distance = std::sqrt(error_x * error_x + error_y * error_y);
        double angle_to_target = std::atan2(error_y, error_x);
        double angle_diff = angle_to_target - current_yaw;

        // Normalize the angle difference to the range [-pi, pi]
        angle_diff = atan2(sin(angle_diff), cos(angle_diff));

        // Define control gains
        const double K_linear = 1.0;
        const double K_angular = 1.0;

        // Calculate velocity commands
        double linear_velocity = K_linear * distance;
        double angular_velocity = K_angular * (angle_diff + error_yaw);

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

    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr pose_subscription_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
    geometry_msgs::msg::Pose current_pose_;  // Current pose of the robot from odometry
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PoseController>());
    rclcpp::shutdown();
    return 0;
}
