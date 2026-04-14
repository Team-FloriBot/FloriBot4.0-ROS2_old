#ifndef ARTICULATED_DRIVE_H
#define ARTICULATED_DRIVE_H

#include <rclcpp/rclcpp.hpp>

#include <rclcpp/time.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/pose2_d.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>

struct DifferentialWheelSpeed
{
    double leftWheel;
    double rightWheel;
};

struct articulatedWheelSpeed
{
    DifferentialWheelSpeed Front, Rear;
};


class ArticulatedDrive
{
    public:
        ArticulatedDrive();
        ArticulatedDrive(double axesLength, double wheelDiameter);
        ~ArticulatedDrive();

        articulatedWheelSpeed inverseKinematics(geometry_msgs::msg::Twist cmdVelMsg);
        geometry_msgs::msg::Pose2D forwardKinematics(articulatedWheelSpeed WheelSpeed, rclcpp::Time Timestamp);
        geometry_msgs::msg::Pose2D getActualPose();
        geometry_msgs::msg::Twist getSpeed();
        void reset();
        void setParam(double AxesLength, double WheelDiameter);
        double getJointAngle();
        
    private:
        std::shared_ptr<rclcpp::Clock> clock_;
        std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
        std::shared_ptr<tf2_ros::TransformListener> tf_listener_{nullptr};
        geometry_msgs::msg::Pose2D Pose_;
        DifferentialWheelSpeed WheelSpeed_;
        geometry_msgs::msg::Twist Speed_;
        rclcpp::Time TimeStamp_;
        double axesLength_, wheelDiameter_, wheelCircumference_, wheelRadius_;
        double targetSpeed_, targetOmega_;
        double angle_;
        
};


#endif

