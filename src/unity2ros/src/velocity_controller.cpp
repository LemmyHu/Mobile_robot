#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"

class VelocityController : public rclcpp::Node
{
public:
    VelocityController()
            : Node("velocity_controller")
    {
        velocity_subscription_ = this->create_subscription<geometry_msgs::msg::TwistStamped>(
                "unity/cmd_vel", 10, std::bind(&VelocityController::velocity_callback, this, std::placeholders::_1));
        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel_velocity", 10);
    }

private:
    void velocity_callback(const geometry_msgs::msg::TwistStamped::SharedPtr msg)
    {
        // Extract the twist part of the TwistStamped message
        geometry_msgs::msg::Twist twist_msg = msg->twist;

        // Directly publish the extracted twist message to the robot
        velocity_publisher_->publish(twist_msg);
    }

    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr velocity_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<VelocityController>());
    rclcpp::shutdown();
    return 0;
}
