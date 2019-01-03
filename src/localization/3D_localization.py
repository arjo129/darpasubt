# -*- coding: utf-8 -*-

"""
Histogram Filter 2D localization example
In this simulation, x,y are unknown, yaw is known.
Initial position is not needed.
author: Atsushi Sakai (@Atsushi_twi)
"""

import math
import numpy as np
import matplotlib.pyplot as plt
import copy
from scipy.stats import norm
from scipy.ndimage import gaussian_filter
from mpl_toolkits import mplot3d

# Parameters
EXTEND_AREA = 10.0  # [m] grid map extention length
SIM_TIME = 50.0  # simulation time [s]
DT = 0.1  # time tick [s]
MAX_RANGE = 10.0  # maximum observation range
MOTION_STD = 1.0  # standard deviation for motion gaussian distribution
RANGE_STD = 3.0  # standard deviation for observation gaussian distribution

# grid map param
XYZ_RESO = 0.5  # xyz grid resolutions
MINX = -15.0
MINY = -5.0
MINZ = 0.0
MAXX = 15.0
MAXY = 25.0
MAXZ = 10.0

# simulation paramters
NOISE_RANGE = 2.0  # [m] 1σ range noise parameter
NOISE_SPEED = 0.5  # [m/s] 1σ speed noise parameter


show_animation = True


class grid_map():

    def __init__(self):
        self.data = None
        self.xyzreso = None
        self.minx = None
        self.miny = None
        self.minz = None
        self.maxx = None
        self.maxx = None
        self.maxz = None 
        self.xw = None
        self.yw = None
        self.zw = None
        self.dx = 0.0  # movement distance
        self.dy = 0.0  # movement distance


def histogram_filter_localization(gmap, u, z, yaw):

    gmap = motion_update(gmap, u, yaw)

    gmap = observation_update(gmap, z, RANGE_STD)

    return gmap


def calc_gaussian_observation_pdf(gmap, z, iz, ix, iy, std):

    # predicted range
    x = ix * gmap.xyreso + gmap.minx
    y = iy * gmap.xyreso + gmap.miny
    d = math.sqrt((x - z[iz, 1])**2 + (y - z[iz, 2])**2)

    # likelihood
    pdf = (1.0 - norm.cdf(abs(d - z[iz, 0]), 0.0, std))

    return pdf


def observation_update(gmap, z, std):

    for iz in range(z.shape[0]):
        for ix in range(gmap.xw):
            for iy in range(gmap.yw):
                gmap.data[ix][iy] *= calc_gaussian_observation_pdf(
                    gmap, z, iz, ix, iy, std)

    gmap = normalize_probability(gmap)

    return gmap

def map_shift(gmap, xshift, yshift):

    tgmap = copy.deepcopy(gmap.data)

    for ix in range(gmap.xw):
        for iy in range(gmap.yw):
            nix = ix + xshift
            niy = iy + yshift

            if nix >= 0 and nix < gmap.xw and niy >= 0 and niy < gmap.yw:
                gmap.data[ix + xshift][iy + yshift] = tgmap[ix][iy]

    return gmap


def motion_update(gmap, u, yaw):

    gmap.dx += DT * math.cos(yaw) * u[0]
    gmap.dy += DT * math.sin(yaw) * u[0]

    xshift = gmap.dx // gmap.xyzreso
    yshift = gmap.dy // gmap.xyzreso

    if abs(xshift) >= 1.0 or abs(yshift) >= 1.0:  # map should be shifted
        gmap = map_shift(gmap, int(xshift), int(yshift))
        gmap.dx -= xshift * gmap.xyzreso
        gmap.dy -= yshift * gmap.xyzreso

    gmap.data = gaussian_filter(gmap.data, sigma=MOTION_STD)

    return gmap

def motion_model(x, u):

    F = np.array([[1.0, 0, 0, 0],
                  [0, 1.0, 0, 0],
                  [0, 0, 1.0, 0],
                  [0, 0, 0, 0]])

    B = np.array([[DT * math.cos(x[2, 0]), 0],
                  [DT * math.sin(x[2, 0]), 0],
                  [0.0, DT],
                  [1.0, 0.0]])

    x = np.dot(F,x) + np.dot(B,u)

    return x

