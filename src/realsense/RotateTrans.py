import numpy as np
import math
import matplotlib.pyplot as plt
from itertools import combinations
import pandas as pd
import random

import rmsd as rmsd


def rotation_matrix(sigma):
    """
    https://en.wikipedia.org/wiki/Rotation_matrix
    """

    radians = sigma * np.pi / 180.0

    r11 = np.cos(radians)
    r12 = -np.sin(radians)
    r21 = np.sin(radians)
    r22 = np.cos(radians)

    R = np.array([[r11, r12], [r21, r22]])

    return R


def save_plot(A, B, filename):

    Ax = A[:,0]
    Ay = A[:,2]

    Bx = B[:,0]
    By = B[:,2]

    plt.plot(Ax, Ay, 'o-', markersize=15, linewidth=3)
    plt.plot(Bx, By, 'o-', markersize=15, linewidth=3)


    plt.ylim([min(Ay)-0.5, max(Ay)+0.5])
    plt.xlim([min(Ax)-0.5, max(Ax)+0.5])
    plt.grid(True)
    plt.tick_params(labelsize=15)
    plt.savefig(filename+'.png')

    plt.clf()

    return



def getDirec(vectorA ,vectorB):

    tanA = math.atan2(vectorA[1], vectorA[0])
    tanB = math.atan2(vectorB[1], vectorB[0])

    if (tanA < tanB):
        return True
    else:
        return False


def computeangle(vA, vB):

    vectorA = vA[0:3:2]
    vectorB = vB[0:3:2]
    magni = math.sqrt(sum(i**2 for i in vectorA))
    magnf = math.sqrt(sum(i**2 for i in vectorB))

    dotproduct = np.dot(vectorA, vectorB)

    cosval = (dotproduct / (magni * magnf))


    if (cosval <= 1):
        return math.acos((dotproduct / (magni * magnf)))
    else:
        return 0

def geterror(vectorsA ,vectorsB):
    mse = (np.square(vectorsA - vectorsB)).mean()
    return mse


def getiterativeAng(pointCldA , pointCldB):
    vectorsA = np.empty((0, 3), np.float)
    vectorsB = np.empty((0, 3), np.float)

    deg = []

    for i in range(len(pointCldA)):
        for j in range(i + 1, len(pointCldA)):
            tempA = pointCldA[j] - pointCldA[i]
            vectorsA = np.vstack([vectorsA, tempA])
            tempB = pointCldB[j] - pointCldB[i]
            vectorsB = np.vstack([vectorsB, tempB])

    min = 1000
    mindeg = 0

    for degree in range(-90,90):
        A = np.array([[math.cos(degree), -math.sin(degree), 0], [math.sin(degree), math.cos(degree), 0], [0, 0, 1]])
        tempPRO = np.matmul(A.transpose(), vectorsA.transpose())
        mse = (geterror(tempPRO , vectorsB.transpose()))
        print(mse)
        if(mse < min):
            min = mse
            mindeg = degree

    print("HERE WE GO")
    print(mindeg)
    print(min)


def getMovement(pointCldA , pointCldB):
    diff = pointCldB - pointCldA
    x = 0
    y =0
    z =0
    for i in diff:
        x += i[0]
        y += i[1]
        z += i[2]
    x = x/len(diff)
    y = y/len(diff)
    z = z/len(diff)

    return x,y,z

def getAngleZ(pointCldA , pointCldB):

    vectorsA = np.empty((0, 3), np.float)
    vectorsB = np.empty((0, 3), np.float)

    deg = []

    removevec = [];

    # for i in range(len(pointCldA)):
    #     if (iszero(pointCldA[i]) or iszero(pointCldA[i])):
    #         removevec.append(i)

    # pointCldA = np.delete(pointCldA, removevec, axis=0)
    # pointCldB = np.delete(pointCldB, removevec, axis=0)


    for i in range(len(pointCldA)):
        for j in range(i+1, len(pointCldA)):

            tempA = pointCldA[j]-pointCldA[i]
            vectorsA = np.vstack([vectorsA , tempA])
            tempB = pointCldB[j] - pointCldB[i]
            vectorsB = np.vstack([vectorsB, tempB])


     #print(vectorsA)
    for i in range(len(vectorsA)):
        degtemp = (computeangle(vectorsA[i] , vectorsB[i]))
        if (getDirec(vectorsA[i] , vectorsB[i])):
            degtemp = -1 * degtemp
        deg.append(degtemp)


    n, b, patches = plt.hist(deg)

    bin_max = np.where(n == n.max())
    plt.show()
    # print("Ans")
    # print(b[bin_max][0])
    return b[bin_max][0]


def translation(degree , arr , orgi):
    A = np.array([[math.cos(-degree),0, -math.sin(-degree)],[0,1,0],[math.sin(-degree), 0,math.cos(-degree)]])
    print("Translation")
    print(np.matmul(A,np.transpose(orgi)) - np.transpose(arr))

def iszero(A):

    if(A[1] == 0):
        return True
    else:
        return False



