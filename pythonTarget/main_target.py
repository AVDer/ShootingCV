
import cv2
import threading
from http.server import HTTPServer
from functools import partial

from socket_server import SocketServer
from image_server import ImageServer
from target import Target
from target_provider import ImageTargetProvider
# from target_provider import VideoTargetProvider

target = Target(ImageTargetProvider('mark1_target.jpg'))

# target = Target(VideoTargetProvider('0'))

server = SocketServer()


def server_function():
    server.start()

def image_server_function():
    image_server = partial(ImageServer, target)
    with HTTPServer(('', 8001), image_server) as server:
        server.serve_forever()

wss_thread = threading.Thread(target=server_function)
wss_thread.start()
https_thread = threading.Thread(target=image_server_function)
https_thread.start()

target.load()
target.detect_center()
target.detect_7()

# target.set_pixel(target.get_center(), (0, 0, 255))
# target.add_mark(target.get_center(), (0, 0, 255))

while True:
    target.next_frame()

    target.detect_center()
    target.detect_7()

    ray = target.detect_ray()

    target.add_grid()
    target.show()

    if ray:
        #print(ray)
        server.send_message(ray)
        pass

    if cv2.waitKey(20) & 0xFF == 27:
        break

cv2.destroyAllWindows()
