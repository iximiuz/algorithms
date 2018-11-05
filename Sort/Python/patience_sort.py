def psort(arr):
    if not arr:
        return []

    buckets = []
    for x in arr:
        idx = _bucket_idx(buckets, x)
        if idx == len(buckets):
            buckets.append([x])
        else:
            buckets[idx].append(x)

    return list(reversed(_merge(buckets, 0, len(buckets))))

def _bucket_idx(buckets, val):
    l, r = 0, len(buckets)
    while l < r:
        m = l + (r - l) // 2
        if buckets[m][-1] < val:
            l = m + 1
        else:
            r = m
    return l

def _merge(buckets, l, r):
    if r - l == 1:
        return buckets[l]

    m = l + (r - l) // 2
    return _merge2(_merge(buckets, l, m), _merge(buckets, m, r))

def _merge2(left, right):
    nleft = len(left)
    nright = len(right)
    res = [0] * (nleft + nright)

    i, j, k = 0, 0, 0
    while i < nleft and j < nright:
        if left[i] > right[j]:
            res[k] = left[i]
            i += 1
        else:
            res[k] = right[j]
            j += 1
        k += 1

    while i < nleft:
        res[k] = left[i]
        i += 1
        k += 1
    while j < nright:
        res[k] = right[j]
        j += 1
        k += 1

    return res

if __name__ == '__main__':
    def _testit():
        import random

        for i in range(100):
            assert list(range(i)) == psort(list(range(i)))
            assert list(range(i)) == psort(list(reversed(range(i))))

        for _ in range(1000):
            n = random.randint(0, 1000)
            arr = [0] * n
            for i in range(n):
                arr[i] = random.randint(-100, 100)

            assert sorted(arr) == psort(arr)
        print('OK')
    
    _testit()
