#!/usr/bin/env python
import cv2
import numpy as np


#------------------#
#    Preprocess    #
#------------------#

def DoG(img, kern1=(3, 3), kern2=(5, 5)):
    """Difference of Gaussian using diff kernel size"""
    smooth1 = cv2.GaussianBlur(img, kern1, 0)
    smooth2 = cv2.GaussianBlur(img, kern2, 0)
    final = smooth1 - smooth2
    return final


def norm_illum_color(img, gamma=2.2):
    """ Normalizes illumination for colored image """
    img = np.float32(img)
    img /= 255.0
    img = cv2.pow(img, 1 / gamma) * 255
    img = np.uint8(img)
    return img


def gamma_correct(img):
    gamma = 2.2
    inverse_gamma = 1.0 / gamma
    b, g, r = cv2.split(img)
    b = np.uint8(cv2.pow(b / 255.0, inverse_gamma) * 255)
    g = np.uint8(cv2.pow(g / 255.0, inverse_gamma) * 255)
    r = np.uint8(cv2.pow(r / 255.0, inverse_gamma) * 255)
    return cv2.merge((b, g, r))


def sharpen(img):
    blur = cv2.GaussianBlur(img, (5, 5), 5)
    res = cv2.addWeighted(img, 1.5, blur, -0.5, 0)
    return res


def deilluminate(img):
    h, s, gray = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2HSV))
    blur = cv2.GaussianBlur(gray, (63, 63), 41)
    gray = cv2.log(np.float32(gray))
    blur = cv2.log(np.float32(blur))
    res = np.exp(gray - blur)
    res = cv2.normalize(res, 0, 255, cv2.NORM_MINMAX) * 255
    v = np.uint8(res)
    return cv2.cvtColor(cv2.merge((h, s, v)), cv2.COLOR_HSV2BGR)


def homomorphic(img):
    h, s, gray = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2HSV))
    gray = cv2.log(np.float32(gray))
    blur = cv2.GaussianBlur(gray, (63, 63), 41)
    res = np.exp(gray - blur)
    res = cv2.normalize(res, 0, 255, cv2.NORM_MINMAX) * 255
    v = np.uint8(res)
    return cv2.cvtColor(cv2.merge((h, s, v)), cv2.COLOR_HSV2BGR)


def deilluminate_single(gray):
    blur = cv2.GaussianBlur(gray, (63, 63), 41)
    gray = cv2.log(np.float32(gray))
    blur = cv2.log(np.float32(blur))
    res = np.exp(gray - blur)
    res = cv2.normalize(res, 0, 255, cv2.NORM_MINMAX) * 255
    gray = np.uint8(res)
    return gray


def motion_deflicker(frames, img):
    log_median = cv2.log(np.float32(np.median(frames, axis=0)))
    log_img = cv2.log(np.float32(img))
    diff = cv2.GaussianBlur(log_img - log_median, (21, 21), 0)
    res = img / np.exp(diff)
    res = res.clip(max=255)
    blur = cv2.GaussianBlur(np.uint8(res), (5, 5), 0)
    res = cv2.addWeighted(np.uint8(res), 1.5, blur, -0.5, 0)
    return res


def deilluminate2(img):
    b, g, r = cv2.split(img)
    h, s, v = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2HSV))
    log_v = cv2.log(np.float32(v))
    blur_v = cv2.log(np.float32(cv2.GaussianBlur(v, (63, 63), 41)))
    res = np.exp(log_v - blur_v)
    return cv2.cvtColor(np.uint8(res * 255), cv2.COLOR_GRAY2BGR)


def gamma1(gray):
    gray = np.float32(gray)
    gray /= 255.0
    gray = 0.3 * ((cv2.log(2 * gray + 0.1)) + abs(np.log(0.1)))
    return np.uint8(gray * 255)


def gamma2(gray):
    gray = np.float32(gray)
    gray /= 255.0
    gray = 0.8 * (cv2.pow(gray, 2))
    return np.uint8(gray * 255)


def gamma3(gray):
    gray = np.float32(gray)
    gray /= 255.0
    total = 1 / (np.exp(8 * (gray - 0.5)) + 1) * 255
    return np.uint8(total)


