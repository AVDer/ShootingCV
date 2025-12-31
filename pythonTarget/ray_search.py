import cv2
import numpy as np

def get_ray_square(image):
    bw_mage = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, _, _, max_loc = cv2.minMaxLoc(bw_mage)
    return max_loc

def get_contour_center(image):
    red = image[:, :, 2]
    (cx, cy) = (-1, -1)

    blurred = cv2.GaussianBlur(red, (5, 5), 0)
    _, bright = cv2.threshold(blurred, 250, 255, cv2.THRESH_BINARY)

    contours, _ = cv2.findContours(bright, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if contours:
        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)
        if M["m00"] != 0:
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])
    return (cx, cy)

def aruco_get_transformation(image, size=1000): 
    aruco = cv2.aruco
    dictionary = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)
    parameters = aruco.DetectorParameters()
 
    detector = aruco.ArucoDetector(dictionary, parameters)
    corners, ids, rejected = detector.detectMarkers(image)
 
    ref_pts = {
        0: (0, 0),
        1: (size, 0),
        2: (size, size),
        3: (0, size)
    }
 
    img_pts = []
    doc_pts = []
 
    for corner, marker_id in zip(corners, ids.flatten()):
        if marker_id in ref_pts:
            c = corner[0].mean(axis=0)
            img_pts.append(c)
            doc_pts.append(ref_pts[marker_id])
 
    H, _ = cv2.findHomography(
        np.array(img_pts),
        np.array(doc_pts),
        cv2.RANSAC)
    
    return H

def aruco_transform(image, homography, size=1000): 
    return cv2.warpPerspective(image, homography, (size, size))