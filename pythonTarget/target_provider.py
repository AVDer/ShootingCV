import cv2
import numpy as np
import random

class TargetProvider:

    K_IMAGE_SIZE = 1000
    
    def __init__(self, url):
        self.url_ = url
    
    def load(self):
        pass
    
    def get_frame(self):
        pass

    def reshape_frame(self, input_frame):
        (h, w, _) = input_frame.shape
        min_dim = min(h, w)
        x = w / 2 - min_dim / 2
        y = h / 2 - min_dim / 2
        return cv2.resize(input_frame[int(y):int(y + min_dim), int(x):int(x + min_dim)], (self.K_IMAGE_SIZE, self.K_IMAGE_SIZE))
    
    
class ImageTargetProvider(TargetProvider):

    def __init__(self, url):
        super().__init__(url)
    
    def get_frame(self):
        frame = cv2.imread(self.url_, cv2.IMREAD_COLOR)
        return self.reshape_frame(frame)

class RandomTargetProvider(TargetProvider):
    
    def __init__(self, url):
        super().__init__(url)
    
    def get_frame(self):
        frame = np.zeros(shape=(self.K_IMAGE_SIZE, self.K_IMAGE_SIZE, 3), dtype=np.uint8);
        x = random.randint(0, self.K_IMAGE_SIZE - 1)
        y = random.randint(0, self.K_IMAGE_SIZE - 1)
        cv2.circle(frame, center=(x, y), radius=3, color=(0, 0, 255), thickness=-1)
        return self.reshape_frame(frame)

class VideoTargetProvider(TargetProvider):

    def __init__(self, url):
        super().__init__(url)
        self.cap_ = cv2.VideoCapture(0)
        if not self.cap_.isOpened():
            print("Cannot open camera")
        self.cap_.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)
        self.cap_.set(cv2.CAP_PROP_EXPOSURE, -30)

    def __del__(self):
        self.cap_.release()

    def get_frame(self):
        if not self.cap_.isOpened():
            print("Cannot open camera")
            return None
        ret, frame = self.cap_.read()
        if not ret:
            print("Can't receive frame")
            return None
        return self.reshape_frame(frame)
