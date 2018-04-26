class IndexedHeap:
    """ Binary heap with O(logn) delition at random position """

    def __init__(self):
        self._heap = []
        self._index = {}

    def peek(self):
        return self._heap[0]

    def has(self, value):
        return value in self._index

    def size(self):
        return len(self._heap)

    def push(self, value):
        self._heap.append(value)
        self._sinkdown(len(self._heap) - 1)

    def pop(self):
        lastelt = self._heap.pop()
        if self._heap:
            smallest = self._heap[0]
            self._heap[0] = lastelt
            self._sinkup(0)
            del self._index[smallest]
            return smallest

        del self._index[lastelt]
        return lastelt

    def decrease(self, value, min_value):
        if min_value >= value:
            raise Exception('min_value must be strictly less than value')

        pos = self._index[value]
        self._heap[pos] = min_value
        self._sinkdown(pos)

    def _sinkdown(self, pos):
        newitem = self._heap[pos]
        parentpos = (pos - 1) // 2
        while pos > 0 and newitem < self._heap[parentpos]:
            self._heap[pos] = self._heap[parentpos]
            self._index[self._heap[parentpos]] = pos
            pos = parentpos
            parentpos = (pos - 1) // 2

        self._heap[pos] = newitem
        self._index[newitem] = pos

    def _sinkup(self, pos):
        newitem = self._heap[pos]
        lastpos = len(self._heap) - 1
        childpos = 2 * pos + 1
        while childpos <= lastpos:
            smallest = self._heap[childpos]
            rightchildpos = childpos + 1
            if rightchildpos <= lastpos and smallest > self._heap[rightchildpos]:
                childpos = rightchildpos
                smallest = self._heap[childpos]

            if smallest >= newitem:
                break

            self._heap[pos] = smallest
            self._index[smallest] = pos
            pos = childpos
            childpos = 2 * pos + 1

        self._heap[pos] = newitem
        self._index[newitem] = pos


if __name__ == '__main__':
    h = IndexedHeap()
    h.push(3)
    h.push(5)
    h.push(4)
    h.push(6)
    print(h.pop())
    print(h.pop())
    print(h.pop())
    print(h.pop())
    
    h = IndexedHeap()
    h.push(100)
    h.push(200)
    h.push(150)
    h.push(175)
    h.push(250)

    print(h.pop())
    print(h.pop())
    print(h.pop())

    h.decrease(250, -1)
    print(h.pop())

