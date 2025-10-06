import cv2

def get_ray_square(image):
    bw_mage = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, _, _, max_loc = cv2.minMaxLoc(bw_mage)
    return max_loc

def get_contour_center(image):
    red = image[:, :, 2]
    (cx, cy) = (-1, -1)

    blurred = cv2.GaussianBlur(red, (11, 11), 0)
    _, bright = cv2.threshold(blurred, 250, 255, cv2.THRESH_BINARY)

    contours, _ = cv2.findContours(bright, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if contours:
        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)
        if M["m00"] != 0:
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])
    return (cx, cy)
