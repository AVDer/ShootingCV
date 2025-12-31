from proto import target_pb2_grpc
from proto import target_pb2

from queue import Empty

class TargetService(target_pb2_grpc.TargetServicer):

    def __init__(self, manager, target = None):
        super().__init__()
        self._manager = manager
        self._x = 27
        self._y = 42
        self._target = target

    def SetPosition(self, position, target):
        self._x = position.x
        self._y = position.y
        return target_pb2.Empty()
    
    def set_point(self, point):
        self._x = point[0]
        self._y = point[1]

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


    def AdjustImage(self, request, context):
        """
        request: Empty
        returns: None
        """
        if self._target is not None:
            self._target.find_homography()
        return target_pb2.Empty()


    def GetImage(self, request, context):
        """
        request: GetImageRequest with enum field image_type
        returns: GetImageResponse with bytes and mime_type string
        """
        if self._target is None:
            return target_pb2.GetImageResponse(image_data=b'', mime_type='')

        # request.image_type is an enum value (int)
        image_bytes, mime = self._target.get_image(request.image_type)
        return target_pb2.GetImageResponse(image_data=image_bytes, mime_type=mime)