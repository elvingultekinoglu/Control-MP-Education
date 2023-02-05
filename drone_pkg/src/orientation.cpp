#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h> 
#include <tf2/LinearMath/Quaternion.h>
#include <drone_pkg/takeoff_serv.h> 

drone_pkg::takeoff_serv servis; 
geometry_msgs::PoseStamped pose; 
tf2::Quaternion quat; 

int main(int argc, char **argv){
    ros::init(argc, argv, "orientation_node");
    ros::NodeHandle nh; 

    ros::ServiceClient client=nh.serviceClient<drone_pkg::takeoff_serv>("/location"); 

    pose.pose.position.z=3.0;

    double yaw=180;
    double roll=0;
    double pitch=0; 

    quat.setRPY(roll,pitch,(3.14/180)*yaw); 
    quat=quat.normalize(); 

    pose.pose.orientation.x=quat.x(); 
    pose.pose.orientation.y=quat.y();
    pose.pose.orientation.z=quat.z();
    pose.pose.orientation.w=quat.w(); 

    servis.request.poseStamped=pose;

    client.call(servis); 
}