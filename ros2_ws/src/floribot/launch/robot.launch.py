from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, GroupAction
from launch.conditions import IfCondition, UnlessCondition
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.launch_description_sources import PythonLaunchDescriptionSource
import os

def generate_launch_description():
    return LaunchDescription([
        # Include base Launch
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([os.path.join(
                get_package_share_directory('base'), 'launch'),
                '/base_node.launch.py'])
        ),
        """
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([os.path.join(
                get_package_share_directory('floribot'), 'launch'),
                '/own_laserscan_multi_merger.launch.py'])
        ),

        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(os.getenv('FLORIBOT_PACKAGE_PATH', ''), 'launch', 'own_scanner.launch.py')
            ),
            launch_arguments={
                'ParentFrame': 'axesFront',
                'LaserFrame': 'laserFront',
                'LaserTopic': 'scanFront',
                'TX': '0.327',
                'TY': '0',
                'TZ': '0',
                'RX': '1',
                'RY': '0',
                'RZ': '0',
                'RW': '0',
                'IPAddress': '192.168.0.52'
            }.items()
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(os.getenv('FLORIBOT_PACKAGE_PATH', ''), 'launch', 'own_scanner.launch.py')
            ),
            launch_arguments={
                'ParentFrame': 'axesRear',
                'LaserFrame': 'laserRear',
                'LaserTopic': 'scanRear',
                'TX': '-0.334',
                'TY': '0',
                'TZ': '0',
                'RX': '0',
                'RY': '-1',
                'RZ': '0',
                'RW': '0',
                'IPAddress': '192.168.0.51'
            }.items()
        ),
        """
        # Include plc_connection Launch
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([os.path.join(
                get_package_share_directory('plc_connection'), 'launch'),
                '/plc_connection_launch.py'])
        ),
    ])
