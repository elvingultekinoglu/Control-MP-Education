#include <ros/ros.h> 
#include <turtlesim/TeleportAbsolute.h> 

int main(int argc, char ** argv){

    ros::init(argc, argv, "ilk_servis_client_denemesi");
    ros::NodeHandle nh; 

    ros::ServiceClient client=nh.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute"); 

    turtlesim::TeleportAbsolute tel; 

    tel.request.x=3.0; 
    tel.request.y=4.0; 

    if(client.call(tel)){
        ROS_INFO("Servis duzgun calisiyor."); 
    } else {
        ROS_INFO("Servis duzgun calismiyor."); 
    }
}