#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Header.h"
#include "projectbergamascodigiusto/floatStamped.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <message_filters/sync_policies/approximate_time.h>
#include "projectbergamascodigiusto/OdometryComputation.h"



class pub_sub{

	public:
	pub_sub(){

		subLeft.subscribe(n, "speedL_stamped", 1);
		subRight.subscribe(n, "speedR_stamped", 1);
		subSteer.subscribe(n, "steer_stamped", 1);
		sync.reset(new Sync(MySyncPolicy(10), subLeft, subRight,subSteer));
		sync->registerCallback(boost::bind(&pub_sub::callback, this, _1, _2, _3));	
		
	}
	void callback(const projectbergamascodigiusto::floatStampedConstPtr& left, const projectbergamascodigiusto::floatStampedConstPtr& right, const projectbergamascodigiusto::floatStampedConstPtr& steer){
		ROS_INFO("[MESSAGE_FILTERS]DATI: (%f, %f , %f)", left->data, right->data, steer->data);

		client=n.serviceClient<projectbergamascodigiusto::OdometryComputation>("compute_odometry");//name of channel,topic?
		srv.request.speedL=left->data;
		srv.request.speedR=right->data;
		srv.request.steer_sensor=steer->data;

		if(client.call(srv)){
        //ROS_INFO("Sum: %ld",(long int) srv.response.sum);
		ROS_INFO("[CLIENT] Server called");

   		 }

 		else{
        ROS_ERROR("[CLIENT] Fault on calling Server");
    	}
	}


	private:
	ros::NodeHandle n; 
	message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subLeft;
	message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subRight;
	message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subSteer;
	ros::ServiceClient client;
	projectbergamascodigiusto::OdometryComputation srv;


	typedef message_filters::sync_policies::ApproximateTime<projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped> MySyncPolicy;
	typedef message_filters::Synchronizer<MySyncPolicy> Sync;
	boost::shared_ptr<Sync> sync;

};




//Classic main of pub_sub in the same node

int main(int argc, char **argv){

 	ros::init(argc, argv, "subscribe_and_publish");
	 
 	pub_sub my_pub_sub;
 	ros::spin();
 	return 0;

}