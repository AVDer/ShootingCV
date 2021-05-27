import cv2


kThreshold = 50


def detect_center(image, center, blur, threshold=kThreshold):
    x_max = 0
    y_max = 0
    x_min = 0
    y_min = 0

    (image_height, image_width, _) = image.shape

    blurry_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    blurry_image = cv2.blur(blurry_image, (blur, blur))

    for c_temp in range(center[0], image_width):
        if blurry_image.item((center[1], c_temp)) > threshold:
            x_max = c_temp
            break

    for c_temp in range(center[0], 0, -1):
        if blurry_image.item((center[1], c_temp)) > threshold:
            x_min = c_temp
            break

    for c_temp in range(center[1], image_height):
        if blurry_image.item((c_temp, center[0])) > threshold:
            y_max = c_temp
            break

    for c_temp in range(center[1], 0, -1):
        if blurry_image.item((c_temp, center[0])) > threshold:
            y_min = c_temp
            break

    return {'x_min': x_min, 'x_max': x_max, 'y_min': y_min, 'y_max': y_max}
