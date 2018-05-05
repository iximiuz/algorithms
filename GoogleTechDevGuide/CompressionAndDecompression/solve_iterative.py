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

def solve_iterative(s):
    pos = 0
    muls = []
    strs = []
    while pos < len(s):
        if '[' == s[pos]:
            pos += 1
            continue

        if ']' == s[pos]:
            strs.append(muls.pop() * strs.pop())
            pos += 1
            continue

        if _is_digit(s[pos]):
            mul, pos = _parse_num(s, pos)
            muls.append(mul)
            strs.append('')

        if _is_alpha(s[pos]):
            subs, pos = _parse_str(s, pos)
            strs[-1] += subs
        
    return ''.join(strs)

if __name__ == '__main__':
    print(solve_iterative('3[abc]4[ab]c'))  # abcabcabcababababc
    print(solve_iterative('2[3[a]b]'))  # aaabaaab

