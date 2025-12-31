import cv2
import grpc

from concurrent import futures 
from target_service import TargetService

from proto import target_pb2_grpc

from stream_manager import StreamManager
from target import Target

from target_provider import VideoTargetProvider
from target_provider import ImageTargetProvider
from target_provider import RandomTargetProvider

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    stream_manager = StreamManager()

    # target_provider = VideoTargetProvider("")
    target_provider = ImageTargetProvider("./pythonTarget/target.jpg")
    #target_provider = RandomTargetProvider("")

    detector = Target(callback=stream_manager.broadcast, setter=None, frame_provider=target_provider)

    target_service = TargetService(stream_manager, detector)

    detector.set_setter(target_service.set_point)

    target_pb2_grpc.add_TargetServicer_to_server(target_service, server)
    server.add_insecure_port("[::]:50051")
    server.start()

    detector.start()

    server.wait_for_termination()
    

if __name__ == "__main__":
    print(f"OpenCV version:\t{cv2.__version__}")
    serve()