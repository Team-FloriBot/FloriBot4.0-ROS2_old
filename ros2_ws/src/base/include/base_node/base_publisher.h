#ifndef KINEMATICS_PUBLISHER_H
#define KINEMATICS_PUBLISHER_H

#include "drives/articulated_drive.h"
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <base/msg/angle.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <base/msg/wheels.hpp>

// KinematicsPublisher Node
class KinematicsPublisher : public rclcpp::Node  
{
public:
    KinematicsPublisher(kinematics::coordinate Base);
    ~KinematicsPublisher();

private:
    void getParam();
    void createPublisherSubscriber();    
    void PublishSpeed();  
    void CmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg);  
    void SpeedCallback(const base::msg::Wheels::SharedPtr msg);  

    ArticulatedDrive Drive_;
    base::msg::Wheels Speedmsg_;
    
    rclcpp::TimerBase::SharedPtr CmdVelTimer_; 
    rclcpp::Publisher<base::msg::Wheels>::SharedPtr SpeedPublisher_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr OdometryPublisher_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr CmdVelSubscriber_;
    rclcpp::Subscription<base::msg::Wheels>::SharedPtr SpeedSubscriber_;

    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadaster_;
    double AxesLength_, WheelDiameter_;
    
};

#endif