def gamma1color(img):
    b, g, r = cv2.split(img)
    b = gamma1(b)
    g = gamma1(g)
    r = gamma1(r)
    return cv2.merge((b, g, r))


def gamma2color(img):
    b, g, r = cv2.split(img)
    b = gamma2(b)
    g = gamma2(g)
    r = gamma2(r)
    return cv2.merge((b, g, r))


def gamma3color(img):
    b, g, r = cv2.split(img)
    b = gamma3(b)
    g = gamma3(g)
    r = gamma3(r)
    return cv2.merge((b, g, r))


def single_deflicker(grayimgs):
    logimgs = [cv2.log(np.float32(x)) for x in grayimgs]
    median = np.median(logimgs, axis=0)
    diff = np.abs(logimgs[-1] - median)
    blur = cv2.GaussianBlur(diff, (3, 3), 1, 1)
    illumination_est = np.exp(blur)
    output = grayimgs[-1] / (illumination_est)
    return output


def motion_deflicker2(imgs):
    """A motion compensated approach to remove sunlight flicker
    Choice of low-pass filter could be changed or used a different standard deviation
    """
    b = [x[:, :, 0] for x in imgs]
    g = [x[:, :, 1] for x in imgs]
    r = [x[:, :, 2] for x in imgs]
    b_corrected = single_deflicker(b)
    g_corrected = single_deflicker(g)
    r_corrected = single_deflicker(r)
    return cv2.merge((np.uint8(b_corrected), np.uint8(g_corrected), np.uint8(r_corrected)))


def single_homomorphic_filter(grayimg):
    log = np.nan_to_num(np.log(np.float32(grayimg)))
    blur = np.nan_to_num(cv2.GaussianBlur(log, (21, 21), 21))
    output = np.exp(cv2.addWeighted(log, 0.5, blur, 0.5, 0))
    return output

def homomorphic_filter(img):
    """Homomorphic filtering"""
    b, g, r = cv2.split(img)
    b_corrected = single_homomorphic_filter(b)
    g_corrected = single_homomorphic_filter(g)
    r_corrected = single_homomorphic_filter(r)
    return cv2.merge((np.uint8(b_corrected), np.uint8(g_corrected), np.uint8(r_corrected)))

def contrast_stretching(chan):
    chan = np.float32(chan)
    c_max = np.max(chan)
    c_min = np.min(chan)
    chan = (chan-c_min)*((255.0-0)/(c_max-c_min))
    return chan


#--------------#
#    Enhance   #
#--------------#

def novel_color_correct(img):
    l, a, b = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2LAB))
    clahe = cv2.createCLAHE(clipLimit=1.0, tileGridSize=(3, 3))
    a = clahe.apply(a)
    b = clahe.apply(b)
    return cv2.cvtColor(cv2.merge((l, np.uint8(a), np.uint8(b))), cv2.COLOR_LAB2BGR)


def meanFilter(chan):
    y, x = chan.shape[:2]
    chan = cv2.resize(chan, (x / 2, y / 2))
    return np.uint8(blockiter(chan, np.mean, blksize=(10, 10)))


def log_chroma(img):
    """Log-chromacity"""
    b, g, r = cv2.split(img)
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    sum = cv2.pow(b + g + r + 0.1, 1 / 3.0)
    b = b / sum
    g = g / sum
    r = r / sum
    b = cv2.log(b)
    g = cv2.log(g)
    r = cv2.log(r)
    b = cv2.normalize(b, 0, 255, cv2.NORM_MINMAX) * 255
    g = cv2.normalize(g, 0, 255, cv2.NORM_MINMAX) * 255
    r = cv2.normalize(r, 0, 255, cv2.NORM_MINMAX) * 255
    out = cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))
    return out


def enhance_tan(img):
    """Tan's method to enhance image"""
    gamma = gamma_correct(img)
    b, g, r = cv2.split(gamma)
    b = cv2.equalizeHist(b)
    g = cv2.equalizeHist(g)
    r = cv2.equalizeHist(r)
    out = cv2.merge((b, g, r))
    return out


