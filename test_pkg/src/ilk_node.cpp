#include <ros/ros.h> 

int main(int argc, char **argv){

    ros::init(argc, argv, "ilk_deneme_node" );
    ros::NodeHandle nh; 

    ros::Rate looprate(20); 

    while(ros::ok()){
        double secs =ros::Time::now().toSec();
        ROS_INFO("time:%f", secs);

        ros::spinOnce();
        looprate.sleep(); 
    }
}