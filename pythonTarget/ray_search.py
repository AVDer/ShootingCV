import cv2
import sys


def get_ray_square(image):
    bwImage = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, _, _, maxLoc = cv2.minMaxLoc(bwImage)
    return maxLoc

    '''
    top_left = (sys.maxsize, sys.maxsize)
    bottom_right = (0, 0)
    max_bright = 0
    (height, width, color_channel) = image.shape
    if color_channel < 3:
        print('Error, number of color channels = {}'.format(color_channel))
        return
    # Get maximal brightness
    for x in range(0, width):
        for y in range(0, height):
            bright = image.item(y, x, 0) + image.item(y, x, 1) + image.item(y, x, 2)
            if bright > max_bright:
                max_bright = bright
    print('Maximal brightness = {}'.format(max_bright))
    # Scan image
    for x in range(0, width):
        for y in range(0, height):
            bright = image.item(y, x, 0) + image.item(y, x, 1) + image.item(y, x, 2)
            if bright == max_bright:
                if x < top_left[0] or y < top_left[1]:
                    top_left = (x, y)
                if x > bottom_right[0] or y > bottom_right[1]:
                    bottom_right = (x, y)
    return top_left, bottom_right
    # print(top_left, bottom_right)
    '''