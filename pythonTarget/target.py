import cv2
import threading
import time

from ray_search import get_contour_center
from ray_search import aruco_get_transformation
from ray_search import aruco_transform

from datetime import datetime

from target_provider import RandomTargetProvider

class Target:

    PREVIEW_SIZE = (200, 200)

    def __init__(self, callback, setter=None, frame_provider=None):
        self._callback = callback
        self._setter = setter
        self._running = False
        self._frame_provider = frame_provider if frame_provider is not None else RandomTargetProvider("")
        self._pr = None

        self._lock = threading.Lock()
        self._homography = None
        self._img_original = None
        self._img_tarnsform = None
        self._img_marked = None
        # last detection point
        self._last_point = (-1, -1)
    
    def set_setter(self, setter):
        self._setter = setter

    def start(self):
        self._running = True
        self._pr = threading.Thread(target=self._run)
        self._pr.start()

    def _run(self):
        while self._running:
            frame = self._frame_provider.get_frame()
            if frame is None:
                time.sleep(0.1)
                continue

            transformed = aruco_transform(frame, self._homography) if self._homography is not None else frame
            marked = transformed

            dot_position = get_contour_center(frame)
            
            self._last_point = dot_position
            self._callback(dot_position)
            try:
                self._setter(dot_position)
            except Exception:
                pass

            if dot_position and dot_position != (-1, -1):
                cv2.circle(marked, dot_position, 6, (0, 255, 0), -1)

            with self._lock:
                self._img_original = frame
                self._img_tarnsform = transformed
                self._img_marked = marked

            time.sleep(0.01)


    def find_homography(self):
        frame = self._frame_provider.get_frame()
        if frame is None:
            return
        self._homography = aruco_get_transformation(frame)
        
    
    def get_image(self, image_type):
        """
        Returns (image_bytes, mime_type) for requested image_type:
        - IMAGE_TYPE_ORIGINAL
        - IMAGE_TYPE_TRANSFORM
        - IMAGE_TYPE_MARK
        If no image available, returns (b'', '').
        image_type is expected as int (enum value).
        """
        with self._lock:
            image_to_send = None
            if image_type == 1:  # IMAGE_TYPE_ORIGINAL
                image_to_send = None if self._img_original is None else self._img_original.copy()
            elif image_type == 2:  # IMAGE_TYPE_TRANSFORM
                image_to_send = None if self._img_tarnsform is None else self._img_tarnsform.copy()
            elif image_type == 3:  # IMAGE_TYPE_MARK
                image_to_send = None if self._img_marked is None else self._img_marked.copy()
            else:
                return b'', ''
            if image_to_send is None:
                return b'', ''
            ok, buf = cv2.imencode('.png', image_to_send)
            if not ok:
                return b'', ''
            return buf.tobytes(), 'image/png'
