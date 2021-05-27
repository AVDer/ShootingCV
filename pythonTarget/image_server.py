import shutil
from http.server import BaseHTTPRequestHandler


class ImageServer(BaseHTTPRequestHandler):

    def __init__(self, target, *args, **kwargs):
        self.target_ = target
        super().__init__(*args, **kwargs)

    def do_GET(self):
        if self.path == '/original.png' or self.path == '/blurry.png' or self.path == '/augmented.png':
            self.target_.store_image(self.path[1:-4])
            self.send_response(200)
            self.send_header('Content-type', 'image/jpeg')
            self.end_headers()
            with open('out_image.png', 'rb') as content:
                shutil.copyfileobj(content, self.wfile)
        else:
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            message = "Hello, World!"
            self.wfile.write(bytes(message, "utf8"))