def util_iace(channel):
    min__val, max__val, min_loc, max_loc = cv2.minMaxLoc(channel)
    min_val, max_val = hist_info(channel)
    channel_ = (channel - min__val) / (max__val - min__val) * 255.0
    return channel_


def iace(img):
    b, g, r = cv2.split(img)
    b_ = util_iace(b)
    g_ = util_iace(g)
    r_ = util_iace(r)
    out = cv2.merge((np.uint8(b_), np.uint8(g_), np.uint8(r_)))  # scale up to 255 range
    return out


def french_preprocess(img):
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    y, cr, cb = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2YCR_CB))
    homo = deilluminate_single(y)
    ansio = cv2.GaussianBlur(homo, (5, 5), 1)
    bgr = cv2.cvtColor(cv2.merge((ansio, cr, cb)), cv2.COLOR_YCR_CB2BGR)
    b, g, r = cv2.split(bgr)
    b = cv2.equalizeHist(b)
    g = cv2.equalizeHist(g)
    r = cv2.equalizeHist(r)
    out = cv2.merge((b, g, r))
    return out


def hybrid_clahe(img):
    img = cv2.medianBlur(img, 5)
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(3, 3))
    h, l, s = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2HLS))
    s = clahe.apply(s)
    l = clahe.apply(l)
    hls2bgr = cv2.cvtColor(cv2.merge((h, l, s)), cv2.COLOR_HLS2BGR)
    b_, g_, r_ = cv2.split(hls2bgr)
    b, g, r = cv2.split(img)
    r = clahe.apply(r)
    g = clahe.apply(g)
    b = clahe.apply(b)
    rgb = cv2.merge((b, g, r))
    out = cv2.addWeighted(hls2bgr, 0.4, rgb, 0.4, 0)
    return out


def grayworld(img):
    b, g, r = cv2.split(img)
    r_mean = np.mean(r)
    g_mean = np.mean(g)
    b_mean = np.mean(b)
    gray = np.mean([r_mean, b_mean, g_mean])
    gray = 0.5 + 0.2 * gray
    b = gray / b_mean * b
    g = gray / g_mean * g
    r = gray / r_mean * r
    b = b.clip(max=255)
    g = g.clip(max=255)
    r = r.clip(max=255)
    return cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))


def sod_minkowski(img):
    """Minkowski P-Norm Shades of Grey"""
    b, g, r = cv2.split(img)
    gray = np.mean([np.mean(b), np.mean(g), np.mean(r)])
    gray = np.power(gray, 1 / 6.0)
    r = gray / np.mean(r) * r
    r = np.uint8(cv2.normalize(r, 0, 255, cv2.NORM_MINMAX) * 255)
    g = gray / np.mean(g) * g
    g = np.uint8(cv2.normalize(g, 0, 255, cv2.NORM_MINMAX) * 255)
    b = gray / np.mean(b) * b
    b = np.uint8(cv2.normalize(b, 0, 255, cv2.NORM_MINMAX) * 255)
    return cv2.merge((b, g, r))


def sodnorm1(img):
    """Shades of gray norm 1"""
    b, g, r = cv2.split(img)
    gray = np.max([np.mean(b), np.mean(g), np.mean(r)])
    r = cv2.normalize(gray / np.mean(r) * r, 0, 255, cv2.NORM_MINMAX) * 255
    b = cv2.normalize(gray / np.mean(b) * b, 0, 255, cv2.NORM_MINMAX) * 255
    g = cv2.normalize(gray / np.mean(g) * g, 0, 255, cv2.NORM_MINMAX) * 255
    return cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))


