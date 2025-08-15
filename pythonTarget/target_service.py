from proto import target_pb2_grpc
from proto import target_pb2

from queue import Empty

class TargetService(target_pb2_grpc.TargetServicer):

    def __init__(self, manager):
        super().__init__()
        self._manager = manager
        self._x = 27
        self._y = 42

    def SetPosition(self, position, target):
        self._x = position.x
        self._y = position.y
        return target_pb2.Empty()

    def GetPosition(self, empty, target):
        return target_pb2.Position(x = self._x, y = self._y)
    
    def StreamPosition(self, request, context):
        q = self._manager.subscribe()
        try:
            while True:
                try:
                    value = q.get(timeout=5)
                    yield target_pb2.Position(
                        x = value[0],
                        y = value[1]
                    )
                except Empty:
                    continue
        finally:
            self._manager.unsubscribe(q)
