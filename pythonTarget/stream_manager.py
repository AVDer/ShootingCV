import threading
import queue

class StreamManager:
    def __init__(self):
        self._lock = threading.Lock()
        self._subscribers = set()

    def subscribe(self):
        q = queue.Queue()
        with self._lock:
            self._subscribers.add(q)
        return q

    def unsubscribe(self, q):
        with self._lock:
            self._subscribers.discard(q)

    def broadcast(self, value):
        with self._lock:
            for q in self._subscribers:
                q.put(value)