def dark_channel(img):
    """Dark Channel Prior"""
    kern = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    darkMap = np.zeros(img.shape[:2], dtype=np.uint8)
    tMap = np.zeros(img.shape[:2], dtype=np.float32)
    h, w, _ = img.shape
    w /= 40
    h /= 40
    b, g, r = cv2.split(img)
    y, cr, cb = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2YCR_CB))
    x = 0
    y = 0
    for i in xrange(40):
        for j in xrange(40):
            bmin, _, _, _ = cv2.minMaxLoc(b[y:y + h - 1, x:x + w - 1])
            gmin, _, _, _ = cv2.minMaxLoc(g[y:y + h - 1, x:x + w - 1])
            rmin, _, _, _ = cv2.minMaxLoc(r[y:y + h - 1, x:x + w - 1])
            dark = min(gmin, rmin)
            darkMap[y:y + h - 1, x:x + w - 1] = dark
            x += w
        x = 0
        y += h
    _, ambient_max, _, ambient_loc = cv2.minMaxLoc(darkMap)
    x = 0
    y = 0
    bmax = b[ambient_loc[1], ambient_loc[0]]
    gmax = g[ambient_loc[1], ambient_loc[0]]
    rmax = r[ambient_loc[1], ambient_loc[0]]
    for i in xrange(40):
        for j in xrange(40):
            bmin, _, _, _ = cv2.minMaxLoc(b[y:y + h - 1, x:x + w - 1])
            gmin, _, _, _ = cv2.minMaxLoc(g[y:y + h - 1, x:x + w - 1])
            rmin, _, _, _ = cv2.minMaxLoc(r[y:y + h - 1, x:x + w - 1])
            t = min(gmin / gmax, rmin / rmax)
            tMap[y:y + h - 1, x:x + w - 1] = max((1 - t), 0.1)
            x += w
        x = 0
        y += h
    tMap = cv2.dilate(tMap, kern, 3)
    tMap = cv2.erode(tMap, kern, 3)
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    b = (b - bmax) / tMap + bmax
    b = cv2.normalize(b, 0, 255, cv2.NORM_MINMAX) * 255 * 255
    g = (g - gmax) / tMap + gmax
    g = cv2.normalize(g, 0, 255, cv2.NORM_MINMAX) * 255 * 255
    # r = (r - rmax)/tMap+rmax
    return cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))
    # return cv2.cvtColor(np.uint8(tMap*255),cv2.COLOR_GRAY2BGR)


def redchannelprior(img):
    img = cv2.resize(img, (img.shape[1] / 2, img.shape[0] / 2))
    b, g, r = cv2.split(img)
    waterEst = cv2.GaussianBlur(r, (5, 5), 0)
    minval, maxval, minloc, maxloc = cv2.minMaxLoc(waterEst)
    A = img[maxloc[1], maxloc[0]]
    A = [i / 255.0 for i in A]
    b = np.float32(b) / 255.0
    g = np.float32(g) / 255.0
    r = np.float32(r) / 255.0
    t_bound = np.full(img.shape[:2], 1)
    r_min = blockiter(1 - r, np.min) / float(1 - A[2])
    g_min = blockiter(g, np.min) / float(A[1])
    b_min = blockiter(b, np.min) / float(A[0])
    tMap = t_bound - np.min([r_min, b_min, g_min], axis=0)
    tMap = cv2.GaussianBlur(tMap, (11, 11), 0)
    # return VUtil.toBGR(np.uint8(tMap*255), 'gray')
    return redchannel_util(img, A, tMap)


def redchannel_util(img, A, t):
    bgr = cv2.split(img)
    bgr = [np.float32(i / 255.0) for i in bgr]
    t_bound = np.full(img.shape[:2], 0.1)
    additive = [(1 - i) * i for i in A]
    J = [(i - A[x]) / np.maximum(t, t_bound) + additive[x] for x, i in enumerate(bgr)]
    J = [np.uint8(z_norm(j) * 255) for j in J]
    print(np.max(J[0]))
    print(np.max(J[1]))
    print(np.max(J[2]))
    return cv2.merge(tuple(J))


def naive_fusionmap(img):
    """Merge weight maps without multiple scale fusion"""
    img = cv2.resize(img, (img.shape[1] / 2, img.shape[0] / 2))
    b, g, r = cv2.split(img)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    maps = [getExposedness(img), cv2.cvtColor(get_salient(gray), cv2.COLOR_GRAY2BGR),
            getLuminance(img), getChromatic(img)]
    maps = [cv2.cvtColor(i, cv2.COLOR_BGR2GRAY) for i in maps]
    mean = np.mean(maps, axis=0) / 255.0 * 8
    mean.clip(max=1.0, out=mean)
    b = np.uint8(mean * b)
    g = np.uint8(mean * g)
    r = np.uint8(mean * r)
    return cv2.merge((b, g, r))


