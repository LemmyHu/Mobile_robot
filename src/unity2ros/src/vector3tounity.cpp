#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
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

class Vector3Node : public rclcpp::Node
{
public:
    Vector3Node()
            : Node("vector3tounity")
    {
        position_publisher_ = this->create_publisher<geometry_msgs::msg::Vector3Stamped>("wheel_position", 10);
        rpy_publisher_ = this->create_publisher<geometry_msgs::msg::Vector3Stamped>("wheel_rpy", 10);

        position_subscription_ = this->create_subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>(
                "present_position", 10, std::bind(&Vector3Node::position_callback, this, std::placeholders::_1));

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
        //double dt = (current_time - last_time_).seconds();

        // Extract motor positions
        double left_wheel_position = msg->positions[0];
        double right_wheel_position = msg->positions[1];

        // Calculate the change in wheel positions (displacements) since the last update
        double delta_left_wheel_position = left_wheel_position - last_left_wheel_position_;
        double delta_right_wheel_position = right_wheel_position - last_right_wheel_position_;

        // Update last positions
        last_left_wheel_position_ = left_wheel_position;
        last_right_wheel_position_ = right_wheel_position;

        // Calculate wheel displacements in meters
        double left_wheel_displacement = delta_left_wheel_position * METERS_PER_TICK;
        double right_wheel_displacement = delta_right_wheel_position * METERS_PER_TICK;

        // Calculate the average displacement and the change in orientation
        double distance = (left_wheel_displacement + right_wheel_displacement) / 2.0;
        double delta_theta = (right_wheel_displacement - left_wheel_displacement) / WHEEL_BASE;
        //RCLCPP_INFO(this->get_logger(), "delta_theta: %f", delta_theta);
        //RCLCPP_INFO(this->get_logger(), "distance: %f", distance);

        const double K_angular = 0.018;
        // Calculate the change in pose
        double delta_x = distance * std::sin(theta_ + delta_theta / 2.0);
        double delta_y = distance * std::cos(theta_ + delta_theta / 2.0);
        //RCLCPP_INFO(this->get_logger(), "delta_x: %f", delta_x);
        //RCLCPP_INFO(this->get_logger(), "delta_y: %f", delta_y);


        // Update the robot's position and orientation
        x_ += delta_x;
        y_ += delta_y;
        theta_ += delta_theta;
        theta_k += delta_theta / K_angular;
        //RCLCPP_INFO(this->get_logger(), "x_: %f", x_);
        //RCLCPP_INFO(this->get_logger(), "y_: %f", y_);
        //RCLCPP_INFO(this->get_logger(), "theta_: %f", theta_);
        //RCLCPP_INFO(this->get_logger(), "theta_k: %f", theta_k);

        auto position_msg = geometry_msgs::msg::Vector3Stamped();
        position_msg.header.stamp = current_time;
        position_msg.header.frame_id = "wheel_position";
        position_msg.vector.x = x_;
        position_msg.vector.y = y_;
        position_msg.vector.z = 0;
        position_publisher_->publish(position_msg);

        auto rpy_msg = geometry_msgs::msg::Vector3Stamped();
        rpy_msg.header.stamp = current_time;
        rpy_msg.header.frame_id = "wheel_rpy";
        rpy_msg.vector.x = 0;
        rpy_msg.vector.y = 0;
        rpy_msg.vector.z = theta_k;
        rpy_publisher_->publish(rpy_msg);

        last_time_ = current_time;
    }

    rclcpp::Publisher<geometry_msgs::msg::Vector3Stamped>::SharedPtr position_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Vector3Stamped>::SharedPtr rpy_publisher_;
    rclcpp::Subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>::SharedPtr position_subscription_;
    rclcpp::Time last_time_;

    double last_left_wheel_position_;
    double last_right_wheel_position_;
    double x_ = 0.0;
    double y_ = 0.0;
    double theta_ = 0.0;
    double theta_k = 0.0;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Vector3Node>());
    rclcpp::shutdown();
    return 0;
}
