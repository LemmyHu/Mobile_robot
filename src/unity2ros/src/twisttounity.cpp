#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "dynamixel_sdk_custom_interfaces/msg/present_position.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"

// Robot parameters
const double WHEEL_DIAMETER = 0.065;  // meters
const double WHEEL_RADIUS = WHEEL_DIAMETER / 2;
const double WHEEL_BASE = 0.1475;  // meters
const double TICKS_PER_REVOLUTION = 4095;
const double WHEEL_CIRCUMFERENCE = M_PI * WHEEL_DIAMETER;
const double METERS_PER_TICK = WHEEL_CIRCUMFERENCE / TICKS_PER_REVOLUTION;

using namespace std::chrono_literals;

class TwistNode : public rclcpp::Node
{
public:
    TwistNode()
            : Node("twisttounity")
    {
        twist_publisher_ = this->create_publisher<geometry_msgs::msg::TwistStamped>("wheel_twist", 10);

        position_subscription_ = this->create_subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>(
                "present_position", 10, std::bind(&TwistNode::position_callback, this, std::placeholders::_1));

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

        double angular_velocity = delta_theta / 3.5;

        // Publish twist (linear and angular velocities) as TwistStamped
        auto twist_msg = geometry_msgs::msg::TwistStamped();
        twist_msg.header.stamp = current_time;
        twist_msg.header.frame_id = "wheel_twist";
        twist_msg.twist.linear.x = distance / dt;
        twist_msg.twist.linear.y = 0;
        twist_msg.twist.linear.z = 0;
        twist_msg.twist.angular.z = angular_velocity / dt;
        twist_publisher_->publish(twist_msg);

        last_time_ = current_time;
    }

    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr twist_publisher_;
    rclcpp::Subscription<dynamixel_sdk_custom_interfaces::msg::PresentPosition>::SharedPtr position_subscription_;
    rclcpp::Time last_time_;

    double last_left_wheel_position_ = 0.0;
    double last_right_wheel_position_ = 0.0;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TwistNode>());
    rclcpp::shutdown();
    return 0;
}