def showWMaps(img):
    """Debug all 6 weight maps before fusion"""
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    laplace = laplacian(img)
    local_contrast = getLocalContrast(img)
    exposedness = getExposedness(img)
    chromatic = getChromatic(img)
    salient = cv2.cvtColor(get_salient(gray), cv2.COLOR_GRAY2BGR)
    luminance = getLuminance(img)
    h1 = np.hstack((laplace, local_contrast, exposedness))
    h2 = np.hstack((chromatic, salient, luminance))
    return np.vstack((h1, h2))


def getLocalContrast(img):  # can find also std between channel and saturation
    h, s, v = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2HSV))
    blur = cv2.GaussianBlur(v, (5, 5), 0)
    final = np.std([v, blur], axis=0)
    final = cv2.normalize(final, 0, 255, cv2.NORM_MINMAX) * 255
    return cv2.cvtColor(np.uint8(final), cv2.COLOR_GRAY2BGR)


def getExposedness(img):  # can find also std between channel and saturation
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    sigma = 2 * (0.25**2)
    final = np.power(img - 0.5, 2) / sigma
    final = cv2.normalize(final, 0, 255, cv2.NORM_MINMAX)
    final = np.exp(-1 * final) * 255
    return cv2.cvtColor(np.uint8(final), cv2.COLOR_GRAY2BGR)


def getChromatic(img):  # can find also std between channel and saturation
    b, g, r = cv2.split(img)
    h, s, v = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2HSV))
    final = np.std([b, g, r, s], axis=0)
    return cv2.cvtColor(np.uint8(final), cv2.COLOR_GRAY2BGR)


def get_salient(chan):
    empty = np.ones_like(chan)
    mean = np.mean(chan)
    mean = empty * mean
    blur = cv2.GaussianBlur(chan, (21, 21), 1)
    final = mean - blur
    final = final.clip(min=0)
    final = np.uint8(final)
    return final


def get_salient_aggregate(img):
    """ Returns saliency map by combining each channels in the img """
    a, b, c = cv2.split(img)
    a = cv2.cvtColor(get_salient(a), cv2.COLOR_GRAY2BGR)
    b = cv2.cvtColor(get_salient(b), cv2.COLOR_GRAY2BGR)
    c = cv2.cvtColor(get_salient(c), cv2.COLOR_GRAY2BGR)
    return np.uint8(np.mean([a, b, c], axis=0))


def get_salient_color(img):
    a, b, c = cv2.split(img)
    a = cv2.cvtColor(get_salient(a), cv2.COLOR_GRAY2BGR)
    b = cv2.cvtColor(get_salient(b), cv2.COLOR_GRAY2BGR)
    c = cv2.cvtColor(get_salient(c), cv2.COLOR_GRAY2BGR)
    return np.hstack((a, b, c))

def get_salient_hsv(img):
    h, s, v = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2HSV))
    h = cv2.cvtColor(get_salient(h), cv2.COLOR_GRAY2BGR)
    s = cv2.cvtColor(get_salient(255-s), cv2.COLOR_GRAY2BGR)
    v = cv2.cvtColor(get_salient(v), cv2.COLOR_GRAY2BGR)
    return np.hstack((h, s, v))

def get_salient_lab(img):
    l, a, b = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2LAB))
    l = cv2.cvtColor(get_salient(l), cv2.COLOR_GRAY2BGR)
    a = cv2.cvtColor(get_salient(255-a), cv2.COLOR_GRAY2BGR)
    b = cv2.cvtColor(get_salient(255-b), cv2.COLOR_GRAY2BGR)
    return np.hstack((l, a, b))

def getLuminance(img):
    b, g, r = cv2.split(img)
    h, s, v = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2HSV))
    final = np.std([b, g, r, v], axis=0)
    return cv2.cvtColor(np.uint8(final), cv2.COLOR_GRAY2BGR)


