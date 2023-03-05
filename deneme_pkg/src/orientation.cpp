#include <ros/ros.h>
#include <deneme_pkg/takeoff_srv.h> 
#include <tf2/LinearMath/Quaternion.h> 
#include <geometry_msgs/PoseStamped.h> 
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>

nav_msgs::Odometry odom; 
deneme_pkg::takeoff_srv servis; 
geometry_msgs::PoseStamped pose; 

ros::Publisher pose_pub; 
double yaw1, roll1, pitch1; 

void odom_cb(const nav_msgs::Odometry::ConstPtr &msg){
    pose.pose.position.x= msg->pose.pose.position.x;
    pose.pose.position.y= msg->pose.pose.position.y;
    pose.pose.position.z= msg->pose.pose.position.z;

    tf::Quaternion quat_2(msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w); 
    tf::Matrix3x3 m(quat_2); 

    m.getRPY(roll1, pitch1, yaw1); 

    pose_pub.publish(pose); 
}

int main(int argc, char **argv){
    ros::init(argc, argv, "orientation_code");
    ros::NodeHandle nh; 

    ros::ServiceClient servis_client=nh.serviceClient<deneme_pkg::takeoff_srv>("/location_service"); 
    pose_pub=nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",10);
    ros::Subscriber odom_sub=nh.subscribe<nav_msgs::Odometry>("/mavros/global_position/local",10,odom_cb); 

    ros::Rate looprate(20); 

    tf2::Quaternion quat; 

    double yaw, roll, pitch; 
    yaw=-90; 

    quat.setRPY(roll, pitch, yaw*(3.14/180)); 
    quat=quat.normalize(); 

    pose.pose.orientation.x=quat.x();
    pose.pose.orientation.y=quat.y();
    pose.pose.orientation.z=quat.z();
    pose.pose.orientation.w=quat.w(); 

    pose.pose.position.z=5.0; 

    servis.request.poseStamped=pose; 

    servis_client.call(servis); 

    while(ros::ok()){
        ROS_INFO("yaw:%f", yaw1); 
        ros::spinOnce();
        looprate.sleep(); 
    }

}
