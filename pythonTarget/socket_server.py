import asyncio
import os
import ssl
from typing import Optional, Awaitable

import tornado
import tornado.ioloop
import tornado.httpserver
import tornado.web
import tornado.websocket
import uuid

LISTEN_PORT = 8000
LISTEN_ADDRESS = '127.0.0.1'

data_dir = os.path.dirname(os.path.realpath(__file__))


class EchoWebSocket(tornado.websocket.WebSocketHandler):
    listeners = dict()

    def data_received(self, chunk: bytes) -> Optional[Awaitable[None]]:
        pass

    def check_origin(self, origin):
        return True

    def open(self):
        self.id_ = uuid.uuid4().hex
        EchoWebSocket.listeners[self.id_] = self
        print("WebSocket opened")

    def on_message(self, message):
        pass

    def on_close(self):
        print("WebSocket closed")
        EchoWebSocket.listeners.pop(self.id_, None)


class SocketServer:

    def __init__(self):
        self.io_loop = None

    def start(self):
        asyncio.set_event_loop(asyncio.new_event_loop())
        tornado.ioloop.IOLoop.configure("tornado.platform.asyncio.AsyncIOLoop")
        self.io_loop = tornado.ioloop.IOLoop.current()
        asyncio.set_event_loop(self.io_loop.asyncio_loop)

        # Create tornado application and supply URL routes
        application = tornado.web.Application([(r'/ws', EchoWebSocket)])

        # Setup HTTP Server
        # http_server = tornado.httpserver.HTTPServer(application)
        ssl_options = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
        ssl_options.options |= ssl.OP_NO_TLSv1_3
        # print(ssl_options.options)
        ssl_options.load_cert_chain(os.path.join(data_dir, 'localhost.crt'), os.path.join(data_dir, 'localhost.key'))
        http_server = tornado.httpserver.HTTPServer(application, ssl_options=ssl_options)

        http_server.listen(LISTEN_PORT, LISTEN_ADDRESS)
        # Start IO/Event loop
        tornado.ioloop.IOLoop.instance().start()

    @staticmethod
    def send(message):
        if len(EchoWebSocket.listeners) == 0:
            print('No clients connected')
        else:
            for _, v in EchoWebSocket.listeners.items():
                v.write_message(message)

    def send_message(self, message):
        self.io_loop.asyncio_loop.call_soon_threadsafe(self.send, message)
