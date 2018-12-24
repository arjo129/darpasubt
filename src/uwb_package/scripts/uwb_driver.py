#!/usr/bin/env python

import serial
from time import sleep

from generic_msgs.msg import UWB
import rospy

with serial.Serial('/dev/tty.usbmodem0007600080851', 115200, timeout=1) as ser:
    counter = 1
    while True:
        line = ser.readline()   # read a '\n' terminated line
        print (str(counter) + ": " + line)
        result = [x.strip() for x in line.split(',')]
        print (result)
        counter += 1
        sleep(.1)
