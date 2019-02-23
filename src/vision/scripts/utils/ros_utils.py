#!/usr/local/bin/python2.7
import os
import yaml
import numpy as np
import cv2

import rospy
import rospkg
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import CompressedImage


bridge = CvBridge()

#########################
#   Image Conversions   #
#########################

def compressed_ros_to_cv2(img):
    try:
        np_arr = np.fromstring(img.data, np.uint8)
        frame = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)
    except CvBridgeError as e:
        rospy.logerr(e)
    return frame 

def image_ros_to_cv2(img):
    try:
        frame = bridge.imgmsg_to_cv2(img, "bgr8")
    except CvBridgeError as e:
        rospy.logerr(e)
    return frame

def cv2_to_ros(img):
    try:
        frame = bridge.cv2_to_imgmsg(img, "bgr8")
    except CvBridgeError as e:
        rospy.logerr(e)
    return frame
    
def cv2_to_compressed_ros(img):
    try:
        msg = CompressedImage()
        msg.header.stamp = rospy.Time.now()
        msg.format = "jpeg"
        msg.data = np.array(cv2.imencode('.jpg', img)[1]).tostring()
        return msg
    except CvBridgeError as e:
        rospy.logerr(e)


#########################
#     Configuration     #
#########################

# Returns configuration file located in package
def get_config(name, package='vision', folder=None):
    rospack = rospkg.RosPack()
    if folder is not None:
        filepath = os.path.join(rospack.get_path(package), 'config', folder, name)
    else:
        filepath = os.path.join(rospack.get_path(package), 'config', name)
    with open(filepath, 'r') as stream:
        return yaml.load(stream)

# Returns list of configuration files
def get_config_files(package='vision', folder=None):
    rospack = rospkg.RosPack()
    if folder is not None:
        filepath = os.path.join(rospack.get_path(package), 'config', folder)
    else:
        filepath = os.path.join(rospack.get_path(package), 'config')
    return [name for name in os.listdir(filepath) if name.endswith('.yaml')]