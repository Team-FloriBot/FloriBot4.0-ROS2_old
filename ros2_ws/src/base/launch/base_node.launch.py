from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, LogInfo
from launch.substitutions import LaunchConfiguration, TextSubstitution
from launch_ros.actions import Node
from launch.conditions import IfCondition

def generate_launch_description():
    # Argumente definieren
    return LaunchDescription([
        DeclareLaunchArgument('frontLength', default_value='-0.38'),
        DeclareLaunchArgument('rearLength', default_value='-0.38'),
        DeclareLaunchArgument('wheelDiameter', default_value='0.280'),
        DeclareLaunchArgument('axesLength', default_value='0.335'),
        DeclareLaunchArgument('frontLaserLength', default_value='0.387'),
        DeclareLaunchArgument('rearLaserLength', default_value='-0.387'),
        
        # Knoten: base_node
        Node(
            package='base',
            executable='base_node',
            name='base',
            output='screen',
            parameters=[
                {'wheelDiameter': -0.38},
                {'axesLength': -0.38},
                {'use_sim_time': False}
            ]
        ),
        Node(
            package='base',
            executable='angle2tf',
            name='angle2tf',
            output='screen',
            parameters=[{'use_sim_time': False}]
        ),
        # tf2 static_transform_publisher Knoten
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='AxesFront2JointFront',
            output='screen',
            arguments=[LaunchConfiguration('frontLength'), '0', '0', '0', '0', '0', '1', 'axesFront', 'jointFront'],
            parameters=[{'use_sim_time': False}]
        ),
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='AxesRear2JointRear',
            output='screen',
            arguments=[LaunchConfiguration('rearLength'), '0', '0', '0', '0', '0', '1', 'jointRear', 'axesRear'],
            parameters=[{'use_sim_time': False}]
        ),
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='Front',
            output='screen',
            arguments=['0', '0', '0', '0', '0', '0', '1', 'base_link', 'axesFront'],
            parameters=[{'use_sim_time': False}]
        ),
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='AxesFront2LaserFront',
            output='screen',
            arguments=[LaunchConfiguration('frontLaserLength'), '0', '0', '0', '0', '0', '1', 'axesFront', 'laserFront'],
            parameters=[{'use_sim_time': False}]
        ),
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='AxesRear2LaserRear',
            output='screen',
            arguments=[LaunchConfiguration('rearLaserLength'), '0', '0', '0', '0', '0', '1', 'axesRear', 'laserRear'],
            parameters=[{'use_sim_time': False}]
        ),
    ])

