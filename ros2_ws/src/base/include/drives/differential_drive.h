#ifndef DIFFERENTIAL_DRIVE_H
#define DIFFERENTIAL_DRIVE_H

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp> 
#include <geometry_msgs/msg/pose2_d.hpp>  
#include <cmath> 

#include <tf2_ros/transform_listener.h>
#include <base/msg/wheels.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <stdexcept>

namespace kinematics
{
    struct DifferentialWheelSpeed
    {
        double leftWheel;
        double rightWheel;
    };

    class differentialDrive
    {
    public:
        differentialDrive(double axesLength, double wheelDiameter);
        differentialDrive();
        ~differentialDrive();

        DifferentialWheelSpeed inverseKinematics(geometry_msgs::msg::Twist cmdVelMsg);
        geometry_msgs::msg::Pose2D forwardKinematics(DifferentialWheelSpeed WheelSpeed, rclcpp::Time Timestamp);
        geometry_msgs::msg::Pose2D getActualPose();
        geometry_msgs::msg::Twist getSpeed();

        void reset();
        void setParam(double axesLength, double wheelDiameter);

    private:
        geometry_msgs::msg::Pose2D Pose_;
        DifferentialWheelSpeed WheelSpeed_;
        geometry_msgs::msg::Twist Speed_;
        rclcpp::Time TimeStamp_;
        double axesLength_, wheelDiameter_, wheelCircumference_, wheelRadius_;
    };
} 

#endif

