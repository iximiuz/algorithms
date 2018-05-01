class InterleavedIteration:
    def __init__(self, iterlist):
        first = prev = last = [None, None]
        for i in iterlist:
            prev = last
            last[0] = i
            last[1] = last = [None, None]

        prev[1] = first  # make a cycle

        self.current = first
        self.prev = prev

    def __iter__(self):
        return self

    def __next__(self):
        # test an edge case when provided iterlist is empty
        if self.current[0] is None:
            raise StopIteration()

        try:
            rv = next(self.current[0])
            self.prev = self.current
            self.current = self.current[1]
            return rv
        except StopIteration:
            if self.current is self.prev:
                raise StopIteration()

            self.current = self.prev[1] = self.current[1]
            return self.__next__()

    def next(self):
        return self.__next__()
        
def main():
    # []
    print([x for x in InterleavedIteration([])])

    # [0, 1, 2, 3, 4]
    print([x for x in InterleavedIteration([iter(range(5))])])

    # [0, 20, 0, 100, 21, 1, 101, 22, 2, 102, 23, 3, 103, 24, 4, 104, 25, 105, 26, 
    #  106, 27, 107, 28, 108, 29, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119]
    ii = InterleavedIteration([
        iter(range(1)),
        iter(range(20, 30)),
        iter(range(5)),
        iter(range(100, 120)),
    ])
    print([x for x in ii])

if __name__ == '__main__':
    main()

