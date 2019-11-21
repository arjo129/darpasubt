import rospy
import serial
from std_msgs.msg import UInt8

devicePort = serial.Serial('/dev/ttyUSB0', 9600, timeout=0.1)

def publisher():
  pub = rospy.Publisher('e_stop', UInt8, 1)
  rospy.init_node('e_stop_republish', anonymous=True)
  rate = rospy.Rate(20)

  while not rospy.is_shutdown():
    value = devicePort.readline().strip(b'\r\n').decode()
    if (value == '1'):
      pub.publish(1)
    if (value == '0'):
      pub.publish(0)

    rate.sleep()

if __name__ == '__main__':
  try:
    publisher()
  except rospy.ROSInterruptException:
    devicePort.close()
    pass

