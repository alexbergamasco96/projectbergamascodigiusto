#include "ros/ros.h"
#include "std_msgs/String.h"
#include "projectbergamascodigiusto/floatStamped.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <message_filters/sync_policies/approximate_time.h>

void callback(const projectbergamascodigiusto::floatStamped::ConstPtr& left, const projectbergamascodigiusto::floatStamped::ConstPtr& right, const projectbergamascodigiusto::floatStamped::ConstPtr& steer){
	
		ROS_INFO ("Received a messages: (%f,%f,%f)", left->data  , right->data, steer->data);
	}

int main (int argc , char ** argv){
    ros::init(argc,argv,"msgs_filters");
    ros::NodeHandle n;

    message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subLeft(n, "speedL_stamped", 1);
		message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subRight(n, "speedR_stamped", 1);
		message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subSteer(n, "steer_stamped", 1);
		typedef message_filters::sync_policies::ApproximateTime<projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped> MySyncPolicy;

  		message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), subLeft, subRight,subSteer);
        sync.registerCallback(boost::bind(&callback,_1, _2,_3));
        ros::spin();
        return 0;

}