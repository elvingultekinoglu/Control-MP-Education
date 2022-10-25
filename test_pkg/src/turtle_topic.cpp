#include <ros/ros.h> 
#include <geometry_msgs/Twist.h> 

geometry_msgs::Twist hiz; 

int main(int argc, char ** argv){

    ros::init(argc, argv, "turtlesim_topic_node"); 
    ros::NodeHandle nh; 

    ros::Rate looprate(20); 

    ros::Publisher pub=nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10); 

    hiz.linear.x=1.0; 
    hiz.linear.y=1.0; 

    while(ros::ok()){

        pub.publish(hiz); 
        looprate.sleep(); 
        
    }

}