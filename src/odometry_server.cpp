#include "ros/ros.h"
#include <cmath> 
#include "projectbergamascodigiusto/OdometryComputation.h"
double time_before;

double x_before;
double y_before;
double theta_before;

double x_comput;
double y_comput;
double theta_comput;

int count;


bool odometryComputation(projectbergamascodigiusto::OdometryComputation::Request &req,projectbergamascodigiusto::OdometryComputation::Response &res){
    ROS_INFO("[SERVER]  Request: vRight=%f,vLeft=%f,steerSensor=%f",(double)req.speedR,(double)req.speedL,(double)req.steer_sensor);
    ROS_INFO("[TIME SERVER] time=%f",(double)req.seconds);

    double l=0.130;
    if(count==0){
        x_before=0;
        y_before=0;    
        theta_before=0;
    }

    double v_k=0.5*((double)req.speedL+(double)req.speedR); //velocity
    double t_s=(double)req.seconds-time_before; //tempo campionamento 
    double w_k=((double)req.speedL+(double)req.speedR)/l;
    
    //Euler
    x_comput=x_before+v_k*t_s*cos(theta_before);
    y_comput=y_before+v_k*t_s*sin(theta_before);
    theta_comput=theta_before+w_k*t_s;

    ROS_INFO("[SERVER]  position: (x,y)=(%f,%f) theta=%f",x_comput,y_comput,theta_comput);

    x_before=x_comput;
    y_before=x_comput;
    theta_before=theta_comput;
    count++;

    //send as response


    return true;
}

int main (int argc, char **argv){
    count=0;
    ros::init(argc,argv,"odometry_server");
    ros::NodeHandle n;

    ros::ServiceServer service=n.advertiseService("compute_odometry",odometryComputation);//name of channel
    ROS_INFO("[SERVER]  Ready to compute odometry with dynamic reconfigure");
    ros::spin();
    return 0;

}