def laplacian(img):
    return cv2.cvtColor(np.uint8(
        cv2.Laplacian(cv2.cvtColor(img, cv2.COLOR_BGR2GRAY), cv2.CV_64F)), cv2.COLOR_GRAY2BGR)


def chromaiter(img, cycle=2):
    b, g, r = cv2.split(img)
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    for i in xrange(cycle):
        b = b / (b + g + r) * 255
        g = g / (b + g + r) * 255
        r = r / (b + g + r) * 255
    out = cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))
    return out


def finlayiter(img, cycle=2):
    b, g, r = cv2.split(img)
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    for i in xrange(cycle):
        b = b / (b + g + r) * 255
        g = g / (b + g + r) * 255
        r = r / (b + g + r) * 255
        bmean = np.mean(b)
        gmean = np.mean(g)
        rmean = np.mean(r)
        b = b / bmean
        g = g / gmean
        r = r / rmean
        b = cv2.normalize(b, 0, 255, cv2.NORM_MINMAX) * 255
        g = cv2.normalize(g, 0, 255, cv2.NORM_MINMAX) * 255
        r = cv2.normalize(r, 0, 255, cv2.NORM_MINMAX) * 255
    out = cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))
    return out


def finlaynorm(img, cycle=2):
    b, g, r = cv2.split(img)
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    # Prevent division by 0
    b = b / (b + g + r + 0.001) * 255
    g = g / (b + g + r + 0.001) * 255
    r = r / (b + g + r + 0.001) * 255
    bmean = np.mean(b)
    gmean = np.mean(g)
    rmean = np.mean(r)
    b = b / bmean
    g = g / gmean
    r = r / rmean
    b = cv2.normalize(b, 0, 255, cv2.NORM_MINMAX) * 255
    g = cv2.normalize(g, 0, 255, cv2.NORM_MINMAX) * 255
    r = cv2.normalize(r, 0, 255, cv2.NORM_MINMAX) * 255
    b = b.clip(max=255)
    g = g.clip(max=255)
    r = r.clip(max=255)
    out = cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))
    return out


def perfectnorm(img, cycle=2):
    b, g, r = cv2.split(img)
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    b = b / (b + g + r) * 255
    g = g / (b + g + r) * 255
    r = r / (b + g + r) * 255
    bmean = np.mean(b)
    gmean = np.mean(g)
    rmean = np.mean(r)
    b = b / bmean
    g = g / gmean
    r = r / rmean
    b = cv2.normalize(b, 0, 255, cv2.NORM_MINMAX) * 255
    g = cv2.normalize(g, 0, 255, cv2.NORM_MINMAX) * 255
    r = cv2.normalize(r, 0, 255, cv2.NORM_MINMAX) * 255
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    b = b / (b + g + r) * 255
    g = g / (b + g + r) * 255
    r = r / (b + g + r) * 255
    out = cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))
    return out


def chromanorm(img):
    b, g, r = cv2.split(img)
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    b = b / (b + g + r) * 255
    g = g / (b + g + r) * 255
    r = r / (b + g + r) * 255
    out = cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))
    return out


def noniternorm(img):
    b, g, r = cv2.split(img)
    b = np.float32(b)
    g = np.float32(g)
    r = np.float32(r)
    log_b = cv2.log(b)
    log_g = cv2.log(g)
    log_r = cv2.log(r)
    b = cv2.exp(log_b - cv2.mean(log_b)[0])
    g = cv2.exp(log_g - cv2.mean(log_g)[0])
    r = cv2.exp(log_r - cv2.mean(log_r)[0])
    b = cv2.normalize(b, 0, 255, cv2.NORM_MINMAX) * 255
    g = cv2.normalize(g, 0, 255, cv2.NORM_MINMAX) * 255
    r = cv2.normalize(r, 0, 255, cv2.NORM_MINMAX) * 255
    b = b.clip(max=255)
    g = g.clip(max=255)
    r = r.clip(max=255)
    return cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r)))


def shade_grey_est(grayimg):
    size = grayimg.size
    power = np.power(np.float32(grayimg), 6)
    normalized_p_norm = np.power(np.sum(power) / size, 1 / 6.0)
    return normalized_p_norm


