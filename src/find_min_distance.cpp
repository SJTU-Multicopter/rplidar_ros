#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "rplidar_ros/Laser.h"

rplidar_ros::Laser pos;

void scanCallback(const sensor_msgs::LaserScan laser)
{
  float min_distance;
  int angle;

  min_distance = 6.0;
  angle = 0;
  for(int i=0; i<laser.ranges.size(); i=i+1)
  {
    if (laser.ranges[i]>0.9 && laser.ranges[i]<laser.range_max)
      if(laser.ranges[i]<min_distance)
      {
        min_distance = laser.ranges[i];
        angle=i;
      }
    }
    pos.min_distance = min_distance*100;
    pos.angle = angle;
} 

int main(int argc, char **argv)
{
  ros::init(argc, argv, "find_min_distance");
  ros::NodeHandle n;
  ros::Subscriber scan_sub = n.subscribe("/scan_horizontal", 1, scanCallback);
  ros::Publisher pub = n.advertise<rplidar_ros::Laser>("/laser_send", 1000);
  ros::Rate loop_rate(10);

  while(ros::ok())
  {
    pub.publish(pos);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}