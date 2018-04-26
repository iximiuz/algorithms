from collections import deque
from indexed_heap import IndexedHeap

class MovingMedian:    
    def __init__(self, window_size=-1):
        self.window_size = window_size
        self.median = 0
        self.iter = 0
        self.history = deque() if window_size > 0 else None
        self.lower = IndexedHeap()
        self.upper = IndexedHeap()
        
    def update(self, v):                
        if not self.upper.size():
            self.upper.push((v, 0))
            if self.window_size > 0:
                self.history.append((v, 0))
            self.median = v
            return

        self.iter += 1

        if self.window_size > 0 and len(self.history) == self.window_size:
            oldest, i = self.history.popleft()
            if self.lower.has((-oldest, i)):
                self.lower.decrease((-oldest, i), (float('-inf'),) * 2)
                self.lower.pop()
            elif self.upper.has((oldest, i)):
                self.upper.decrease((oldest, i), (float('-inf'),) * 2)
                self.upper.pop()
            else:
                raise Exception('oldest value not found {}'.format(oldest))

        if v > self.upper.peek()[0]:
            self.upper.push((v, self.iter))
            if self.window_size > 0:
                self.history.append((v, self.iter))
        else:
            self.lower.push((-v, self.iter))
            if self.window_size > 0:
                self.history.append((v, self.iter))

        if self.upper.size() - self.lower.size() > 1:
            item = self.upper.pop()
            self.lower.push((-item[0], item[1]))

        if self.lower.size() - self.upper.size() > 1:
            item = self.lower.pop()
            self.upper.push((-item[0], item[1]))

        if self.upper.size() > self.lower.size():
            self.median = self.upper.peek()[0]
        elif self.lower.size() > self.upper.size():
            self.median = -self.lower.peek()[0]
        else:
            self.median = (-self.lower.peek()[0] + self.upper.peek()[0]) / 2.0


if __name__ == "__main__":
    m = MovingMedian()
    h = []
    for x in range(32):
        h.append(x)
        m.update(x)
        print(h, m.median)

    m = MovingMedian(5)
    h = []
    for x in range(32):
        h.append(x)
        h = h[-5:]
        m.update(x)
        print(h, m.median)