def shadegrey(img):
    """Minkowski P-Norm Shades of Grey"""
    b, g, r = cv2.split(img)
    illumination_est = np.mean([shade_grey_est(x) for x in [b, g, r]]) + 0.0001
    b_corrected = illumination_est / float(np.mean(b) + 0.001) * b
    b_corrected = b_corrected.clip(max=240)
    g_corrected = illumination_est / float(np.mean(g) + 0.001) * g
    g_corrected = g_corrected.clip(max=240)
    r_corrected = illumination_est / float(np.mean(r) + 0.001) * r
    r_corrected = r_corrected.clip(max=240)
    return cv2.merge((np.uint8(b_corrected), np.uint8(g_corrected), np.uint8(r_corrected)))


def shadegrey_lab(img):
    """Minkowski P-Norm Shades of Grey"""
    l, a, b = cv2.split(cv2.cvtColor(img, cv2.COLOR_BGR2LAB))
    illumination_est = np.median([shade_grey_est(x) for x in [l, a, b]]) + 0.0001
    # l_corrected = illumination_est / float(np.mean(l) + 0.001) * l
    # l_corrected = l_corrected.clip(max=240)
    a_corrected = illumination_est / float(np.mean(a) + 0.001) * a
    a_corrected = a_corrected.clip(max=240)
    b_corrected = illumination_est / float(np.mean(b) + 0.001) * b
    b_corrected = b_corrected.clip(max=240)
    return cv2.cvtColor(cv2.merge((np.uint8(l), np.uint8(a_corrected), np.uint8(b_corrected))), cv2.COLOR_LAB2BGR)

def enhance_red(img):
    b, g, r = cv2.split(img)
    r_stretched = contrast_stretching(r)
    return cv2.merge((np.uint8(b), np.uint8(g), np.uint8(r_stretched)))

# for sonar images
def power_law(img, power, blur=True):
    img=img.astype('float32')
    img /=255.0
    if blur:
        img = cv2.GaussianBlur(img,(5,5),5)
    img = img**power
    img *= (255.0)
    img = img.astype('uint8')
    return img

#---------------------#
#     Descriptors     #
#---------------------#




#---------------#
#     Utils     #
#---------------#

# https://www.pyimagesearch.com/2015/02/16/faster-non-maximum-suppression-python/
# Malisiewicz et al.
# return filtered bboxes or indices of bboxes picked
def non_max_suppression_fast(boxes, overlapThresh, indexes=False):
    # if there are no boxes, return an empty list
    if len(boxes) == 0:
        return []
 
    # if the bounding boxes integers, convert them to floats --
    # this is important since we'll be doing a bunch of divisions
    if boxes.dtype.kind == "i":
        boxes = boxes.astype("float")
 
    # initialize the list of picked indexes 
    pick = []
 
    # grab the coordinates of the bounding boxes
    x1 = boxes[:,0]
    y1 = boxes[:,1]
    x2 = boxes[:,2]
    y2 = boxes[:,3]
 
    # compute the area of the bounding boxes and sort the bounding
    # boxes by the bottom-right y-coordinate of the bounding box
    area = (x2 - x1 + 1) * (y2 - y1 + 1)
    idxs = np.argsort(y2)
 
    # keep looping while some indexes still remain in the indexes
    # list
    while len(idxs) > 0:
        # grab the last index in the indexes list and add the
        # index value to the list of picked indexes
        last = len(idxs) - 1
        i = idxs[last]
        pick.append(i)
 
        # find the largest (x, y) coordinates for the start of
        # the bounding box and the smallest (x, y) coordinates
        # for the end of the bounding box
        xx1 = np.maximum(x1[i], x1[idxs[:last]])
        yy1 = np.maximum(y1[i], y1[idxs[:last]])
        xx2 = np.minimum(x2[i], x2[idxs[:last]])
        yy2 = np.minimum(y2[i], y2[idxs[:last]])
 
        # compute the width and height of the bounding box
        w = np.maximum(0, xx2 - xx1 + 1)
        h = np.maximum(0, yy2 - yy1 + 1)
 
        # compute the ratio of overlap
        overlap = (w * h) / area[idxs[:last]]
 
        # delete all indexes from the index list that have
        idxs = np.delete(idxs, np.concatenate(([last],
            np.where(overlap > overlapThresh)[0])))
 
    # return only the bounding boxes that were picked using the
    # integer data type
    if indexes:
        return pick
    else:
        return boxes[pick].astype("int")
    


