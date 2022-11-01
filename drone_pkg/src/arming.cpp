#include <ros/ros.h> 
#include <mavros_msgs/CommandBool.h> 

mavros_msgs::CommandBool arm; 

int main(int argc, char **argv){
    ros::init(argc,argv, "arming_node"); 
    ros::NodeHandle nh; 

    ros::ServiceClient arming_client=nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming"); 

    arm.request.value=true; 
    if(arming_client.call(arm)==true){
        if(arm.response.result==0){
            ROS_INFO("Arming is successfull!"); 
        }
        
    }

}