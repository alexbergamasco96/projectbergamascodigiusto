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
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <dynamic_reconfigure/server.h>
#include <projectbergamascodigiusto/dynamicConfig.h>
#include "projectbergamascodigiusto/computedOdom.h"



class pub_sub{

	public:
	pub_sub(){
		changed=false;

		subLeft.subscribe(n, "speedL_stamped", 1);
		subRight.subscribe(n, "speedR_stamped", 1);
		subSteer.subscribe(n, "steer_stamped", 1);
		pub = n.advertise<nav_msgs::Odometry>("odom",1);
		pub2 = n.advertise<projectbergamascodigiusto::computedOdom>("computed_odometry", 1);
		sync.reset(new Sync(MySyncPolicy(10), subLeft, subRight,subSteer));

		//Dynamic reconfigure
		f = boost::bind(&pub_sub::callback_dynamic_reconfigure,this, _1, _2);
		server_dynamic.setCallback(f);

		sync->registerCallback(boost::bind(&pub_sub::callback, this, _1, _2, _3));	
		

		

	}

	void callback_dynamic_reconfigure(projectbergamascodigiusto::dynamicConfig &config, uint32_t level) {
			x_init_set=config.x_initial;
			y_init_set=config.y_initial;
			odom_set=config.odometry_type;

  			ROS_INFO("[DYNAMIC RECONFIGURE]    Initial point (%d,%d)   Odometry type: %d", 
            x_init_set, y_init_set, 
            odom_set 
            );

			changed=true;


            //ROS_INFO ("[DYNAMIC RECONFIGURE]  Level:%d",level);
}


	void callback(const projectbergamascodigiusto::floatStampedConstPtr& left, const projectbergamascodigiusto::floatStampedConstPtr& right, const projectbergamascodigiusto::floatStampedConstPtr& steer){
		ROS_INFO("[MESSAGE_FILTERS]DATI: (%f, %f , %f)", left->data, right->data, steer->data);

		client=n.serviceClient<projectbergamascodigiusto::OdometryComputation>("compute_odometry");//name of channel,topic?
		srv.request.changed=changed;
		srv.request.x_init=x_init_set;
		srv.request.y_init=y_init_set;
		srv.request.algorithm=odom_set;

		//srv.request.seconds=ros::Time::now().toSec();
		srv.request.seconds=(left->header.stamp.toSec() + right->header.stamp.toSec())/2;
		srv.request.speedL=left->data;
		srv.request.speedR=right->data;
		srv.request.steer_sensor=steer->data;

		if(client.call(srv)){
        	//ROS_INFO("Sum: %ld",(long int) srv.response.sum);
			ROS_INFO("[CLIENT] Server called");
			ROS_INFO("[CLIENT] Send to tf the msg");
			odom.header.stamp = ros::Time::now();
			odom.header.frame_id = "odom";
			odom.pose.pose.position.x = srv.response.x;
			odom.pose.pose.position.y = srv.response.y;
			odom.pose.pose.position.z = 0.0;
			geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(srv.response.steer_comput);
			odom.pose.pose.orientation = odom_quat;
			pub.publish(odom);
			computed_odom.odometry = odom;
			if(odom_set == 1){
				computed_odom.type = "Differential";
			}else{
				computed_odom.type = "Ackerman";
			}

			pub2.publish(computed_odom);

			changed=false; 

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
	ros::Publisher  pub;
	ros::Publisher pub2;
	nav_msgs::Odometry odom;
	projectbergamascodigiusto::computedOdom computed_odom;
	dynamic_reconfigure::Server<projectbergamascodigiusto::dynamicConfig> server_dynamic;
	dynamic_reconfigure::Server<projectbergamascodigiusto::dynamicConfig>::CallbackType f;
	int x_init_set;
	int y_init_set;
	int odom_set;
	
	bool changed; //if it is changed the dynamic reconfig



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