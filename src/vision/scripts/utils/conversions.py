#!/usr/bin/env python
import rospy
import cv2
import numpy as np

def hue_to_bgr(hue):
    bgr = cv2.cvtColor(np.uint8([[[hue, 255, 255]]]), cv2.COLOR_HSV2BGR)
    return bgr[0][0].tolist()


def bgr_to_opp(img):
    """Converts BGR color space to Opponent color space"""
    b, g, r = cv2.split(img)
    o1 = np.abs((r - g) / math.sqrt(2))
    o2 = np.abs((r + g - 2 * b) / math.sqrt(6))
    o3 = np.abs((r + g + b) / math.sqrt(3))
    return o1, o2, o3


def bgr_to_hsi(img):
    o1, o2, o3 = bgr_to_opp(img)
    h = np.arctan2(o1, o2) * 180 / np.pi
    s = np.sqrt(o1**2 + o2**2)
    return h, s, o3


def to_bgr(img, flag):
    """ Converts 3 designated color space to BGR """
    if flag is 'gray':
        return cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
    if flag is 'hsv':
        return cv2.cvtColor(img, cv2.COLOR_HSV2BGR)
    if flag is 'lab':
        return cv2.cvtColor(img, cv2.COLOR_LAB2BGR)


def to_hsv(img):
    return cv2.cvtColor(img, cv2.COLOR_BGR2HSV)


def to_lab(img):
    return cv2.cvtColor(img, cv2.COLOR_BGR2LAB)

def to_gray(img):
    return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
