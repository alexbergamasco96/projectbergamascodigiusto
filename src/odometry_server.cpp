#include "ros/ros.h"
#include <cmath> 
#include "projectbergamascodigiusto/OdometryComputation.h"
//initial point x,y set by the dynamic reconfigure in the step before.Needed for checking and changing
int x_current_dynamic;
int y_current_dynamic;

double time_before;
double x_before;
double y_before;
double theta_before;

double x_comput;
double y_comput;
double theta_comput;

int count;

double v_k; //velocity
double t_s; 
double w_k;
double alpha;
double vx;
double vy;

//Features of the car
const double l=1.30; 
const double steering_factor = 18;
const double d = 1.765;


//check (and change) if dynamic reconfigure has changed the initial point (x,y)
void xy_is_changed(int x_changed,int y_changed){
    if(count!=0 && (x_changed!=x_current_dynamic || y_changed!=y_current_dynamic)) {
        x_before=(double)x_changed;
        y_before=(double)y_changed;
        theta_before=0;
    }
}

//Service that computes odometry
bool odometryComputation(projectbergamascodigiusto::OdometryComputation::Request &req,projectbergamascodigiusto::OdometryComputation::Response &res){
    ROS_INFO("[SERVER]  Request: vLeft=%f,vRight=%f,steerSensor=%f",(double)req.speedL,(double)req.speedR,(double)req.steer_sensor);
    ROS_INFO("[TIME SERVER] time=%f",(double)req.seconds);
    xy_is_changed(req.x_init,req.y_init);
    
     //EULER
    switch(req.algorithm){
        case (1):   //Differential drive
            v_k=0.5*((double)req.speedL+(double)req.speedR); //velocity
            t_s=(double)req.seconds-time_before; //tempo campionamento time
            w_k=((double)req.speedR-(double)req.speedL)/l;

            vx=v_k*cos(theta_before);
            vy=v_k*sin(theta_before);
            
            //Euler

            /*
            x_comput=x_before+t_s*vx;
            y_comput=y_before+t_s*vy;
            theta_comput=theta_before+w_k*t_s;
            */

            //Runge-kutta
            x_comput = x_before + v_k*t_s*cos(theta_before + (w_k*t_s)*0.5);
            y_comput = y_before + v_k*t_s*sin(theta_before + (w_k*t_s)*0.5);
            theta_comput = theta_before + w_k*t_s;

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
            
            break;

        case (2):     //Ackerman
            t_s=(double)req.seconds-time_before;
            v_k = 0.5*((double)req.speedL + (double)req.speedR);
            alpha = (double)req.steer_sensor / steering_factor;
            

            vx = v_k*cos(alpha);
            vy = v_k*sin(alpha);

            alpha = (alpha*3.14)/180;
            w_k = (v_k * tan(alpha)) / (double)d;

            //Euler
            /*
            x_comput=x_before+t_s*vx;
            y_comput=y_before+t_s*vy;
            theta_comput=theta_before+w_k*t_s;
            */

            //Runge-Kutta
            x_comput = x_before + v_k*t_s*cos(theta_before + (w_k*t_s)*0.5);
            y_comput = y_before + v_k*t_s*sin(theta_before + (w_k*t_s)*0.5);
            theta_comput = theta_before + w_k*t_s;

            ROS_INFO("[SERVER]  position: (x,y)=(%f,%f) theta=%f ",x_comput,y_comput,theta_comput);
            ROS_INFO("[SERVER] INFO: Ts=%f, w=%f, v=%f, alpha=%f", t_s,w_k,v_k, alpha);

            //Response

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

            break;
        
    }

    x_current_dynamic=req.x_init;
    y_current_dynamic=req.y_init;
    count++; //for the function xy_is_changed(int,int)

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