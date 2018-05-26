from collections import defaultdict

def is_square(words):
    n = len(words)
    word_lens = set(len(w) for w in words)
    if len(word_lens) != 1 or n not in word_lens:
        raise Exception('Not a square at all')

    for r in range(n):
        for c in range(n):
            if words[r][c] != words[c][r]:
                return False
    return True

def make_squares(words):
    word_lens = set(len(w) for w in words)
    if len(word_lens) != 1:
        raise Exception('Different word lengths')

    n = word_lens.pop()
    index = defaultdict(lambda: defaultdict(set))
    for w in words:
        for p in range(len(w)):
            index[p][w[p]].add(w)

    def make_squares(words, partial):
        if len(partial) == n:
            return [partial]

        squares = []
        next_pos = len(partial)
        candidates = words
        for idx, p in enumerate(partial):
            candidates = candidates & index[idx][p[next_pos]]
            if not candidates:
                break
        else:
            for c in candidates:
                squares.extend(make_squares(words - {c}, partial + [c]))
        return squares

    return list(filter(None, make_squares(set(words), [])))

if __name__ == '__main__':
    print(is_square(['area', 'roam', 'easy', 'amyl']))
    print(make_squares(['easy', 'roam', 'area', 'amyl']))

    print(is_square(['BALL', 'AREA', 'LEAD', 'LADY']))
    print(make_squares(['AREA', 'BALL', 'DEAR', 'LADY', 'LEAD', 'YARD']))

