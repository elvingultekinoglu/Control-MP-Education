#include <ros/ros.h>
#include <geometry_msgs/Twist.h> 

geometry_msgs::Twist vel; 

int main(int argc, char **argv){
    ros::init(argc,argv,"circle_node"); 
    ros::NodeHandle nh; 

    ros::Rate looprate(10); 

    ros::Publisher vel_pub=nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10); 

    vel.linear.x=3.0;
    vel.angular.z=1.0; 

    while(ros::ok()){
        vel_pub.publish(vel); 
        looprate.sleep();

    }
}