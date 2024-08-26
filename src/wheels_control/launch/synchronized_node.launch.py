from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch.actions import LogInfo

def generate_launch_description():
    return LaunchDescription([
        ExecuteProcess(
            cmd=['ssh', '-o', 'StrictHostKeyChecking=no', 'lemmy@192.168.43.167', 'bash -c "source /opt/ros/humble/setup.bash && source ~/robotis_ws/install/setup.bash && export ROS_DOMAIN_ID=0 && ros2 launch wheels_control wheels_control.launch.py"'],
            output='screen'
        ),
        ExecuteProcess(
            cmd=['ssh', '-o', 'StrictHostKeyChecking=no', 'lemmy@192.168.43.33', 'bash -c "source /opt/ros/humble/setup.bash && source ~/Mobile_robot/install/setup.bash && export ROS_DOMAIN_ID=0 && ros2 launch wheels_control wheels_control.launch.py"'],
            output='screen'
        ),
        LogInfo(msg="Waiting for nodes to be ready..."),
        ExecuteProcess(
            cmd=['bash', '-c', 'sleep 5 && echo start | nc 192.168.43.167 12345 && echo start | nc 192.168.43.33 12345'],
            output='screen'
        )
    ])
