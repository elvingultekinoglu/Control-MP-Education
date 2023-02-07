#include <ros/ros.h> 
#include <geometry_msgs/PoseStamped.h>
#include <vector>
#include <drone_pkg/takeoff_serv.h>

geometry_msgs::PoseStamped pose; 
drone_pkg::takeoff_serv servis; 
 
ros::ServiceClient client;

void gotoposition(double x, double y, double z){
    servis.request.poseStamped.pose.position.x=x;
    servis.request.poseStamped.pose.position.y=y; 
    servis.request.poseStamped.pose.position.z=z; 

    client.call(servis);

}

void position(){
    struct desired_location{
        double x;
        double y;
        double z; 
    };

    std::vector<desired_location> positions;

    positions.reserve(20); 
    positions.push_back({5.0,0.0,5.0});
    positions.push_back({5.0,5.0,5.0}); 

    for(desired_location &des : positions){
        gotoposition(des.x,des.y,des.z);
        ros::Duration(10.0).sleep();
        ros::spinOnce(); 
    }
}

int main(int argc, char** argv){
    ros::init(argc,argv,"cube_node"); 
    ros::NodeHandle nh; 

    client=nh.serviceClient<drone_pkg::takeoff_serv>("/location"); 

    ros::Rate looprate(10); 

    position(); 

}

