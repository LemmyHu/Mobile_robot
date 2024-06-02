#include <rclcpp/rclcpp.hpp>
#include "dynamixel_sdk_custom_interfaces/srv/get_pid_params.hpp"
#include "dynamixel_sdk_custom_interfaces/srv/set_pid_params.hpp"

using GetPIDParams = dynamixel_sdk_custom_interfaces::srv::GetPIDParams;
using SetPIDParams = dynamixel_sdk_custom_interfaces::srv::SetPIDParams;

class PIDClientNode : public rclcpp::Node {
public:
    PIDClientNode() : Node("pid_client_node") {
        get_pid_params_client_ = this->create_client<GetPIDParams>("get_pid_params");
        set_pid_params_client_ = this->create_client<SetPIDParams>("set_pid_params");

        // Example usage: Get and set PID parameters for multiple motors
        auto request = std::make_shared<GetPIDParams::Request>();
        request->ids = {1, 2};

        auto result = get_pid_params_client_->async_send_request(request);
        if (rclcpp::spin_until_future_complete(this->shared_from_this(), result) == rclcpp::FutureReturnCode::SUCCESS) {
            auto response = result.get();
            RCLCPP_INFO(this->get_logger(), "Current PID values:");

            for (size_t i = 0; i < response->p_gains.size(); ++i) {
                RCLCPP_INFO(this->get_logger(), "ID %d: P=%d, I=%d, D=%d", request->ids[i], response->p_gains[i], response->i_gains[i], response->d_gains[i]);
            }

            // Set new PID values
            auto set_request = std::make_shared<SetPIDParams::Request>();
            set_request->ids = {1, 2};
            set_request->p_gains = {response->p_gains[0] + 1, response->p_gains[1] + 1};
            set_request->i_gains = {response->i_gains[0] + 1, response->i_gains[1] + 1};
            set_request->d_gains = {response->d_gains[0] + 1, response->d_gains[1] + 1};

            auto set_result = set_pid_params_client_->async_send_request(set_request);
            if (rclcpp::spin_until_future_complete(this->shared_from_this(), set_result) == rclcpp::FutureReturnCode::SUCCESS) {
                auto set_response = set_result.get();
                if (set_response->success) {
                    RCLCPP_INFO(this->get_logger(), "Successfully set new PID values");
                } else {

                    RCLCPP_ERROR(this->get_logger(), "Failed to set new PID values");
                }
            } else {
                RCLCPP_ERROR(this->get_logger(), "Failed to call set_pid_params service");
            }
        } else {
            RCLCPP_ERROR(this->get_logger(), "Failed to call get_pid_params service");
        }
    }

private:
    rclcpp::Client<GetPIDParams>::SharedPtr get_pid_params_client_;
    rclcpp::Client<SetPIDParams>::SharedPtr set_pid_params_client_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PIDClientNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
