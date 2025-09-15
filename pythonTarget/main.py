import cv2
import grpc

from concurrent import futures 
from target_service import TargetService

from proto import target_pb2_grpc

from stream_manager import StreamManager
from target import Target

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    stream_manager = StreamManager()

    target_service = TargetService(stream_manager)

    target_pb2_grpc.add_TargetServicer_to_server(target_service, server)
    server.add_insecure_port("[::]:50051")
    server.start()

    detector = Target(callback=stream_manager.broadcast, setter=target_service.set_point)
    detector.start()

    server.wait_for_termination()

if __name__ == "__main__":
    print(f"OpenCV version:\t{cv2.__version__}")
    serve()