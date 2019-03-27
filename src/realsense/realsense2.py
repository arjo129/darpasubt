#!/usr/bin/env python
# First import the library
import pyrealsense2 as rs
# Import Numpy for easy array manipulation
import numpy as np
# Import OpenCV for easy image rendering
import cv2
#import RotateTrans as RT
import math
import time
#import iterativeclosestpt as icp
import random as rand
import rospy
from std_msgs.msg import String

def getAngle(R):
    return math.atan2(R[1][0] , R[0][0])


def getAvgDepth(aligned_depth_frame,Point ,depth_intrin , initPoint = None):

    depthval = aligned_depth_frame.get_distance(int(Point[0]), int(Point[1]))
    depth_pointCtr = rs.rs2_deproject_pixel_to_point(
        depth_intrin, [int(Point[0]), int(Point[1])], depthval)

    Centre = depth_pointCtr[2];

    sum = 0
    count = 0


    if(Centre == 0):
        return depth_pointCtr


    for i in range(-3 , 3):
        for j in range(-3 ,3):
            depthval = aligned_depth_frame.get_distance(int(Point[0]+i), int(Point[1]+j))
            depth_point = rs.rs2_deproject_pixel_to_point(
                depth_intrin, [int(Point[0]+i), int(Point[1]+j)], depthval)

            if (depth_point[2] == 0):
                depth_pointCtr[2] = 0
                return depth_pointCtr

            if(abs(Centre - depth_point[2]) < 0.1):
                sum+=depth_point[2]
                count += 1

    probabledist = sum/count
    if(initPoint is not None):
        if(abs(probabledist - initPoint[2]) > 0.5):
            probabledist =0

    depth_pointCtr[2] = probabledist;


    return depth_pointCtr


def iszero(A):
    if(A[2] <= 0.4 or A[0] == 0 or A[1] == 0):
        return True
    else:
        return False

def distance(Matrix , i , j):
    vector = Matrix[i] - Matrix[j]
    dist = math.sqrt(sum(x ** 2 for x in vector))

    return dist

def filterlousypts(Matrixinit , Matrixpts):
    removevec = [];

    for i in range(len(Matrixinit)):
        if (iszero(Matrixinit[i]) or iszero(Matrixpts[i])):
            removevec.append(i)


    Matrixinit = np.delete(Matrixinit, removevec, axis=0)
    Matrixpts = np.delete(Matrixpts, removevec, axis=0)
    #
    # removevec = []
    #
    # count = 0
    # for i in range(len(Matrixinit)):
    #     for j in range(len(Matrixinit)):
    #         if(abs(distance(Matrixinit , i , j) - distance(Matrixpts , i , j)) > 0.2):
    #             count += 1
    #     if(count > (len(Matrixinit)/2)):
    #         removevec.append(i)
    #
    # Matrixinit = np.delete(Matrixinit, removevec, axis=0)
    # Matrixpts = np.delete(Matrixpts, removevec, axis=0)

    return Matrixinit , Matrixpts

pub = rospy.Publisher('Degree', String, queue_size=10)
rospy.init_node('talker', anonymous=True)



# params for ShiTomasi corner detection
feature_params = dict(maxCorners=100,
                      qualityLevel=0.8,
                      minDistance=20,
                      blockSize=8)
# Parameters for lucas kanade optical flow
lk_params = dict(winSize=(15, 15),
                 maxLevel=2,
                 criteria=(cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))
# Create some random colors
color = np.random.randint(0, 255, (100, 3))

p0 = []
p1 = []

MatrixPts = [0]
MatrixInit = [0]

DEG = 0

first = True
# Create a pipeline
pipeline = rs.pipeline()

# Create a config and configure the pipeline to stream
#  different resolutions of color and depth streams
config = rs.config()
config.enable_stream(rs.stream.depth, 640, 360, rs.format.z16, 15)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 15)

