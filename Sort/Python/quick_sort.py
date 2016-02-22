from random import shuffle


def _partition(array, lt, gt):
    i = lt + 1
    j = gt
    while True:
        while i <= gt and array[i] <= array[lt]:
            i += 1
        while j > lt and array[j] >= array[lt]:
            j -= 1

        if j <= i: break
        array[i], array[j] = array[j], array[i]

    array[lt], array[j] = array[j], array[lt]    
    return j 


def _sort(array, lt, gt):
    if lt >= gt: return

    j = _partition(array, lt, gt)
    _sort(array, lt, j - 1)
    _sort(array, j + 1, gt)


def _partition_3way(array, lt, gt):
    i = lt
    c = array[lt]
    while i <= gt:
        if c > array[i]:
            array[lt], array[i] = array[i], array[lt]
            lt += 1
            i +=1
        elif c < array[i]:
            array[gt], array[i] = array[i], array[gt]
            gt -= 1
        else:  # c == array[i]
            i += 1        
    return lt, gt


def _sort_3way(array, lt, gt):
    if lt >= gt: return

    l, g = _partition_3way(array, lt, gt)
    _sort_3way(array, lt, l - 1)
    _sort_3way(array, g + 1, gt)


def quick_sort(array):
    shuffle(array)
    _sort(array, 0, len(array) - 1)


def quick_sort_3way(array):
    shuffle(array)
    _sort_3way(array, 0, len(array) - 1)
