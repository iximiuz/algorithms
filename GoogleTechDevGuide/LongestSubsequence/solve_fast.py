from collections import defaultdict

def solve(s, words):
    words = [(w, 0) for w in words]
    index = defaultdict(list)
    for wt in words:
        index[wt[0][wt[1]]].append(wt)

    longest_complete_word = ''
    for l in s:
        tuples = index[l]
        index[l] = []
        for wt in tuples:
            pos = wt[1] + 1
            if pos == len(wt[0]) - 1:
                longest_complete_word = max(longest_complete_word, wt[0])
            else:
                index[wt[0][pos]].append((wt[0], pos))

    return longest_complete_word

if __name__ == '__main__':
    print(solve("abppplee", {"able", "ale", "apple", "bale", "kangaroo"}))

