from collections import deque


class SimpleBST:
    """ Naive (non-balanced) binary search tree """

    class Node:
        def __init__(self, key, value=None):
            self.key = key
            self.value = value
            self.left = None
            self.right = None

    def __init__(self):
        self.root = None

    def __str__(self):
        result = []
        for parent, node in self.traversal_levelorder():
            if parent is not None:
                result.append('%s->%s' % (parent, node))
        return ' '.join(result)

    def contains(self, key):
        return self._find(self.root, key) is not None

    def find(self, key):
        node = self._find(self.root, key)
        return node.value if node else None

    def is_empty(self):
        return self.root is None

    def insert(self, key, value=None):
        self.root = self._insert(self.root, self.Node(key, value=value))

    def range(self, lo, hi):
        range_root = self._range(self.root, lo, hi)
        for node in self._traversal_inorder(range_root, lo=lo, hi=hi):
            yield node.value

    def traversal_inorder(self):
        return self._traversal_inorder(self.root)

    def traversal_preorder(self):
        while False:
            yield None

    def traversal_postorder(self):
        while False:
            yield None

    def traversal_levelorder(self):        
        if self.is_empty():
            return

        queue = deque([(None, self.root)])
        while queue:
            parent, node = queue.popleft()
            yield parent.key if parent else None, node.key

            if node.left:
                queue.append((node, node.left))

            if node.right:
                queue.append((node, node.right))

    def _find(self, root, key):
        if root is None:
            return None

        if root.key > key:
            return self._find(root.left, key)

        if root.key < key:
            return self._find(root.right, key)            

        return root

    def _insert(self, root, node):
        if root is None:
            return node

        if node.key < root.key:
            root.left = self._insert(root.left, node)
        else:
            root.right = self._insert(root.right, node)

        return root

    def _range(self, root, lo, hi):
        if root is None:
            return None

        if root.key > hi:
            return self._range(root.left, lo, hi)

        if root.key < lo:
            return self._range(root.right, lo, hi)        

        return root

    def _traversal_inorder(self, root, lo=None, hi=None):
        if root is not None:
            for node in self._traversal_inorder(root.left, lo=lo, hi=hi):
                if lo is not None and lo > node.key:
                    break
                yield node

            yield root

            for node in self._traversal_inorder(root.right, lo=lo, hi=hi):
                if hi is not None and hi < node.key:
                    break
                yield node


if __name__ == '__main__':
    from random import randint

    def _range(values, lo, hi):
        values = sorted(values)
        for v in values:
            if v > hi:
                break

            if v >= lo:
                yield v


    # fixtures = [randint(0, 100) for _ in range(20)]
    fixtures = [95, 80, 96, 22, 88, 11, 62, 19, 27, 30]*2
    inserted = []
    bst = SimpleBST()
    for v in fixtures[:len(fixtures)//2]:
        bst.insert(v, value=v)
        inserted.append(v)

    print(bst)

    for v in inserted:
        assert bst.contains(v)

    for v in set(fixtures) - set(inserted):
        assert not bst.contains(v)

    for _ in range(100):
        # lo, hi = sorted((randint(0, 100), randint(0, 100)))
        lo, hi = 13, 46
        expected = list(_range(inserted, lo, hi))
        actual = list(bst.range(lo, hi))
        assert actual == expected, '%s != %s, lo=%s, hi=%s' % (actual, expected, lo, hi)