# Start streaming
profile = pipeline.start(config)

# Getting the depth sensor's depth scale (see rs-align example for explanation)
depth_sensor = profile.get_device().first_depth_sensor()
depth_scale = depth_sensor.get_depth_scale()
print("Depth Scale is: ", depth_scale)


clipping_distance_in_meters = 3  # 1 meter
clipping_distance = clipping_distance_in_meters / depth_scale

# Create an align object
# rs.align allows us to perform alignment of depth frames to others frames
# The "align_to" is the stream type to which we plan to align depth frames.
align_to = rs.stream.color
align = rs.align(align_to)
time.sleep(2)
# Streaming loop
try:
    while True:
        # Get frameset of color and depth
        frames = pipeline.wait_for_frames()
        # frames.get_depth_frame() is a 640x360 depth image

        # Align the depth frame to color frame
        aligned_frames = align.process(frames)

        # Get aligned frames
        aligned_depth_frame = aligned_frames.get_depth_frame()  # aligned_depth_frame is a 640x480 depth image
        color_frame = aligned_frames.get_color_frame()

        # Validate that both frames are valid
        if not aligned_depth_frame or not color_frame:
            continue

        depth_image = np.asanyarray(aligned_depth_frame.get_data())
        color_image = np.asanyarray(color_frame.get_data())


        depth_intrin = aligned_depth_frame.profile.as_video_stream_profile().intrinsics

        # Remove background - Set pixels further than clipping_distance to grey
        grey_color = 153
        depth_image_3d = np.dstack(
            (depth_image, depth_image, depth_image))  # depth image is 1 channel, color is 3 channels
        bg_removed = np.where((depth_image_3d > clipping_distance) | (depth_image_3d <= 0), grey_color, color_image)
        # cv2.imshow("Depth", bg_removed);
        """
        depthval = aligned_depth_frame.get_distance(160,360)
        depth_point = rs.rs2_deproject_pixel_to_point(
            depth_intrin, [160,360], depthval)
        """

        # Render images
        depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)

        images = np.hstack((bg_removed, depth_colormap))

        mask = np.zeros_like(color_image)

        if (first):
            color_image = cv2.blur(color_image, (5, 5))
            old_gray = cv2.cvtColor(color_image, cv2.COLOR_BGR2GRAY)
            #canny = cv2.Canny(old_gray, 100, 200)
            old_gray = cv2.adaptiveThreshold(old_gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

            p0 = cv2.goodFeaturesToTrack(old_gray, mask=None, **feature_params)
            if p0 is not None:
                NUMofP = len(p0)
                Matrixinit1 = np.stack(p0, axis=1).ravel().reshape(NUMofP, 2)
                MatrixInit = np.zeros((NUMofP, 3))
                MatrixInit[:, :-1] = Matrixinit1

                for i in range(NUMofP):
                    depth_point = getAvgDepth(aligned_depth_frame,MatrixInit[i] ,depth_intrin )

                    MatrixInit[i][0] = depth_point[0]
                    MatrixInit[i][1] = depth_point[1]
                    MatrixInit[i][2] = depth_point[2]

                # MatrixInit = np.around(MatrixInit, decimals=2)
                first = False
            else:
                NUMofP = 0

            DEG = 0

        else:
            color_image = cv2.blur(color_image, (5, 5))
            frame_gray = cv2.cvtColor(color_image, cv2.COLOR_BGR2GRAY)
            #canny = cv2.Canny(frame_gray, 100, 200)
            frame_gray = cv2.adaptiveThreshold(frame_gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

            cv2.imshow("Grey", frame_gray)
            #cv2.imshow("canny", canny)
            # calculate optical flow
            p1, st, err = cv2.calcOpticalFlowPyrLK(old_gray, frame_gray, p0, None, **lk_params)

            # DO computation

            if p1 is not None:

                NUMofP = len(p0)
                Matrixpts1 = np.stack(p1, axis=1).ravel().reshape(NUMofP, 2)
                MatrixPts = np.zeros((NUMofP, 3))
                MatrixPts[:, :-1] = Matrixpts1



                for i in range(NUMofP):
                    depth_point = getAvgDepth(aligned_depth_frame, MatrixPts[i], depth_intrin , MatrixInit[i])

                    MatrixPts[i][0] = depth_point[0]
                    MatrixPts[i][1] = depth_point[1]
                    MatrixPts[i][2] = depth_point[2]

                MatrixPts = np.around(MatrixPts, decimals=2)
                MatrixInit = np.around(MatrixInit, decimals=2)
                MatrixInit , MatrixPts = filterlousypts(MatrixInit, MatrixPts)


                #plot3d(MatrixPts, MatrixInit)
                # print("The 2 arrays are as follows")
                # print(MatrixPts)
                # print(MatrixInit)
                ppoints = MatrixInit[0:3:2]
                cpoints = MatrixPts[0:3:2]



                try:
                    R, T = icp.ICP_matching(ppoints, cpoints)

                    degree = getAngle(R)
                    #degree = 0.0

                    # print(R)

                    # degree = RT.getkabschangle(ppoints, cpoints)
                    if (abs(R[0][0]) < 1):
                        degree = math.acos(R[0][0])
                    else:
                        degree = 0
                    hello_str = "Degree: %f" % degree
                    rospy.loginfo(hello_str)
                    pub.publish(hello_str)

                    #DEG += degree
                    #print("degree")
                    #print(degree)
                    #print(DEG)
                except(IndexError):
                    #print("Index erro")
                    idk =1


                #degree = RT.getallangles(MatrixInit, MatrixPts)
                # degree = RT.getAngleZ(MatrixInit, MatrixPts)
                # degree = 0
                #degree = round(degree, 2)
                # print("The degree")
                #print(degree)

                #if (abs(degree) < 0.02):
                #    degree = 0;
                #DEG += degree

                # print("Degree")
                # print(round(DEG ,1))
                #
                # Angle Computation

                # Select good points
                good_new = p1[st == 1]
                good_old = p0[st == 1]

                for i, (new, old) in enumerate(zip(good_new, good_old)):
                    a, b = new.ravel()
                    c, d = old.ravel()
                    mask = cv2.line(mask, (a, b), (c, d), color[i].tolist(), 2)
                    color_image = cv2.circle(color_image, (a, b), 1, color[i].tolist(), -1)

                old_gray = frame_gray.copy()
                p0 = cv2.goodFeaturesToTrack(old_gray, mask=None, **feature_params)

                if p0 is not None:
                    NUMofP = len(p0)
                    Matrixinit1 = np.stack(p0, axis=1).ravel().reshape(NUMofP, 2)
                    MatrixInit = np.zeros((NUMofP, 3))
                    MatrixInit[:, :-1] = Matrixinit1

                    for i in range(NUMofP):
                        depth_point = getAvgDepth(aligned_depth_frame, MatrixInit[i], depth_intrin)

                        MatrixInit[i][0] = depth_point[0]
                        MatrixInit[i][1] = depth_point[1]
                        MatrixInit[i][2] = depth_point[2]

                    # MatrixInit = np.around(MatrixInit, decimals=2)
                else:
                    NUMofP = 0



            else:
                NUMofP = 0



            img = cv2.add(color_image, mask)

            cv2.namedWindow('Align Example', cv2.WINDOW_AUTOSIZE)
            cv2.imshow('Align Example', img)

        # cv2.namedWindow('Align Example', cv2.WINDOW_AUTOSIZE)
        # cv2.imshow('Align Example', img)
        key = cv2.waitKey(1)
        # Press esc or 'q' to close the image window
        if key & 0xFF == ord('q') or key == 27:
            cv2.destroyAllWindows()
            break
finally:
    pipeline.stop()
