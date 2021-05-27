
import cv2


class TargetProvider:
    
    def __init__(self, url):
        self.url_ = url
    
    def load(self):
        pass
    
    def get_frame(self):
        pass
    
    
class ImageTargetProvider(TargetProvider):
    
    def get_frame(self, scale_size_y=0):
        frame = cv2.imread(self.url_, cv2.IMREAD_COLOR)
        return cv2.resize(frame, (600, 800))


class VideoTargetProvider(TargetProvider):

    def __init__(self, url):
        super().__init__(url)
        self.cap_ = cv2.VideoCapture(1)
        if not self.cap_.isOpened():
            print("Cannot open camera")

    def __del__(self):
        self.cap_.release()

    def get_frame(self, scale_size_y=0):
        if not self.cap_.isOpened():
            print("Cannot open camera")
            return None
        ret, frame = self.cap_.read()
        if not ret:
            print("Can't receive frame")
            return None
        return cv2.resize(frame, (600, 800))
