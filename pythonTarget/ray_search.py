import cv2

def get_ray_square(image):
    bw_mage = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, _, _, max_loc = cv2.minMaxLoc(bw_mage)
    return max_loc
