#include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <projectbergamascodigiusto/dynamicConfig.h>

void callback(projectbergamascodigiusto::dynamicConfig &config, uint32_t level) {
  ROS_INFO("Reconfigure Request.    Initial point (%d,%d)   Odometry type: %d", 
            config.x_initial, config.y_initial, 
            config.odometry_type 
            );
            
            ROS_INFO ("%d",level);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "dynamic_rcfg");

  dynamic_reconfigure::Server<projectbergamascodigiusto::dynamicConfig> server;
  dynamic_reconfigure::Server<projectbergamascodigiusto::dynamicConfig>::CallbackType f;

  f = boost::bind(&callback, _1, _2);
  server.setCallback(f);

  ROS_INFO("Spinning node");
  ros::spin();
  return 0;
}
