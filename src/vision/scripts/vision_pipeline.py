import numpy as np
import cv2
import signal
import rospy
import time
from sensor_msgs.msg import Image, CompressedImage
from utils.logger import Logger
from utils.ros_utils import get_config, get_config_files, image_ros_to_cv2, cv2_to_compressed_ros, cv2_to_ros
from utils.constant import Sensor
from utils.debug_tools import get_channels
from utils.conversions import to_gray

from detectors.cnnDetector import CNNDetector

MAIN_CAM_FREQ = 1

class VisionPipeline ():
    def __init__ (self):
        signal.signal(signal.SIGINT, self.handle_SIGINT)
        # Topics
        self.topic = "/kinect2/hd/image_color"

        # Data
        self.data = {}

        # Subscribers
        self.init_subscribers()

        # Publishers
        self.publishers = self.init_publishers()

        # Processes - if we need more might need to consider threading
        self.process("main")

        # CNN detector
        self.cnn = CNNDetector()


    def init_publishers(self):
        pubs = {}

        pubs['main'] = rospy.Publisher('vision/main', Image, queue_size=10)

        return pubs

    
    def init_subscribers(self):
        rospy.Subscriber(self.topic, Image, self.image_cb, callback_args="main", queue_size=1)


    def image_cb(self, rosimg, name):
        cv_img = image_ros_to_cv2(rosimg)
        self.data[name] = cv_img

    def handle_SIGINT(self, signal, frame):
        rospy.signal_shutdown("Terminated by user")


    def process_cvimg (self, cvimg, name):
        # Dump all preprocessors here
        self.publishers['main'].publish(cv2_to_ros(cvimg))


    def process(self, name):
        rospy.loginfo('Started!')

        while True:
            if name in self.data:
                image = self.data[name]
                boxes = self.cnn.detect(image)

                print (boxes)

                self.process_cvimg(image, name)
                del self.data[name]
            else:
                time.sleep(MAIN_CAM_FREQ)


if __name__ == '__main__':
    rospy.init_node('vision_pipeline')
    rospy.loginfo('Starting vision pipeline')
    VisionPipeline()
    rospy.spin()

