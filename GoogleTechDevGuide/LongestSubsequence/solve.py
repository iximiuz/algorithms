from collections import defaultdict

def solve(seq, words):
    index = defaultdict(list)
    for pos, l in enumerate(seq):
        index[l].append(pos)

    for w in reversed(sorted(words)):
        last_pos = -1
        for pos, l in enumerate(w):
            positions = [p for p in index[l] if p > last_pos]
            if not positions:
                break
            last_pos = positions[0]
        else:
            return w

    return None

if __name__ == '__main__':
    print(solve("abppplee", {"able", "ale", "apple", "bale", "kangaroo"}))

