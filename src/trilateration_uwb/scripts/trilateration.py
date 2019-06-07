#!/usr/bin/env python
import rospy
import numpy as np
import tf
from visualization_msgs.msg import Marker, MarkerArray
from generic_msgs.msg import UWB

class BeaconTriangulator:
    """
    Visuallize the UWB beacons in 3D Space.
    """
    def __init__(self):
        self.uwb_positions = [[0,0,0], [7.31,0,0], [3.46, 7.06, 0]] #Position of the beacons
        self.pub = rospy.Publisher('/uwb_least_squares', MarkerArray, queue_size=1)
        self.sub = rospy.Subscriber('/uwb_tlm', UWB, self.tlm_cb)
        self.uwb_distance = [1,1.4,0]
        self.markers = []

    def tlm_cb(self,msg):
        #position = (msg.a,msg.b,msg.c)
        self.uwb_distance = [msg.a, msg.b, msg.c]


    def draw_sphere(self, alpha=1.0, position=(0,0,0), color=(0,0,0), scale=1, label="base_link"):
        """
        Draws a sphere at a given location
        """
        br = tf.TransformBroadcaster()
        br.sendTransform(position,
                     tf.transformations.quaternion_from_euler(0, 0, 0),
                     rospy.Time.now(),
                     label,
                     "world")

        marker = Marker()
        marker.header.frame_id = label
        marker.header.stamp = rospy.Time.now()
        marker.ns = "solution"
        marker.id = len(self.markers)
        marker.type = Marker.SPHERE
        marker.action = Marker.ADD
        marker.pose.position.x = 0
        marker.pose.position.y = 0
        marker.pose.position.z = 0
        marker.pose.orientation.x = 0.0
        marker.pose.orientation.y = 0.0
        marker.pose.orientation.z = 0.0
        marker.pose.orientation.w = 1.0

        marker.scale.x = 2*scale
        marker.scale.y = 2*scale
        marker.scale.z = 2*scale

        marker.color.a = alpha
        marker.color.r = color[0]
        marker.color.g = color[1]
        marker.color.b = color[2]
        self.markers.append(marker)

    def draw_buffer(self):
        markers = MarkerArray()
        markers.markers = self.markers
        self.pub.publish(markers)
        self.markers = []

    def draw_beacon_radius(self):
        """
        Helps visualize the individual beacons
        """
        for i in range(len(self.uwb_positions)):
            self.draw_sphere(alpha=0.1, position=self.uwb_positions[i], color=(255,255,255), scale=self.uwb_distance[i], label="uwb_node"+str(i))
    
    def least_squares_solution(self):
        """
        Baseline algo - least square solution of intersections
        TODO: Improve
        """
        arr = []
        dist_norm = []
        for i in range(len(self.uwb_positions)):
            res = np.array([-0.5,-0.5,-0.5]+self.uwb_positions[i])
            arr.append(res*-2)
            dist_norm.append([self.uwb_distance[i]**2 - np.linalg.norm(self.uwb_positions[i])**2])
        A = np.vstack(arr)
        B = np.array(dist_norm)
        res = np.linalg.lstsq(A,B)
        position =  (res[0][3][0], res[0][4][0], res[0][5][0]) 
        self.draw_sphere(alpha=1, position=position, color=(255,0,0), scale=0.1, label="base_link")

if __name__  == "__main__":
    rospy.init_node("BeaconTriangulator")
    bc = BeaconTriangulator()
    while True:
        bc.draw_beacon_radius()
        bc.least_squares_solution()
        bc.draw_buffer()
        rospy.sleep(0.1)
