#include "ros/ros.h"
#include "std_msgs/String.h"
#include "projectbergamascodigiusto/floatStamped.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <message_filters/sync_policies/approximate_time.h>


class pub_sub{

	projectbergamascodigiusto::floatStamped left1;
	projectbergamascodigiusto::floatStamped right1;
	projectbergamascodigiusto::floatStamped steer1;

	private:
	ros::NodeHandle n; 
			
	public:
	pub_sub(){

		message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subLeft(n, "speedL_stamped", 1);
		message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subRight(n, "speedR_stamped", 1);
		message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subSteer(n, "steer_stamped", 1);
		typedef message_filters::sync_policies::ApproximateTime<projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped> MySyncPolicy;

  		message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), subLeft, subRight,subSteer);
	    sync.registerCallback(boost::bind(&pub_sub::callback,this, _1, _2,_3));

		//idea: if function. if(odom=diff){sync.reg ... with callback1} else ...callback2
		//callback1 compute differential drive, callback2 ackermann
		//sync.registerCallback(boost::bind(&pub_sub::callback, _1, _2, _3));
		
	}



	//Here compute the odometry.
	void callback(const projectbergamascodigiusto::floatStamped::ConstPtr& left, const projectbergamascodigiusto::floatStamped::ConstPtr& right, const projectbergamascodigiusto::floatStamped::ConstPtr& steer){
		left1=*left;
		right1=*right;
		steer1=*steer;
		ROS_INFO ("Received a messages: (%f,%f,%f)", left1.data  , right1.data, steer1.data);

	}

};




//Classic main of pub_sub in the same node

int main(int argc, char **argv){

 	ros::init(argc, argv, "subscribe_and_publish");
	 
 	pub_sub my_pub_sub;
 	ros::spin();
 	return 0;

}