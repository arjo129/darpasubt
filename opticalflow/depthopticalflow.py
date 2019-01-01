from pykinect import nui
import numpy
import cv2 as cv
from datetime import datetime
dt = datetime.now()


kinect = nui.Runtime()
save_image = False


DEPTH_WINSIZE = 640, 480
VIDEO_WINSIZE = 640, 480
video_display = False



feature_params = dict(maxCorners=100,
                      qualityLevel=0.8,
                      minDistance=7,
                      blockSize=7)
# Parameters for lucas kanade optical flow
lk_params = dict(winSize=(15, 15),
                 maxLevel=2,
                 criteria=(cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 10, 0.03))
# Create some random colors
color = numpy.random.randint(0, 255, (100, 3))
# Take first frame and find corners in it

isfirst = True
old_gray = None
frame_gray = None
initialtime = 0
p1 = [0]
NUMofP = 0
MatrixPts = [0]
MatrixInit = [0]

_FLT_EPSILON = 1.192092896e-07
_NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS = 571.26

X = 0
Y = 0

p0 = [0]

def copyinto(video, x, y, temp):
    video.itemset((y, x, 0), temp[0])
    video.itemset((y, x, 1), temp[1])
    video.itemset((y, x, 2), temp[2])
    video.itemset((y, x, 3), 255)


def TransformSmoothParameters(vPoint):
    """returns depthX (float), depthY (float), depthValue (int)"""

    if vPoint.z > _FLT_EPSILON:
        # Center of depth sensor is at (0,0,0) in skeleton space, and
        # and (160,120) in depth image coordinates.  Note that positive Y
        # is up in skeleton space and down in image coordinates.
        #

        pfDepthX = 0.5 + vPoint.x * _NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS / (
                    vPoint.z * 640.0)
        pfDepthY = 0.5 - vPoint.y * _NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS / (
                    vPoint.z * 480.0)

        #
        #  Depth is in meters in skeleton space.
        #  The depth image pixel format has depth in millimeters shifted left by 3.
        #

        pusDepthValue = int(vPoint.z * 1000) << 3
        return pfDepthX, pfDepthY, pusDepthValue

    return 0.0, 0.0, 0


def getColorImage(frame):
    video = numpy.empty((480, 640, 4), numpy.uint8)
    frame.image.copy_bits(video.ctypes.data)

    global feature_params
    global lk_params

    mask = numpy.zeros_like(video)

    global NUMofP

    global isfirst
    global p0
    global p1
    global old_gray
    global frame_gray
    global MatrixPts
    global MatrixInit

    if (isfirst == True):
        old_gray = cv.cvtColor(video, cv.COLOR_BGR2GRAY)
        p0 = cv.goodFeaturesToTrack(old_gray, mask=None, **feature_params)
        NUMofP = len(p0)
        Matrixinit1 = numpy.stack(p0, axis=1).ravel().reshape(NUMofP, 2)
        MatrixInit = numpy.zeros((NUMofP,3))
        MatrixInit[:, :-1] = Matrixinit1
        #print(p0, len(p0))
        isfirst = False


    frame_gray = cv.cvtColor(video, cv.COLOR_BGR2GRAY)

    #print("p0 is this")
    #print(p0)

    p1, st, err = cv.calcOpticalFlowPyrLK(old_gray, frame_gray, p0, None, **lk_params)
    #print(p1)

    good_new = p1
    good_old = p0
    # draw the tracks
    #print(p0)
    #print(p1)

    for i, (new, old) in enumerate(zip(good_new, good_old)):
        a, b = numpy.stack(new, axis=1).ravel()
        c, d = numpy.stack(old, axis=1).ravel()
        mask = cv.line(mask, (a, b), (c, d), color[i].tolist(), 2)
        video = cv.circle(video, (a, b), 5, color[i].tolist(), -1)

    img = cv.add(video, mask)

    Matrixpts1 = numpy.stack(p1, axis=1).ravel().reshape(NUMofP, 2)
    MatrixPts = numpy.zeros((NUMofP,3))
    MatrixPts[:, :-1] = Matrixpts1

    #print("Matrixpts is as follows")
    #print(MatrixInit)
    #print(MatrixPts)



    #print(MatrixInit)
    #print(MatrixPts)

    cv.imshow('KINECT Video Stream', img)



def getDepthImage(frame):
    global p0
    global p1
    global old_gray
    global frame_gray
    global MatrixPts
    global MatrixInit
    global initialtime

    currenttime = dt.now().microsecond
    #print("YAY DEPTH")
    #print(MatrixPts)
    #print(MatrixInit)

    height,width = frame.image.height,frame.image.width

    depth = numpy.empty((height,width,1),numpy.uint8)
    arr2d = (depth >> 3) & 4095
    arr2d >>= 4

    frame.image.copy_bits(arr2d.ctypes.data)

    velocity = 0
    for i in range(NUMofP):

        X = int(MatrixPts[i][0])
        Y = int(MatrixPts[i][1])

        vector = nui.SkeletonEngine.depth_image_to_skeleton(X, Y, arr2d[Y][X])
        #print(vector.z)
        MatrixPts[i][2] = vector.z

        xval ,yval , zval = TransformSmoothParameters(vector)
        #print(xval)
        #print(yval)
        #print(vector.z)

        #velocity calculaton to be replaced by better math
        velocity += (vector.z - MatrixInit[i][2])/((currenttime-initialtime)*0.000001)

    velocity /= NUMofP

    print(velocity)

    #print("Depth Found")
    #print(MatrixPts)




    cv.imshow('KINECT depth Stream', arr2d)

    old_gray = frame_gray.copy()
    p0 = p1.copy()
    MatrixInit = MatrixPts.copy()



kinect.video_frame_ready += getColorImage
kinect.video_stream.open(nui.ImageStreamType.Video, 2,nui.ImageResolution.Resolution640x480,nui.ImageType.Color)
cv.namedWindow('KINECT Video Stream', cv.WINDOW_AUTOSIZE)

kinect.depth_frame_ready += getDepthImage
kinect.depth_stream.open(nui.ImageStreamType.Depth, 2, nui.ImageResolution.Resolution640x480, nui.ImageType.Depth)
cv.namedWindow('KINECT depth Stream', cv.WINDOW_AUTOSIZE)

while True:
    key = cv.waitKey(0)
    if key == 116:
        save_image = not save_image
    if key == 27:
        cv.destroyAllWindows()
        kinect.close()
        break


