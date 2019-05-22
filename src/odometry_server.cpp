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

double v_k; //velocity
double t_s; //tempo campionamento time
double w_k;
double vx;
double vy;



bool odometryComputation(projectbergamascodigiusto::OdometryComputation::Request &req,projectbergamascodigiusto::OdometryComputation::Response &res){
    ROS_INFO("[SERVER]  Request: vLeft=%f,vRight=%f,steerSensor=%f",(double)req.speedL,(double)req.speedR,(double)req.steer_sensor);
    ROS_INFO("[TIME SERVER] time=%f",(double)req.seconds);

    //INIT

    double l=1.30;
    double steering_factor = 18;
    double d = 1.765;

    if((bool)req.changed){
        x_before=(double)req.x_init;
        y_before=(double)req.y_init;
        theta_before=0;
    }




     //EULER
    switch(req.algorithm){
        case (1):   //Differential drive
            v_k=0.5*((double)req.speedL+(double)req.speedR); //velocity
            t_s=(double)req.seconds-time_before; //tempo campionamento time
            w_k=((double)req.speedL-(double)req.speedR)/l;

            vx=v_k*cos(theta_before);
            vy=v_k*sin(theta_before);
            
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
            y_before=y_comput;
            time_before=(double)req.seconds;
            theta_before=theta_comput;
            
            //count++;
            break;

        case (2):     //Ackerman
            /*t_s=(double)req.seconds-time_before;
            vel = 0.5*((double)req.speedL + (double)req.speedR);
            alpha = (double)req.steer_sensor / steering_factor;
            vx = vel*cos()*/

            break;
        
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