# python3

from collections import deque

class TreeNode:
    __slots__ = ['start', 'end', 'children']
    def __init__(self, start, end, children=None):
        self.start = start
        self.end = end
        self.children = children or {}
        # print('  > new node', t[start:end+1])

    def __str__(self):
        stack = [(0, self)]
        out = []
        while stack:
            ident, node = stack.pop()
            out.append('{}{} {}:{}'.format('    '*ident, t[node.start:node.end+1], node.start, node.end))
            for child in node.children.values():
                stack.append((ident + 1, child))
        return '\n'.join(out)

def suffix_tree(text):
    root = TreeNode(0, 0)
    n = len(text)
    for start_pos in range(n):
        node = root
        # print('\n\nadd [' + text[start_pos:] + '] to\n', root)
        pos = start_pos
        while pos < n:
            # print('subadd [' + text[pos:] + '] to\n', node)
            char = text[pos]
            if char not in node.children:  
                node.children[char] = TreeNode(pos, n-1)
                # print('new child {}:{}'.format(pos, n-1))
                break
                
            node = node.children[char]
            # print('matching [' + text[pos:] + '] with\n', node)

            nomatches = 0
            nodelen = node.end - node.start + 1
            need_split = False
            while not need_split and nomatches < nodelen:
                if text[node.start+nomatches] == text[pos]:
                    nomatches += 1
                    pos += 1
                else:
                    need_split = True
            # print('nomatches', nomatches)

            if need_split:
                node_end_old = node.end
                node.end = node.start+nomatches-1
                node.children = {
                        text[node.end+1]: TreeNode(node.end+1, node_end_old, node.children),
                        text[pos]: TreeNode(pos, n-1)
                }
                # print('splitted')
                break

    return root

def tree2str(text, tree):
    q = deque(tree.children.values())
    out = []
    while q:
        node = q.popleft()
        out.append(text[node.start:node.end+1])
        for child in node.children.values():
            q.append(child)
    return out

def testit(t, expected):
    assert sorted(tree2str(t, suffix_tree(t))) == expected.split()

if __name__ == '__main__':
    testit('A$', '$ A$')
    testit('ACA$', '$ $ A CA$ CA$')
    testit('AAA$', '$ $ $ A A A$')
    testit('ATAAATG$', '$ A A AAATG$ AAATG$ ATG$ G$ G$ G$ T T TG$')
    testit('ACACAA$', '$ $ A A$ A$ A$ CA CA CAA$ CAA$')