def z_norm(arr):
    minX = np.min(arr)
    maxX = np.max(arr)
    return arr - minX / (maxX - minX)


def blockiter(img, func, blksize=(30, 30)):
    mask = np.zeros(img.shape[:2], dtype=np.float32)
    y, x = img.shape[:2]
    for i in xrange(0, y, 5):
        dy = blksize[1] - 1 if i + blksize[1] < y else y - i - 1
        for j in xrange(0, x, 5):
            dx = blksize[0] - 1 if j + blksize[0] < x else x - j - 1
            view = img[i:i + dy, j:j + dx]
            mask[i:i + dy, j:j + dx] = func(view)
    return mask


def hist_info(chan):  # For iace
    done_low = True
    hist, bins = np.histogram(chan, 256, [0, 256])
    cdf = hist.cumsum()
    low = int(chan.size * 0.04)
    hi = int(chan.size * 0.96)
    for h, i in enumerate(cdf):
        if i > low and done_low:
            low_thresh = h
            done_low = False
        if i > hi:
            high_thresh = h
            break
    return (low_thresh, high_thresh)

class MovingAverage():
    queueList=[]
    # auto_resol resets the moving average when the resolution changes
    def __init__(self, resolution, maxSize=3, auto_resol=True):
        self.resolution=resolution
        self.maxSize = maxSize
        self.auto_resol = auto_resol
        self.accum = self.zero_image()
        for x in range(0, self.maxSize):
            self.queueList.append(self.zero_image())

    def add_image(self, img):
        out = self.queueList.pop(0)
        self.accum -= out
        if img.shape == self.resolution:
            self.queueList.append(img.astype(np.uint16, copy=False))
            self.accum += img
        elif img.shape != self.resolution and self.auto_resol:
            self.queueList = []
            self.resolution = img.shape
            self.accum = self.zero_image()
        else: # img.shape != self.resolution and not auto_resol
            img = cv2.resize(img.astype(np.uint16), self.resolution)
            self.queueList.append(img.astype(np.uint16, copy=False))
            self.accum += img
        
    def process_image(self):
        averaged = (self.accum / self.maxSize).astype(np.uint8, copy=False)
        return averaged

    def zero_image(self):
        zeroMat=np.zeros((self.resolution[1], self.resolution[0], 3)).astype(np.uint16, copy=False)
        return zeroMat

class MovingMedian():
    # auto_resol resets the moving median when the resolution changes
    def __init__(self, resolution, maxSize=3, auto_resol=True):
        self.resolution=resolution
        self.maxSize = maxSize
        self.concat = self.zero_image_stacked()
        self.auto_resol = auto_resol
        # for x in range(0, self.maxSize-1):
        #     self.concat = np.dstack((self.concat,self.zero_image()))

    def add_image(self, img):
        self.concat = self.concat[:,:,1:]
        if img.shape == self.resolution:
            self.concat = np.dstack((self.concat,img.astype(np.uint16, copy=False)))
        elif img.shape != self.resolution and self.auto_resol:
            self.concat = self.zero_image_stacked()
            self.resolution = img.shape
        else: # img.shape != self.resolution and not auto_resol
            img = cv2.resize(img.astype(np.uint16, copy=False),self.resolution)
            self.concat = np.dstack((self.concat,img.astype(np.uint16, copy=False)))
                    
    def process_image(self):
        median = np.median(self.concat, axis=2)
        median = median.astype(np.uint8, copy=False)
        return median

    def zero_image_stacked(self):
        zeroMat=np.zeros((self.resolution[0], self.resolution[1], self.maxSize)).astype(np.uint16, copy=False)
        return zeroMat