def getallangles(A, B):

    randomnum = []

    # for x in range(10):
    #     randomnum.append(random.randint(0, len(oriA)-1))
    #
    # A = []
    # B = []
    # for j in randomnum:
    #     A.append(oriA[j])
    #     B.append(oriB[j])


    #print(A, B)
    vectors = []
    Degrees = []
    vectorsA = np.empty((0, 3), np.float)
    vectorsB = np.empty((0, 3), np.float)


    removevec = [];

    for i in range(len(A)):
        if(iszero(A[i]) or iszero(B[i])):
            removevec.append(i)

    A = np.delete(A, removevec , axis=0)
    B = np.delete(B , removevec , axis = 0)

    #print(A,B)


    number = len(A)
    #print(number)
    perma = combinations(range(number),3)
    for i in list(perma):
        vectors.append(i)

    for i in vectors:
        for j in i:
            vectorsA = np.vstack([vectorsA, A[j]])
            vectorsB = np.vstack([vectorsB, B[j]])

        Degrees.append(getkabschangle(vectorsA ,vectorsB))
        vectorsA = np.empty((0, 3), np.float)
        vectorsB = np.empty((0, 3), np.float)


    #print(Degrees)


    n, b, patches = plt.hist(Degrees)

    bin_max = np.where(n == n.max())
    plt.show()
    #print("Ans")
    #print(b[bin_max][0])
    return b[bin_max][0]


def rotationMatrixToEulerAngles(R):

    sy = math.sqrt(R[0, 0] * R[0, 0] + R[1, 0] * R[1, 0])

    singular = sy < 1e-6

    if not singular:
        x = math.atan2(R[2, 1], R[2, 2])
        y = math.atan2(-R[2, 0], sy)
        z = math.atan2(R[1, 0], R[0, 0])
    else:
        x = math.atan2(-R[1, 2], R[1, 1])
        y = math.atan2(-R[2, 0], sy)
        z = 0

    return np.array([x, y, z])

def getkabschangle(Ac,Bc):

    # Ac = Ac[:,0:3:2]
    # Bc = Bc[:,0:3:2]
    # Manipulate

    A = Ac - rmsd.centroid(Ac)
    B = Bc - rmsd.centroid(Bc)

    U = rmsd.kabsch(A, B)
    # print(U)

    degvals = (rotationMatrixToEulerAngles(U))



    return degvals[1]
    """
    if ((U[0][0]) <= 1):
        deg = math.acos(U[0][0])
    else:
        #print("OIII GOT ERROR SIAL")
        deg = 0


   

    degree =  deg
    #degree /= 2

    rotaApos = np.array([[math.cos(degree),-math.sin(degree)],  [math.sin(degree), math.cos(degree)]])
    rotaAneg = np.array([[math.cos(-degree), -math.sin(-degree)], [math.sin(-degree), math.cos(-degree)]])

    Apos = np.matmul(rotaApos ,np.transpose(Ac))
    Aneg = np.matmul( rotaAneg, np.transpose(Ac))

    #print("Aneg")
    #print(Aneg)
    #print(B)
    print("Positive")
    print(geterror(np.transpose(Apos), Bc))
    print("Negative")
    print(geterror(np.transpose(Aneg), Bc))
    #if(rmsd.rmsd(Apos, B) > rmsd.rmsd(Aneg, B) ):
    #    deg = -deg
    """
    #return deg

def kabschangleVisual(A,B):
    # Manipulate
    #A = A[:, 0:3:2]
    #B = B[:, 0:3:2]
    print(A)
    print(B)
    print("RMSD", geterror(A, B))

    save_plot(A, B, "plot_beginning")
    A -= rmsd.centroid(A)
    B -= rmsd.centroid(B)



    print("Translated RMSD", geterror(A, B))
    save_plot(A, B, "plot_translated")

    U = rmsd.kabsch(A, B)
    print("Rotation Matrix")
    print(U)
    print("Angle")


    #val = math.acos(U[0][0])
    #print(U[0][0])
    #if(abs(val) <=1):
    #    deg = math.acos(U[0][0]) / 3.14 * 180
    #else:
    #    print("OIII GOT ERROR SIAL")
    #    deg = 0
    degvals = (rotationMatrixToEulerAngles(U))
    print("The degree")
    print(degvals[1])
    #print(deg)

    #A = np.dot(A, U)

    print("Rotated RMSD", geterror(A, B))
    save_plot(A, B, "plot_rotated")

    #return deg

def calculateXYZ(ypix, xpix, centerx, centery, depthval):
    tempx = ((78 - 85) * (ypix) / 480 + 85) * abs(xpix - centerx) / 640
    tempy = abs(ypix - centery) * 65 / 480
    tempz = 69

    S = tempx * tempx + tempy * tempy + tempz * tempz
    S = depthval * depthval / S
    S = math.sqrt(S)

    actualx = ((78 - 85)* (ypix) / 480 + 85) *S * (xpix - centerx) / 640
    actualy = (centery - ypix)/480 * 65 * S
    actualz = S*69

    print(actualx)
    print(actualy)
    print(actualz)

    return actualx , actualy , actualz