def observation(xTrue, u, RFID):

    xTrue = motion_model(xTrue, u)

    z = np.zeros((0, 3))

    for i in range(len(RFID[:, 0])):

        dx = xTrue[0, 0] - RFID[i, 0]
        dy = xTrue[1, 0] - RFID[i, 1]
        dz = xTrue[2, 0] - RFID[i, 2]
        d = math.sqrt(dx**2 + dy**2 + dz**2)
        if d <= MAX_RANGE:
            # add noise to range observation
            dn = d + np.random.randn() * NOISE_RANGE
            zi = np.array([dn, RFID[i, 0], RFID[i, 1], RFID[i, 2]])
            z = np.vstack((z, zi))

    # add noise to speed
    ud = u[:, :]
    ud[0] += np.random.randn() * NOISE_SPEED

    return xTrue, z, ud

def normalize_probability(gmap):

    sump = 0 

    for iz in range(gmap.zw): 
        for iy in range(gmap.yw):
            for ix in range(gmap.xw):
                sump += gmap.data[ix][iy][iz] 
    # sump = sum([sum(igmap) for igmap in gmap.data])
    print(sump)
    print(gmap.data)

    for ix in range(gmap.xw):
        for iy in range(gmap.yw):
            for iz in range(gmap.zw):
                gmap.data[ix][iy][iz] /= sump

    return gmap

def draw_heatmap(data, mx, my, mz):
    # maxp = max([max(igmap) for igmap in data])
    # plt.pcolor(mx, my, data, vmax=maxp, cmap='RdYlGn')
    # plt.axis("equal")
    plt.axes(projection='3d')    
    plt.scatter(mx, my, mz, cmap='viridis', linewidth=0.5);

def init_gmap(xyzreso, minx, miny, minz, maxx, maxy, maxz):

    gmap = grid_map() #object

    gmap.xyzreso = xyzreso
    gmap.minx = minx
    gmap.miny = miny
    gmap.minz = minz
    gmap.maxx = maxx
    gmap.maxy = maxy
    gmap.maxz = maxz
    gmap.xw = int(round((gmap.maxx - gmap.minx) / gmap.xyzreso)) #round to nearest int & typecast to int
    gmap.yw = int(round((gmap.maxy - gmap.miny) / gmap.xyzreso))
    gmap.zw = int(round((gmap.maxz - gmap.minz) / gmap.xyzreso))

    gmap.data = [[[1.0 for i in range(gmap.zw)] for i in range(gmap.yw)] for i in range(gmap.xw)]


    gmap = normalize_probability(gmap)

    return gmap

def calc_grid_index(gmap):
    mx, my, mz = np.mgrid[slice(gmap.minx - gmap.xyzreso / 2.0, gmap.maxx + gmap.xyzreso / 2.0, gmap.xyzreso),
                      slice(gmap.miny - gmap.xyzreso / 2.0, gmap.maxy + gmap.xyzreso / 2.0, gmap.xyzreso), 
                      slice(gmap.minz - gmap.xyzreso / 2.0, gmap.maxz + gmap.xyzreso / 2.0, gmap.xyzreso)]

    return mx, my, mz

def calc_input():
    v = 1.0  # [m/s]
    yawrate = 0.1  # [rad/s]
    u = np.array([v, yawrate]).reshape(2, 1)
    return u


def main():
    print(__file__ + " start!!")

    # RFID positions [x, y, z]
    RFID = np.array([[15.0, 0.0, 5.0],
                     [15.0, 20.0, 2.0],
                     [-15.0, 20.0, 1.0],
                     [-15.0, 0.0, 4.0]])

    time = 0.0

    xTrue = np.zeros((4, 1)) # array of zeroes , 4 rows, 1 col
    gmap = init_gmap(XYZ_RESO, MINX, MINY, MINZ, MAXX, MAXY, MAXZ)
    mx, my, mz = calc_grid_index(gmap)  # for grid map visualization

    while SIM_TIME >= time:
        time += DT
        print("Time:", time)

        u = calc_input()

        yaw = xTrue[2, 0]  # Orientation is known
        xTrue, z, ud = observation(xTrue, u, RFID)

        gmap = histogram_filter_localization(gmap, u, z, yaw)

        if show_animation:
            plt.cla()
            draw_heatmap(gmap.data, mx, my, mz)
            plt.plot(xTrue[0, :], xTrue[1, :], "xr")
            plt.plot(RFID[:, 0], RFID[:, 1], RFID[:, 2], ".k")
            # for i in range(z.shape[0]):
            #     plt.plot([xTrue[0, :], z[i, 1]], [
            #              xTrue[1, :], z[i, 2]], "-k")
            plt.title("Time[s]:" + str(time)[0: 4])
            plt.pause(0.1)

    print("Done")


if __name__ == '__main__':
    main()