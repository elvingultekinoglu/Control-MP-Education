#include <ros/ros.h> 

int main(int argc, char **argv){

    ros::init(argc, argv, "ilk_deneme_node" );
    ros::NodeHandle nh; 

    ros::Rate looprate(20); 

    
    double last_time; 
    while(ros::ok()){
        double secs =ros::Time::now().toSec();

        ros::spinOnce();
        looprate.sleep(); 

        last_time=secs; 
    }
}