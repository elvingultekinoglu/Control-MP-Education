#include <ros/ros.h>
#include <mavros_msgs/SetMode.h> 

mavros_msgs::SetMode mode; 

int main(int argc, char**argv){
    ros::init(argc, argv, "mode_node");
    ros::NodeHandle nh; 

    ros::ServiceClient mode_client=nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode"); 

    mode.request.custom_mode="OFFBOARD"; 

    if(mode_client.call(mode)==true){
        ROS_INFO("Mode ayarlandi!"); 
    }
}