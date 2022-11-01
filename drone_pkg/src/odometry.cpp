#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <mavros_msgs/State.h> 

nav_msgs::Odometry odom;
mavros_msgs::State state; 

void odom_cb(const nav_msgs::Odometry::ConstPtr &msg){
    odom= *msg; 
}

void state_cb(const mavros_msgs::State::ConstPtr &msg){
    state= *msg; 
}

int main(int argc, char **argv){
    ros::init(argc, argv, "odometry_node");
    ros::NodeHandle nh; 

    ros::Rate looprate(20); 

    ros::Subscriber odom_sub=nh.subscribe<nav_msgs::Odometry>("/mavros/global_position/local", 10, odom_cb);
    ros::Subscriber state_sub=nh.subscribe<mavros_msgs::State>("/mavros/state",10,state_cb);

    while(ros::ok()){
        ROS_INFO("x:%f, y:%f, z:%f, mode:%s", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z, state.mode.c_str());
        ros::spinOnce();
        looprate.sleep(); 
    }
}
