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
    ROS_INFO("[SERVER]  Request: vLeft=%f,vRight=%f,steerSensor=%f",(double)req.speedL,(double)req.speedR,(double)req.steer_sensor);
    ROS_INFO("[TIME SERVER] time=%f",(double)req.seconds);

    //INIT

    double l=0.130;
    if((bool)req.changed){
        x_before=(double)req.x_init;
        y_before=(double)req.y_init;
        theta_before=0;
    }




     //EULER
    if(req.algorithm==1){
    double v_k=0.5*((double)req.speedL+(double)req.speedR); //velocity
    double t_s=(double)req.seconds-time_before; //tempo campionamento 
    double w_k=((double)req.speedL-(double)req.speedR)/l;

    double vx=v_k*cos(theta_before);
    double vy=v_k*sin(theta_before);
    
    //Euler
    x_comput=x_before+t_s*vx;
    y_comput=y_before+t_s*vy;
    theta_comput=theta_before+w_k*t_s;

    ROS_INFO("[SERVER]  position: (x,y)=(%f,%f) theta=%f ",x_comput,y_comput,theta_comput);
    ROS_INFO("[SERVER] INFO: Ts=%f, w=%f, v=%f", t_s,w_k,v_k);

    //response
    res.x=x_comput;
    res.y=y_comput;
    res.steer_comput=theta_comput;
    res.vx=vx;
    res.vy=vy;
    res.vt=w_k;

    x_before=x_comput;
    y_before=x_comput;
    time_before=(double)req.seconds;
    theta_before=theta_comput;
    }
    //count++;
    if(req.algorithm==2){
        //ACKERMAN





    }

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