#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <math.h>
#include <drone_pkg/takeoff_serv.h>

geometry_msgs::PoseStamped pose; 
drone_pkg::takeoff_serv srv; 

int main(int argc, char** argv){
    ros::init(argc, argv, "circle_node");
    ros::NodeHandle nh; 

    ros::ServiceClient client=nh.serviceClient<drone_pkg::takeoff_serv>("/location"); 

    ros::Rate looprate(10); 

    while(ros::ok()){
        for(float i=0; i<=6.28; i=i+0.01){
            pose.pose.position.x=cos(i)*5;
            pose.pose.position.y=sin(i)*5;
            pose.pose.position.z=5.0; 

            srv.request.poseStamped=pose; 

            client.call(srv); 

            ros::Duration(0.01).sleep(); 
        }

        ros::spinOnce();
        looprate.sleep(); 
    }
}