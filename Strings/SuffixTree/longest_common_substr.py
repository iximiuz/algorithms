class Node:
    __slots__ = ['word', 'start', 'end', 'children']

    def __init__(self, word, start, end, children=None):
        self.word = word
        self.start = start
        self.end = end
        self.children = children or {}

    def __str__(self):
        return self._str(0)

    def _str(self, ident):
        children = ''.join(c._str(ident+1) for c in self.children.values())
        return '{ident}{text} {start}:{end}\n{children}'.format(ident=' '*4*ident,
                                                                text=self.word[self.start:self.end+1],
                                                                start=self.start,
                                                                end=self.end,
                                                                children=children)

def suffix_tree(words):
    root = Node('<root>', -1, -1)
    for w in words:
        n = len(w)
        for start in range(n):
            node = root
            # print(node)
            pos = start
            # print(' > ADD', w[start:])
            while pos < n:
                char = w[pos]
                if char not in node.children:
                    node.children[char] = Node(w, pos, n-1)
                    break

                node = node.children[char]
                matches = 0
                for i in range(node.start, node.end+1):
                    if w[pos] != node.word[i]:
                        break
                    pos += 1
                    matches += 1
                # print('node', node, 'has', matches, 'matches with word', w[pos-matches:], '\n')

                if matches < node.end - node.start + 1:
                    # if matches == 1:
                    #    print(w, pos)
                    #    print(node.word, node.start + matches)
                    node.children = {
                        w[pos]: Node(w, pos, n-1),
                        node.word[node.start + matches]: Node(node.word,
                                                              node.start + matches,
                                                              node.end,
                                                              node.children),
                    }
                    node.end = node.start + matches - 1
                    break
    return root

def find_lca(node, depth, lca):
    depth += node.end - node.start + 1
    if not node.children:
        return {node.word[node.end]}

    wids = set()
    for child in node.children.values():
        wids |= find_lca(child, depth, lca)
    # print(wids, depth)
    if len(wids) == 2 and depth > lca[0]:
        # print(node.word[node.start:node.end+1])
        lca[0] = depth
    return wids

def lcs(a, b):
    ans = [0]
    tree = suffix_tree([a+'#', b+'$'])
    find_lca(tree, 0, ans)
    return ans[0] - 1

if __name__ == '__main__':
    print(lcs('ABCDGH', 'ACDGHR'))
    print(lcs('ABC', 'AC'))

