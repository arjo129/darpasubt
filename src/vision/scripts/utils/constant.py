#!/usr/bin/env python
import cv2
import numpy as np
from enum import Enum #Install enum34
import rospy

class Color(Enum):
    '''BGR COLOR CODES'''
    black = [0, [0, 0, 0]]
    red = [1, [0, 0, 255]]
    blue = [2, [255, 128, 0]]
    yellow = [3, [0, 255, 255]]
    green = [4, [0, 255, 0]]
    orange = [5, [0, 100, 255]]
    purple = [6, [204, 0, 204]]
    white = [7, [255, 255, 255]]
    cyan = [8, [255, 255, 0]]

    def index(self):
        return self.value[0]

    def bgr(self):
        return self.value[1]

    @staticmethod
    def get_color(index):
        for x in Color:
            if x.index() == index:
                return x
        return None

class Sensor(Enum):
    # AUV
    bottom = 0
    front = 1
    sonar = 2

    # ASV
    left = 3
    center = 4
    right = 5
    lidar = 6

    @staticmethod
    def get_auv_sensor_names():
        return ['bottom', 'front', 'sonar']

    @staticmethod
    def get_asv_sensor_names():
        return ['left', 'center', 'right', 'lidar']

    @staticmethod
    def get_enum(name):
        for x in Sensor:
            if x.name == name:
                return x
        return None

class Coords(Enum):
    xy = 1
    xyz = 2
    rt = 3

class Object(Enum):
    buoy = 0
    bin = 1
    dice = 2
    torpedo = 3
    torpedo_arm = 4

    @staticmethod
    def get_enum(name):
        for x in Object:
            if x.name == name:
                return x
        return None
