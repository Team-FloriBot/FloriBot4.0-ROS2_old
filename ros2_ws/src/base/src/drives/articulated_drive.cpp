#include "drives/articulated_drive.h"
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/time.hpp>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/pose2_d.hpp>
#include <base/msg/wheels.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <stdexcept>

// Konstruktoren
// ----------------------
ArticulatedDrive::ArticulatedDrive()
{
    clock_ = std::make_shared<rclcpp::Clock>(RCL_SYSTEM_TIME);
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(clock_);
    tf_listener_= std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
}
ArticulatedDrive::ArticulatedDrive(double axesLength, double wheelDiameter)
{
    setParam(axesLength, wheelDiameter);
    clock_ = std::make_shared<rclcpp::Clock>(RCL_SYSTEM_TIME);
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(clock_);
    tf_listener_= std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
}


ArticulatedDrive::~ArticulatedDrive() {}

// Inverse Kinematic
// ----------------------
articulatedWheelSpeed ArticulatedDrive::inverseKinematics(geometry_msgs::msg::Twist cmdVelMsg)
{
    articulatedWheelSpeed retVal;

    // calculate inverse kinematic by hand
    targetSpeed_ = cmdVelMsg.linear.x;
    targetOmega_ = cmdVelMsg.angular.z;
    angle_ = getJointAngle();

    if (targetSpeed_ >= 0)
    {
        retVal.Front.leftWheel = 1/wheelRadius_ * targetSpeed_ - (wheelDiameter_/(2*wheelRadius_))* targetOmega_;
        retVal.Front.rightWheel = 1/wheelRadius_ * targetSpeed_ + (wheelDiameter_/(2*wheelRadius_))* targetOmega_;
        retVal.Rear.leftWheel = (cos(angle_)/wheelRadius_ - (wheelDiameter_ * sin(angle))/(axesLength_ * wheelRadius_)) * targetSpeed_ + ((axesLength_*sin(angle_) / (2*wheelRadius_))+ (wheelDiameter * cos(angle_))/(2*wheelRadius_)) * targetOmega_;
        retVal.Rear.rightWheel= (cos(angle_)/wheelRadius_ + (wheelDiameter_ * sin(angle))/(axesLength_ * wheelRadius_)) * targetSpeed_ + ((axesLength_*sin(angle_) / (2*wheelRadius_))- (wheelDiameter * cos(angle_))/(2*wheelRadius_)) * targetOmega_;
    }

    else
    {
        retVal.Front.leftWheel = (cos(angle_)/wheelRadius_ + (wheelDiameter_ * sin(angle))/(axesLength_ * wheelRadius_)) * targetSpeed_ + ((axesLength_*sin(angle_) / (2*wheelRadius_))- (wheelDiameter * cos(angle_))/(2*wheelRadius_)) * targetOmega_;
        retVal.Front.rightWheel= (cos(angle_)/wheelRadius_ - (wheelDiameter_ * sin(angle))/(axesLength_ * wheelRadius_)) * targetSpeed_ + ((axesLength_*sin(angle_) / (2*wheelRadius_))+ (wheelDiameter * cos(angle_))/(2*wheelRadius_)) * targetOmega_;
        retVal.Rear.leftWheel = 1/wheelRadius_ * targetSpeed_ + (wheelDiameter_/(2*wheelRadius_))* targetOmega_;
        retVal.Rear.rightWheel = 1/wheelRadius_ * targetSpeed_ - (wheelDiameter_/(2*wheelRadius_))* targetOmega_;
    }


    return retVal;
}

// Forward Kinematic
// ----------------------
geometry_msgs::msg::Pose2D ArticulatedDrive::forwardKinematics(articulatedWheelSpeed WheelSpeed, rclcpp::Time Timestamp)
{
        double deltaTime = (Timestamp - TimeStamp_).seconds();
        TimeStamp_ = Timestamp;

        WheelSpeed_ = WheelSpeed.Front;
        Speed_.linear.x = (WheelSpeed_.leftWheel * wheelRadius_ + WheelSpeed_.rightWheel * wheelRadius_) / 2.0;
        Speed_.angular.z = (WheelSpeed_.rightWheel * wheelRadius_ - WheelSpeed_.leftWheel * wheelRadius_) / axesLength_;

        Pose_.x += Speed_.linear.x * deltaTime * cos(Pose_.theta + 0.5 * Speed_.angular.z * deltaTime);
        Pose_.y += Speed_.linear.x * deltaTime * sin(Pose_.theta + 0.5 * Speed_.angular.z * deltaTime);
        Pose_.theta += Speed_.angular.z * deltaTime;

        return Pose_;
}

// Parameter setzen
// ----------------------
void ArticulatedDrive::setParam(double AxesLength, double WheelDiameter)
{
        reset();
        axesLength_ = axesLength;
        wheelDiameter_ = wheelDiameter;
        wheelRadius_ = wheelDiameter_ / 2.0;
        wheelCircumference_ = 2.0 * M_PI * wheelDiameter_ / 2.0;
}

void ArticulatedDrive::reset()
{
    Pose_.theta = 0.0;
    Pose_.x = 0.0;
    Pose_.y = 0.0;

    WheelSpeed_.leftWheel = 0.0;
    WheelSpeed_.rightWheel = 0.0;

    rclcpp::Clock clock(RCL_SYSTEM_TIME);
    TimeStamp_ = clock.now();
}

// Aktuelle Position vom Vorderwagen zurückgeben
// ----------------------
geometry_msgs::msg::Pose2D ArticulatedDrive::getActualPose()
{
    return Pose_;
}

// Aktuelle Geschwindigkeit zurückgeben
// ----------------------
geometry_msgs::msg::Twist ArticulatedDrive::getSpeed()
{
    return Speed_;
}

// Winkel des Knickgelenks auslesen

double ArticulatedDrive::getJointAngle() 
{
    try {
        // Hier sicherstellen, dass tf_buffer_ valide ist
        auto transform = tf_buffer_->lookupTransform("jointFront", "jointRear", tf2::TimePointZero);
        tf2::Quaternion q(
            transform.transform.rotation.x,
            transform.transform.rotation.y,
            transform.transform.rotation.z,
            transform.transform.rotation.w);
        double roll, pitch, yaw;
        tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);
        return yaw;
    } catch (tf2::TransformException &ex) {
        // Nicht bei jedem Loop loggen, das frisst Performance
        return 0.0; 
    }
}
