#include <ros/ros.h>
#include <mavros_msgs/ParamSet.h> 

mavros_msgs::ParamSet param; 

int main(int argc, char **argv){
    ros::init(argc, argv, "parameter_node"); 
    ros::NodeHandle nh; 

    ros::ServiceClient param_client=nh.serviceClient<mavros_msgs::ParamSet>("/mavros/param/set"); 

    param.request.param_id="NAV_RCL_ACT";
    param.request.value.integer=1; 

    if(param_client.call(param)==true){
        ROS_INFO("Parametre ayarlandi!"); 
    }
}