"""
x1 = np.array([[-1.6, 0.40, 0], [-1.2,0.8,0],[-1.6, 0.8, 0],[-2,0.8,0]], np.float)


x2 = np.array([[0, 0.8, 0], [0.4, 0.4, 0], [0.4, 0.8, 0], [0.4, 1.2, 0]], np.float)

#x2 = np.array([[0.4, 2.40, 1], [1.6,2.8,2],[2.4, 1.6, 0],[1.2,0.8,0.5]], np.float)


#x1 = np.array([[0, 1.14, 0], [1.14, 0.57, 1], [0.855, -0.855, -1], [-0.57, -0.57, -0.5]], np.float)

#x1 = np.array( [[-0.87 ,-0.22,  1.1 ] ,[-0.69 , 0.01 , 1.07] ,[-1.13 , -0.22 , 1.28],[-0.85 ,-0.34 , 1.12] ], np.float)

x2 = np.array( [[-0.87 , -0.22 , 1.1 ] , [-0.69 , 0.01 , 1.07] ,[-1.16 ,-0.23 , 1.31] , [-0.85 , -0.34 , 1.11] ] , np.float)

x3 = np.array( [[-0.93 ,-0.23 ,1.17] , [-0.77 , 0.01 , 1.2 ],[-1.04 ,-0.21  ,1.17],[-0.88, -0.35 , 1.15]] , np.float)

v1 =  np.array( [[0.17 ,0.04,  0.41 ] ,[0.08 , 0.07 , 0.66] ,[0.11 , 0.04 , 0.67] ], np.float)
v2 =  np.array( [[0.03 ,0.04,  0.42 ] ,[-0.13 , 0.06 , 0.64] ,[-0.1 , 0.03 , 0.66] ], np.float)

v =  np.array(([21, 22, 23]))

cldA =  v1[:,0:3:2]
cldB = v2[:,0:3:2]

d1 =  np.array([[ 0.27 , 0.35 , 1.31], [ 0.18,  0.27 , 1.35],[ 0.17 , 0.07 , 1.34], [ 0.06, -0.06  ,1.33], [ 0.36 , 0.34 , 1.27], [ 0.1  , 0.11 , 1.37], [ 0.26 , 0.15 , 1.31] ,[ 0.45 , 0.25  ,1.25], [ 0.19, -0.08 , 1.31],[ 0.52 , 0.08 , 1.19], [ 0.08 , 0.05,  1.39], [ 0.18 , 0.2 ,  1.34], [ 0.08 , 0.27 , 1.42]], np.float)
d2 =  np.array([[ 0.27 , 0.35 , 1.31], [ 0.18,  0.27 , 1.35],[ 0.17 , 0.07 , 1.34], [ 0.06, -0.06  ,1.32], [ 0.36 , 0.34 , 1.27], [ 0.1  , 0.11 , 1.36], [ 0.26 , 0.15 , 1.31] ,[ 0.45 , 0.25  ,1.25], [ 0.19, -0.08 , 1.31],[ 0.52 , 0.08 , 1.19], [ 0.08 , 0.05,  1.39], [ 0.18 , 0.2 ,  1.34], [ 0.08 , 0.27 , 1.42]], np.float)

e1 = np.array([[-0.23, -0.23 , 1.52], [-0.2 , -0.24  ,1.56], [-0.11, -0.14 , 1.54]], np.float)
e2 = np.array([[-0.23, -0.23,  1.54], [-0.2 , -0.24 , 1.57], [-0.11 ,-0.14  ,1.54]] , np.float)


f1 = np.array([[ 0.51 ,-0.08 , 1.3 ],
 [ 0.04 ,-0.46 , 1.57],
 [ 0.53 , 0.1 ,  1.23],
 [ 0.52 , 0.08 , 1.22],
 [-0.25 , 0.1 ,  1.49],
 [-0.36 , 0.01 , 1.47],
 [-0.34 , 0.   , 1.5 ],
 [ 0.49, -0.22 , 1.19]],np.float)

f2 = np.array([[ 0. ,  -0. ,   0.  ],
 [ 0.04, -0.46 , 1.58],
 [ 0.53 , 0.1 ,  1.23],
 [ 0. ,   0. ,   0.  ],
 [-0.25 , 0.1 ,  1.49],
 [-0.36 , 0.01 , 1.47],
 [-0.34 , 0. ,   1.49],
 [ 0.5 , -0.23,  1.21]] , np.float)
#degree = getiterativeAng(x1, x2)

#kabschangleVisual(f1 ,f2)


#translation(0.2756 , v2.transpose() , v1.transpose() )

#print("Done that part")

#calculateXYZ(231.83926392 , 480.31890869  ,320 , 360 , 143)


#print("Angles")

#print(getkabschangle(d1 , d2))

#print("error")

#print(geterror(v1[:, 0:3:2],v2[:, 0:3:2]))


#print("Original Angle")
#getAngleZ(e1, e2)

print(getallangles(f1, f2))
"""