#include "ros/ros.h"
#include "projectbergamascodigiusto/OdometryComputation.h"

bool odometryComputation(projectbergamascodigiusto::OdometryComputation::Request &req,projectbergamascodigiusto::OdometryComputation::Response &res){
    ROS_INFO("request: vRight=%f,vLeft=%f,steerSensor=%f",(double)req.speedR,(double)req.speedL,(double)req.steer_sensor);
    return true;
}

int main (int argc, char **argv){
    ros::init(argc,argv,"odometry_server");
    ros::NodeHandle n;

    ros::ServiceServer service=n.advertiseService("compute_odometry",odometryComputation);//name of channel
    ROS_INFO("Ready to compute odometry with dynamic reconfigure");
    ros::spin();
    return 0;

}