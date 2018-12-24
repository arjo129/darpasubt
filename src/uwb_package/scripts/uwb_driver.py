#!/usr/bin/env python

import serial
from time import sleep

from generic_msgs.msg import UWB
import rospy

def serialReader(ser):
    pub = rospy.Publisher('uwb_tlm', UWB, queue_size=1)

    rospy.init_node('uwb_tlm_node', anonymous=True)
    rate = rospy.Rate(10) # 10hz

    while not rospy.is_shutdown():
        line = ser.readline()   # read a '\n' terminated line
        result = [x.strip() for x in line.split(',')]
        rospy.loginfo(line)

        if (len(result) >= 3):
            message = UWB()
            try:
                message.a = float(result[0])
                message.b = float(result[1])
                message.c = float(result[2])

                pub.publish(message)
            except:
                rospy.loginfo("Could not convert message to 3 floats")

        rate.sleep()

if __name__ == '__main__':
    with serial.Serial('/dev/ttyACM0', 115200, timeout=1) as ser:
        try:
            serialReader(ser)
        except rospy.ROSInterruptException:
            pass
