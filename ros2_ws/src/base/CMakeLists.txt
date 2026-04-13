cmake_minimum_required(VERSION 3.5)
project(base)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Message generation
find_package(rosidl_default_generators REQUIRED)

# Ensure message files are present in the correct path
set(msg_files
  "msg/Wheels.msg"
  "msg/Angle.msg"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  DEPENDENCIES std_msgs
)

include_directories(
  include
  ${rclcpp_INCLUDE_DIRS}
  ${std_msgs_INCLUDE_DIRS}
  ${geometry_msgs_INCLUDE_DIRS}
  ${nav_msgs_INCLUDE_DIRS}
  ${tf2_ros_INCLUDE_DIRS}
  ${sensor_msgs_INCLUDE_DIRS}
  ${Eigen3_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${rosidl_default_generators_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/rosidl_generator_cpp/${PROJECT_NAME}
)


ament_export_dependencies(rosidl_default_runtime)
ament_export_include_directories(include)
ament_export_include_directories(${CMAKE_BINARY_DIR}/rosidl_generator_cpp)


add_library(articulated_drive
  src/drives/articulated_drive.cpp
)

add_library(basePublisher
  src/base_node/base_publisher.cpp
)

add_executable(${PROJECT_NAME}_node src/${PROJECT_NAME}_node/${PROJECT_NAME}_node.cpp)
add_executable(angle2tf src/${PROJECT_NAME}_node/angle2tf.cpp)


target_link_libraries(differential_drive
  rclcpp::rclcpp
  tf2_ros::tf2_ros
  )

target_link_libraries(articulated_drive
  rclcpp::rclcpp
  tf2_ros::tf2_ros
  differential_drive
  )

target_link_libraries(basePublisher
  rclcpp::rclcpp
  tf2_ros::tf2_ros
  differential_drive
  articulated_drive
  )

target_link_libraries(${PROJECT_NAME}_node
  rclcpp::rclcpp
  tf2_ros::tf2_ros
  differential_drive
  articulated_drive
  basePublisher
  )

target_link_libraries(angle2tf
  rclcpp::rclcpp
  tf2_ros::tf2_ros
  differential_drive
  articulated_drive
  basePublisher
)

ament_target_dependencies(${PROJECT_NAME}_node rclcpp tf2_ros std_msgs geometry_msgs nav_msgs sensor_msgs Eigen3)
ament_target_dependencies(angle2tf rclcpp tf2_ros std_msgs geometry_msgs nav_msgs sensor_msgs Eigen3)


rosidl_target_interfaces(differential_drive
  ${PROJECT_NAME} "rosidl_typesupport_cpp")
rosidl_target_interfaces(articulated_drive
  ${PROJECT_NAME} "rosidl_typesupport_cpp")
rosidl_target_interfaces(basePublisher
  ${PROJECT_NAME} "rosidl_typesupport_cpp")
rosidl_target_interfaces(base_node
  ${PROJECT_NAME} "rosidl_typesupport_cpp")

install(TARGETS
  differential_drive
  articulated_drive
  basePublisher
  ${PROJECT_NAME}_node
  angle2tf
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

install(FILES
  mapping_rule_Angle.yaml
  mapping_rule_Wheels.yaml
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
