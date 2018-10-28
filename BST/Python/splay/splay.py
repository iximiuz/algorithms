"""
  The idea behind the Splay tree is:
    - keep doing left/right rotations in order to bring the last
      accessed node to the root of the tree. 
"""

class Node:
    def __init__(self, key, parent=None, left=None, right=None):
        self.key = key
        self.parent = parent
        self.left = left
        self.right = right

def find(root, key):
    node = _find(root, key)
    if node:
        root = _splay(node)
    return root, node and node.key == key

def insert(root, key):
    root = _insert(root, key)
    return find(root, key)

def delete(root, key):
    left, right = split(root, key)
    if right and right.key == key:
        return merge(left, right.right), True
    return merge(left, right), False

def split(root, key):
    if not root:
        return None

    node, _ = find(root, key)
    t1 = node.left
    if t1:
        t1.parent = None

    t2 = node
    t2.left = None
    t2.parent = None

    return t1, t2

def merge(t1, t2):
    """ each keysof(t1) < each keysof(t2) """
    if t2 is None:
        return t1
    if t1 is None:
        return t2

    t1 = _splay(_find_max(t1))
    t1.right = t2
    t2.parent = t1
    return t1

def height(root):
    h = 0
    stack = [(0, root)]
    while stack:
        d, node = stack.pop()
        if node is None:
            continue
        stack.append((d + 1, node.left))
        stack.append((d + 1, node.right))
        h = max(h, d + 1)
    return h

def traverse_inorder(root):
    stack = [(0, root)]
    lst = []
    while stack:
        typ, val = stack.pop()
        if val is None:
            continue
        if typ == 1:
            lst.append(val)
        else:
            stack.append((0, val.right))
            stack.append((1, val.key))
            stack.append((0, val.left))
    return lst

def _find(root, key):
    node = root
    while True:
        if node.key > key:
            if node.left is None:
                break
            else:
                node = node.left
        elif node.key < key:
            if node.right is None:
                break
            else:
                node = node.right
        else:
            break
    return node

def _find_max(root):
    node = root
    while node and node.right:
        node = node.right
    return node

def _insert(root, key):
    if root is None:
        root = Node(key)
    else:
        node = _find(root, key)
        if key < node.key:
            node.left = Node(key, node)
        elif key > node.key:
            node.right = Node(key, node)
    return root

def _splay(node):
    if node.parent is None:
        return node

    x = node
    p = x.parent
    g = p.parent
    while g is not None:
        if x is p.left:
            if p is g.left:
                _rotate_right(g)
                _rotate_right(p)
            else:
                _rotate_right(p)
                _rotate_left(g)
        else:
            if p is g.right:
                _rotate_left(g)
                _rotate_left(p)
            else:
                _rotate_left(p)
                _rotate_right(g)
        p = x.parent
        g = p.parent if p is not None else None

    if x.parent is not None:
        if x == x.parent.left:
            _rotate_right(x.parent)
        else:
            _rotate_left(x.parent)

    return x

def _rotate_right(node):
    g = node.parent

    new_root = node.left
    tmp = new_root.right
    new_root.right = node
    node.left = tmp

    if g:
        if g.left is node:
            g.left = new_root
        else:
            g.right = new_root

    new_root.parent = g
    node.parent = new_root
    if tmp:
        tmp.parent = node

def _rotate_left(node):
    g = node.parent

    new_root = node.right
    tmp = new_root.left
    new_root.left = node
    node.right = tmp

    if g:
        if g.left is node:
            g.left = new_root
        else:
            g.right = new_root

    new_root.parent = g
    node.parent = new_root
    if tmp:
        tmp.parent = node

def _t1():
    t, ok = insert(None, 100)
    assert ok
    assert t.key == 100
    assert t.parent is None
    assert t.left is None
    assert t.right is None
    print('OK')

def _t2():
    t, ok = insert(insert(None, 100)[0], 200)
    assert ok
    assert t.key == 200
    assert t.parent is None
    assert t.left.key == 100
    assert t.right is None
    print('OK')

def _t3():
    t1 = Node(100)
    t2 = Node(200)

    t1.right = t2
    t2.parent = t1

    t, ok = find(t1, 200)
    assert ok
    assert t.key == 200
    assert t.parent is None
    assert t.left.key == 100
    assert t.right is None
    print('OK')

def _t4():
    t = None
    for i in range(100):
        t, _ = insert(t, i)
        assert t.key == i
        assert height(t) == i + 1

    assert height(t) == 100  # worst case, O(n)

    t1, t2 = split(t, 50)
    assert height(t1) == 50
    assert height(t2) == 27

    assert list(range(50)) == traverse_inorder(t1)
    assert list(range(50, 100)) == traverse_inorder(t2)
    assert list(range(100)) == traverse_inorder(merge(t1, t2))
    print('OK')

def _t5():
    heights = [1, 2, 2, 3, 4, 4, 4, 6, 6, 8, 9, 6, 6, 5, 5, 6, 6]
    t = None
    for i, x in enumerate((1, 8, 7, 5, 2, 3, 4, 9, 6, 10, 13, 1, 8, 5, 4, 3, 14)):
        t, _ = insert(t, x)
        assert t.key == x
        assert height(t) == heights[i]
    print('OK')

def _t6():
    t = None
    for i in range(10):
        t, _ = insert(t, i)

    assert list(range(10)) == traverse_inorder(t)

    t, ok = delete(t, 42)
    assert not ok
    assert list(range(10)) == traverse_inorder(t)

    t, ok = delete(t, 9)
    assert ok
    assert list(range(9)) == traverse_inorder(t)

    t, ok = delete(t, 0)
    assert ok
    assert list(range(1, 9)) == traverse_inorder(t)
    print('OK')

if __name__ == '__main__':
    _t1()
    _t2()
    _t3()
    _t4()
    _t5()
    _t6()

