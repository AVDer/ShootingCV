import cv2


def get_ray_square(image):
    bwImage = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, _, _, maxLoc = cv2.minMaxLoc(bwImage)
    return maxLoc
