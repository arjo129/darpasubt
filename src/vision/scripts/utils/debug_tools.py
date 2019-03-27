#!/usr/bin/env python
import cv2
import numpy as np
from algorithms import get_salient_color, get_salient_hsv, get_salient_lab

def get_channels(img):
    """ Returns bgr, hsv, lab and saliency of bgr channels of image in order """
    return np.vstack((get_bgr_stack(img), get_hsv_stack(img), get_lab_stack(img), get_salient_color(img), get_salient_hsv(img), get_salient_lab(img)))


def get_bgr_stack(img):
    """ Returns horizontal stack of BGR channels """
    b, g, r = cv2.split(img)
    b = cv2.cvtColor(b, cv2.COLOR_GRAY2BGR)
    g = cv2.cvtColor(g, cv2.COLOR_GRAY2BGR)
    r = cv2.cvtColor(r, cv2.COLOR_GRAY2BGR)
    return np.hstack((b, g, r))


def get_hsv_stack(img):
    return get_bgr_stack(cv2.cvtColor(img, cv2.COLOR_BGR2HSV))


def get_luv_stack(img):
    return get_bgr_stack(cv2.cvtColor(img, cv2.COLOR_BGR2LUV))


def get_ycb_stack(img):
    return get_bgr_stack(cv2.cvtColor(img, cv2.COLOR_BGR2YCR_CB))


def get_lab_stack(img):
    return get_bgr_stack(cv2.cvtColor(img, cv2.COLOR_BGR2LAB))

