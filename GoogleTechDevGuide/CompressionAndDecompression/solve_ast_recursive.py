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

def _parse_group(s, pos):
    assert pos < len(s)
    assert s[pos] == '['

    pos += 1
    start = pos
    counter = 1
    while counter > 0:
        if s[pos] == '[':
            counter += 1
        if s[pos] == ']':
            counter -= 1
        pos += 1
    return s[start:pos - 1], pos

def ast(s, pos=-1):
    if pos == -1:
        return ['+', '', ast(s, 0)]

    if pos >= len(s):
        return ''

    if '[' == s[pos]:
        _, pos = _parse_group(s, pos)
        return ['+', '', ast(s, pos)]

    if _is_digit(s[pos]):
        mul, pos = _parse_num(s, pos)
        group, pos = _parse_group(s, pos)
        return ['+', ['*', mul, ast(group)], ast(s, pos)]

    if _is_alpha(s[pos]):
        subs, pos = _parse_str(s, pos)
        return ['+', subs, ast(s, pos)]

    raise Exception('Unreachable')

def traverse(node):
    if not isinstance(node, list):
        return node

    op, l, r = node
    if op == '*':
        return l * traverse(r)
    if op == '+':
        return traverse(l) + traverse(r)

    raise Exception('Unreachable')

def solve_ast_recursive(s):
    return traverse(ast(s))

if __name__ == '__main__':
    print(solve_ast_recursive('3[abc]4[ab]c'))  # abcabcabcababababc
    print(solve_ast_recursive('2[3[a]b]'))  # aaabaaab
    print(solve_ast_recursive('a[]b'))  # ab
    print(solve_ast_recursive('0[abc]'))  # <empty>

