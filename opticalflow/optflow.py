import numpy as np
import cv2 as cv
import math
import time

NUMofP = 2

def computeangle(vectorA, vectorB):
    magni = math.sqrt(vectorA[0] ** 2 + vectorA[1] ** 2)
    magnf = math.sqrt(vectorB[0] ** 2 + vectorB[1] ** 2)

    dotproduct = np.dot(vectorA, vectorB)

    cosval = (dotproduct / (magni * magnf))
    if (cosval <= 1):
        return math.acos((dotproduct / (magni * magnf)))
    else:
        return 0

def getDirec(vectorA ,vectorB):

    tanA = math.atan2(vectorA[1], vectorA[0])
    tanB = math.atan2(vectorB[1], vectorB[0])

    if (tanA < tanB):
        return True
    else:
        return False


cap = cv.VideoCapture("vidtest2.mp4")
# params for ShiTomasi corner detection
feature_params = dict(maxCorners=100,
                      qualityLevel=0.8,
                      minDistance=7,
                      blockSize=7)
# Parameters for lucas kanade optical flow
lk_params = dict(winSize=(15, 15),
                 maxLevel=2,
                 criteria=(cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 10, 0.03))
# Create some random colors
color = np.random.randint(0, 255, (100, 3))
# Take first frame and find corners in it
ret, old_frame = cap.read()
old_gray = cv.cvtColor(old_frame, cv.COLOR_BGR2GRAY)
p0 = cv.goodFeaturesToTrack(old_gray, mask=None, **feature_params)

print(p0 , len(p0))

# Create a mask image for drawing purposes
mask = np.zeros_like(old_frame)

ret, frame = cap.read()
frame_gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
# calculate optical flow
#p1, st, err = cv.calcOpticalFlowPyrLK(old_gray, frame_gray, p0, None, **lk_params)

NUMofP = len(p0)

Matrixi = np.stack(p0, axis=1).ravel().reshape(NUMofP, 2)

vectorArri = []

for i in range(NUMofP):
    for j in range(i + 1, NUMofP):
        vector = Matrixi[i] - Matrixi[j]
        vectorArri.append(vector)

degcurrent = 0

while (1):
    ret, frame = cap.read()
    frame_gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    # calculate optical flow
    p1, st, err = cv.calcOpticalFlowPyrLK(old_gray, frame_gray, p0, None, **lk_params)

    MatrixCurrent = p1[st == 1]
    #good_old = p0[st == 1]

    #Matrix1 = np.stack(p1, axis=1).ravel().reshape(NUMofP, 2)

    vectorArrCurrent = []

    for i in range(NUMofP):
        for j in range(i + 1, NUMofP):
            vector = MatrixCurrent[i] - MatrixCurrent[j]
            vectorArrCurrent.append(vector)
    deg = 0
    degvalues = []
    for i in range(len(vectorArrCurrent)):
        degdelta = computeangle(vectorArrCurrent[i], vectorArri[i])
        #deg += degdelta
        degvalues.append(degdelta)

    #degavg = deg/len(vectorArrCurrent)

    maxdeg = degvalues[0]

    for i in degvalues:
        if maxdeg < i:
            maxdeg = i


    direction = getDirec(vectorArri[0], vectorArrCurrent[0])
    if(direction):
        degcurrent += maxdeg
    else:
        degcurrent -= maxdeg
    vectorArri = vectorArrCurrent.copy()
    print(degcurrent/math.pi*180)


    # Select good points
    good_new = p1[st == 1]
    good_old = p0[st == 1]


    for i, (new, old) in enumerate(zip(good_new, good_old)):
        a, b = new.ravel()
        c, d = old.ravel()
        mask = cv.line(mask, (a, b), (c, d), color[i].tolist(), 2)
        frame = cv.circle(frame, (a, b), 5, color[i].tolist(), -1)

    img = cv.add(frame, mask)

    cv.imshow('frame', img)
    k = cv.waitKey(30) & 0xff
    if k == 27:
        break
    # Now update the previous frame and previous points
    old_gray = frame_gray.copy()
    p0 = good_new.reshape(-1, 1, 2)
    time.sleep(0.1)
cv.destroyAllWindows()
cap.release()
