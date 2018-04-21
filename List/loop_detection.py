class Node:
    def __init__(self, value, nxt=None):
        self.value = value
        self.nxt = nxt

def has_loop(l):
    cursor1 = l
    cursor2 = l.nxt
    while cursor1 and cursor2:
        if cursor1 is cursor2:
            return True

        cursor1 = cursor1.nxt
        cursor2 = cursor2.nxt.nxt if cursor2.nxt else None
   
    return False

def main():
    l1 = Node(0, Node(1, Node(2, Node(3, Node(4, Node(5))))))
    print(has_loop(l1))

    x = Node(6)
    l2 = Node(0, Node(1, Node(2, Node(3, Node(4, Node(5, x))))))
    x.nxt = Node(7, Node(8, Node(9, Node(10, x))))
    print(has_loop(l2))
    
if __name__ == '__main__':
    main()

