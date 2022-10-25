#include <ros/ros.h> 

int main(int argc, char **argv){

    ros::init(argc, argv, "ilk_deneme_node" );
    ros::NodeHandle nh; 

    ros::Rate looprate(20); 

    while(ros::ok()){
        ROS_INFO("Hello world!");

        ros::spinOnce();
        looprate.sleep(); 
    }
}