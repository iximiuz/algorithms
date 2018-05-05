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

def solve_recursive(s, pos=0):
    if pos >= len(s):
        return ''

    if '[' == s[pos]:
        _, pos = _parse_group(s, pos)
        return solve_recursive(s, pos)

    if _is_digit(s[pos]):
        mul, pos = _parse_num(s, pos)
        group, pos = _parse_group(s, pos)
        return mul * solve_recursive(group) + solve_recursive(s, pos)

    if _is_alpha(s[pos]):
        prefix, pos = _parse_str(s, pos)
        return prefix + solve_recursive(s, pos)
        
    raise Exception('Unreachable! s={}, pos={}'.format(s, pos))

if __name__ == '__main__':
    print(solve_recursive('3[abc]4[ab]c'))  # abcabcabcababababc
    print(solve_recursive('2[3[a]b]'))  # aaabaaab
    print(solve_recursive('a[]b'))  # ab
    print(solve_recursive('0[abc]'))  # <empty>

