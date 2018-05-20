class Node:
    def __init__(self, val):
        self.val = val
        self.count = 1
        self.height = 1
        self.size = 1
        self.left = None
        self.right = None

    def balance(self):
        return self.height_left() - self.height_right()

    def height_left(self):
        return self.left.height if self.left else 0

    def height_right(self):
        return self.right.height if self.right else 0

    def size_left(self):
        return self.left.size if self.left else 0

    def size_right(self):
        return self.right.size if self.right else 0

def count_greater_than(root, val):
    if not root:
        return 0

    if root.val == val:
        return root.size_right()

    if root.val > val:
        return root.count + root.size_right() + count_greater_than(root.left, val)

    if root.val < val:
        return count_greater_than(root.right, val)

def insert_node(root, val):
    if not root:
        return Node(val)

    if val == root.val:
        root.count += 1
    elif val < root.val:
        root.left = insert_node(root.left, val)
    else:
        root.right = insert_node(root.right, val)

    root.height = 1 + max(root.height_left(), root.height_right())
    root.size = root.count + root.size_left() + root.size_right()

    balance = root.balance()

    if balance > 1 and root.left.val > val:
        return rotate_right(root)
    if balance < -1 and root.right.val < val:
        return rotate_left(root)
    if balance > 1:
        root.left = rotate_left(root.left)
        return rotate_right(root)
    if balance < -1:
        root.right = rotate_right(root.right)
        return rotate_left(root)

    return root

def rotate_left(node):
    root = node.right

    tmp = root.left
    root.left = node
    node.right = tmp

    node.height = 1 + max(node.height_left(), node.height_right())
    node.size = node.count + node.size_left() + node.size_right()
    root.height = 1 + max(root.height_left(), root.height_right())
    root.size = root.count + root.size_left() + root.size_right()

    return root

def rotate_right(node):
    root = node.left

    tmp = root.right
    root.right = node
    node.left = tmp

    node.height = 1 + max(node.height_left(), node.height_right())
    node.size = node.count + node.size_left() + node.size_right()
    root.height = 1 + max(root.height_left(), root.height_right())
    root.size = root.count + root.size_left() + root.size_right()

    return root

def print_dot(tree):
    import sys

    def traverse(node, level=0, offset=0):
        if node is None:
            return

        node_id = 'n{}'.format(id(node))
        label = '{} ({},{})'.format(node.val, level, offset)
        graph.append('{} [label="{}"];'.format(node_id, label))

        if node.left:
            left_node_id = 'n{}'.format(id(node.left))
            traverse(node.left, level=level + 1, offset=offset)
            graph.append(node_id + ' -> ' + left_node_id + ';')

        if node.right:
            right_node_id = 'n{}'.format(id(node.right))
            traverse(node.right, level=level + 1, offset=offset + 1)
            graph.append(node_id + ' -> ' + right_node_id + ';')

    graph = ['digraph G {']
    traverse(tree)
    graph.append('}')
    graph.append('')

    print('\n'.join(graph), file=sys.stderr)


# test facilities
def height(root):
    if not root:
        return 0
    return 1 + max(height(root.left), height(root.right))

def is_balanced(root):
    if not root:
        return True
    return (
        abs(height(root.left) - height(root.right)) <= 1 
        and is_balanced(root.left) 
        and is_balanced(root.right)
    )

def walk_inorder(root):
    if not root:
        return []
    return walk_inorder(root.left) + [root.val] + walk_inorder(root.right)

def test_insertion(fixture):
    root = None
    for x in fixture:
        root = insert_node(root, x)
        # print_dot(root)

    assert root
    assert is_balanced(root)
    assert set(walk_inorder(root)) == set(fixture)

if __name__ == '__main__':
    test_insertion([988, 857, 744, 492, 228, 366, 860, 937, 433, 552, 438, 229, 276, 408, 475])
    test_insertion([859, 396, 30, 238, 236, 794, 819, 429, 144, 12, 929, 
                    530, 777, 405, 444, 764, 614, 539, 607, 841, 905, 819])
    print('ok')

