#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/quaternion.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_velocity.hpp"
#include <tf2/LinearMath/Quaternion.h>

using std::placeholders::_1;

class OdometryNode : public rclcpp::Node {
public:
    OdometryNode() : Node("odometry_node") {
        odom_pub_ = this->create_publisher<nav_msgs::msg::Odometry>("odom", 10);
        position_sub_ = this->create_subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>(
                "present_position", 10, std::bind(&OdometryNode::positionCallback, this, std::placeholders::_1));
        velocity_sub_ = this->create_subscription<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>(
                "present_velocity", 10, std::bind(&OdometryNode::velocityCallback, this, std::placeholders::_1));

        x_ = 0.0;
        y_ = 0.0;
        th_ = 0.0;
    }

private:
    void positionCallback(const dynamixel_sdk_custom_interfaces::msg::PresentPosition::SharedPtr msg) {
        last_position_ = msg;
    }

    void velocityCallback(const dynamixel_sdk_custom_interfaces::msg::PresentVelocity::SharedPtr msg) {
        last_velocity_ = msg;
        computeOdometry();
    }

    void computeOdometry() {
        if (!last_position_ || !last_velocity_) {
            return;
        }

        auto now = this->get_clock()->now();
        auto dt = (now - last_time_).seconds();
        last_time_ = now;

        // Assuming last_velocity_->velocities[0] is the left wheel velocity
        // and last_velocity_->velocities[1] is the right wheel velocity
        double left_wheel_velocity = last_velocity_->velocities[0];
        double right_wheel_velocity = last_velocity_->velocities[1];

        // Parameters specific to your robot
        double wheel_radius = 0.0325; // Radius of the wheels in meters
        double wheel_base = 0.1475; // Distance between the wheels in meters

        // Compute the linear and angular velocities
        double vx = wheel_radius * (left_wheel_velocity + right_wheel_velocity) / 2.0;
        double vy = 0.0; // No lateral movement in a differential drive robot
        double vth = wheel_radius * (right_wheel_velocity - left_wheel_velocity) / wheel_base;

        // Compute changes in position and orientation
        double delta_x = vx * dt;
        double delta_y = vy * dt;
        double delta_th = vth * dt;

        // Update the position and orientation
        x_ += delta_x * cos(th_) - delta_y * sin(th_);
        y_ += delta_x * sin(th_) + delta_y * cos(th_);
        th_ += delta_th;

        // Convert the orientation to a quaternion
        tf2::Quaternion q;
        q.setRPY(0, 0, th_);
        geometry_msgs::msg::Quaternion odom_quat;
        odom_quat.x = q.x();
        odom_quat.y = q.y();
        odom_quat.z = q.z();
        odom_quat.w = q.w();

        // Populate the odometry message
        nav_msgs::msg::Odometry odom;
        odom.header.stamp = now;
        odom.header.frame_id = "odom";

        // Position and orientation
        odom.pose.pose.position.x = x_;
        odom.pose.pose.position.y = y_;
        odom.pose.pose.position.z = 0.0;
        odom.pose.pose.orientation = odom_quat;

        // Linear and angular velocity
        odom.child_frame_id = "base_link";
        odom.twist.twist.linear.x = vx;
        odom.twist.twist.linear.y = vy;
        odom.twist.twist.angular.z = vth;

        odom_pub_->publish(odom);
    }

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
    rclcpp::Subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>::SharedPtr position_sub_;
    rclcpp::Subscription<dynamixel_sdk_custom_interfaces::msg::PresentVelocity>::SharedPtr velocity_sub_;

    dynamixel_sdk_custom_interfaces::msg::PresentPosition::SharedPtr last_position_;
    dynamixel_sdk_custom_interfaces::msg::PresentVelocity::SharedPtr last_velocity_;
    rclcpp::Time last_time_;

    double x_;
    double y_;
    double th_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<OdometryNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
