import threading
import time

from ray_search import get_ray_square
from target_provider import VideoTargetProvider

from datetime import datetime

class Target:
    def __init__(self, callback):
        self._callback = callback
        self._running = False
        self._frame_provider = VideoTargetProvider("")
        self._pr = None

    def start(self):
        self._running = True
        self._pr = threading.Thread(target=self._run)
        self._pr.start()

    def _run(self):
        print(".")
        while self._running:
            frame = self._frame_provider.get_frame()
            dot_position = get_ray_square(frame)
            self._callback(dot_position)
            time.sleep(1)

'''
with open("output.txt", "w", encoding="utf-8") as f:
    while True:
        frame = frame_provider.get_frame()
        dot_position = get_ray_square(frame)
        #timestamp = datetime.now()
        #f.write(timestamp.strftime("%H-%M-%S.%f") + '\t' + str(dot_position) + '\n')
        #cv2.circle(frame, dot_position, 5, (0, 255, 0), -1)
        #cv2.imwrite('captured_image.jpg', frame)
        #break
'''

'''
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Cannot open camera")
    exit()

# Read one frame
ret, frame = cap.read()

if not ret:
    print("Error: Cannot read frame")
else:
    # Save the frame as an image
    cv2.imwrite('captured_image.jpg', frame)
    print("Image saved as 'captured_image.jpg'")

# Release the camera
cap.release()
cv2.destroyAllWindows()
'''