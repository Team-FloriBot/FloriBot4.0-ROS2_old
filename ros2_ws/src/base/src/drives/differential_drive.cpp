#include "drives/differential_drive.h"

namespace kinematics
{
    // Konstruktoren
    // ----------------------
    differentialDrive::differentialDrive(double axesLength, double wheelDiameter)
    {
        setParam(axesLength, wheelDiameter);
    }
    differentialDrive::differentialDrive()
    {
        reset();
    }
    differentialDrive::~differentialDrive()
    {
    }

    // Reset Data
    // ----------------------
    void differentialDrive::reset()
    {
        Pose_.theta = 0.0;
        Pose_.x = 0.0;
        Pose_.y = 0.0;

        WheelSpeed_.leftWheel = 0.0;
        WheelSpeed_.rightWheel = 0.0;

        rclcpp::Clock clock(RCL_SYSTEM_TIME);
        TimeStamp_ = clock.now();
    }

    // Parameter setzen
    // ----------------------
    void differentialDrive::setParam(double axesLength, double wheelDiameter)
    {
        reset();
        axesLength_ = axesLength;
        wheelDiameter_ = wheelDiameter;
        wheelRadius_ = wheelDiameter_ / 2.0;
        wheelCircumference_ = 2.0 * M_PI * wheelDiameter_ / 2.0;
    }

    // Forward Kinematic
    // ----------------------
    geometry_msgs::msg::Pose2D differentialDrive::forwardKinematics(DifferentialWheelSpeed WheelSpeed, rclcpp::Time Timestamp)
    {
     
        double deltaTime = (Timestamp - TimeStamp_).seconds();
        TimeStamp_ = Timestamp;

        WheelSpeed_ = WheelSpeed;
        Speed_.linear.x = (WheelSpeed_.leftWheel * wheelRadius_ + WheelSpeed_.rightWheel * wheelRadius_) / 2.0;
        Speed_.angular.z = (WheelSpeed_.rightWheel * wheelRadius_ - WheelSpeed_.leftWheel * wheelRadius_) / axesLength_;

        Pose_.x += Speed_.linear.x * deltaTime * cos(Pose_.theta + 0.5 * Speed_.angular.z * deltaTime);
        Pose_.y += Speed_.linear.x * deltaTime * sin(Pose_.theta + 0.5 * Speed_.angular.z * deltaTime);
        Pose_.theta += Speed_.angular.z * deltaTime;

        return Pose_;
    }
    
    // Inverse Kinematic
    // ----------------------
    kinematics::DifferentialWheelSpeed differentialDrive::inverseKinematics(geometry_msgs::msg::Twist cmdVelMsg)
    {
        kinematics::DifferentialWheelSpeed WheelSpeed;

	// Berechnung Wheelspeed aus cmdVelMsg
        WheelSpeed.rightWheel = cmdVelMsg.linear.x + cmdVelMsg.angular.z * axesLength_ / 2.0;
        WheelSpeed.leftWheel = WheelSpeed.rightWheel - cmdVelMsg.angular.z * axesLength_;

        WheelSpeed.rightWheel = WheelSpeed.rightWheel / wheelRadius_;
        WheelSpeed.leftWheel = WheelSpeed.leftWheel / wheelRadius_;

        return WheelSpeed;
    }

    geometry_msgs::msg::Pose2D differentialDrive::getActualPose()
    {
        return Pose_;
    }

    geometry_msgs::msg::Twist differentialDrive::getSpeed()
    {
        return Speed_;
    }
}

