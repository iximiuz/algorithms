def _is_alpha(c):
    return 'a' <= c <= 'z' or 'A' <= c <= 'Z'

def _is_digit(c):
    return '0' <= c <= '9'

def _parse_num(s, pos):
    assert pos < len(s)
    assert _is_digit(s[pos])

    accum = 0
    while pos < len(s) and _is_digit(s[pos]):
        accum = 10 * accum + ord(s[pos]) - ord('0')
        pos += 1
    return accum, pos

def _parse_str(s, pos):
    assert pos < len(s)
    assert _is_alpha(s[pos])

    start = pos
    while pos < len(s) and _is_alpha(s[pos]):
        pos += 1
    return s[start:pos], pos

def make_ast(s):
    pos = 0
    root_node = cur_node = ['+']
    parents = []

    while pos < len(s):
        if '[' == s[pos]:
            pos += 1
            node = ['+']
            cur_node.append(node)
            parents.append(cur_node)
            cur_node = node
            continue

        if ']' == s[pos]:
            pos += 1
            parents.pop()
            cur_node = parents.pop()
            continue

        if _is_digit(s[pos]):
            mul, pos = _parse_num(s, pos)
            node = ['*', mul]
            cur_node.append(node)
            parents.append(cur_node)
            cur_node = node
            continue

        if _is_alpha(s[pos]):
            subs, pos = _parse_str(s, pos)
            cur_node.append(subs)
            continue

    return root_node

def solve_ast_iterative(s):
    ast = make_ast(s)
    operands = []
    fringe = [ast]
    while fringe:
        node = fringe.pop()
        # TODO: ...

if __name__ == '__main__':
    print(solve_ast_iterative('3[abc]4[ab]c'))  # abcabcabcababababc
    print(solve_ast_iterative('2[3[a]b]'))  # aaabaaab

