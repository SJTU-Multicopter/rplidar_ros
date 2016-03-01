#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include <math.h>

bool start_ok = false;
bool init1_ok = false;
bool init2_ok = false;
bool flag1 = false;
bool flag2 = false;
int count = 0;

sensor_msgs::LaserScan scan1;
sensor_msgs::LaserScan scan2;

void scan1Callback(const sensor_msgs::LaserScan scan)
{
        float sum = 0;
        float dist = 0;
        int num = 0;

        if(init1_ok ==false || init2_ok ==false){
                if(start_ok == false){
                        	count ++;
                        	if(count == 10){
                        	        start_ok = true;
                	}
                }else{
                	for(int i = 0; i<scan.ranges.size(); i++){
                	        if (scan.ranges[i]>scan.range_min && scan.ranges[i]<scan.range_max){
                	        	sum = sum + scan.ranges[i]*sin(i/180*M_PI);
                	        	num ++;
                	        }
                	}
                	dist = sum/num;
                	if(dist > 0.5 && dist < 1.0 && num >90 && num <180){
                	        flag1 = true;	
                	}
                }   	
                init1_ok = true;
        }else{
                scan1 = scan;
        }
} 
	
void scan2Callback(const sensor_msgs::LaserScan scan)
{
        float sum = 0;
        float dist = 0;
        int num = 0;

        if(init1_ok ==false || init2_ok ==false){
                if(start_ok == false){

                }else{
                	for(int i = 0; i<scan.ranges.size(); i++){
                	        if (scan.ranges[i]>scan.range_min && scan.ranges[i]<scan.range_max){
                	        	sum = sum + scan.ranges[i]*sin(i/180*M_PI);
                	        	num ++;
                	        }
                	}
                	dist = sum/num;
                	if(dist > 0.5 && dist < 1.0 && num >90 && num <180){
                	        flag2 = true;	
                	}
                }   
                init2_ok = true;	
        }else{
                scan2 = scan;
        }
} 

int main(int argc, char **argv)
{
        ros::init(argc, argv, "separate");
        ros::NodeHandle n;
        ros::Subscriber scan_sub1 = n.subscribe("/scan1", 1, scan1Callback);
        ros::Subscriber scan_sub2 = n.subscribe("/scan2", 1, scan2Callback);
        ros::Publisher scan_pub1 = n.advertise<sensor_msgs::LaserScan>("/scan_vertical", 1000);
        ros::Publisher scan_pub2 = n.advertise<sensor_msgs::LaserScan>("/scan_horizontal", 1000);
        ros::Rate loop_rate(10);

        while(ros::ok())
        {
                if(flag1==true && flag2 == false){
                	scan_pub1.publish(scan1);
                	scan_pub2.publish(scan2);
                }else if(flag1==false && flag2 == true){
                	scan_pub1.publish(scan2);
                	scan_pub2.publish(scan1);
                }else{
                	ROS_INFO("RPlidar initial fail");
                }

                ros::spinOnce();
                loop_rate.sleep();
        }
        return 0;
}