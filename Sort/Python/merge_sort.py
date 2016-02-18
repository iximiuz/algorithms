def merge_sort_top_down(array):
    def _sort(array, aux, low, high):        
        # print('sort', low, high)
        if high - low > 0:            
            mid = low + (high - low)//2
            _sort(array, aux, low, mid)
            _sort(array, aux, mid + 1, high)
            _merge(array, aux, low, mid, high)
            # print('after merging', array, low, high)

    _sort(array, list(array), 0, len(array) - 1)


def merge_sort_bottom_up(array):
    part_size = 1
    size = len(array)
    aux = []*size
    while part_size < size:
        for offset in range(size, part_size):
            low = offset
            mid = offset + part_size
            high = min(size - 1, offset + 2*part_size)
            _merge(array, aux, low, mid, high)

        part_size += part_size


def _merge(array, aux, low, mid, high):
        # print('merge', low, mid, high)        
        for i in range(low, high + 1):
            aux[i] = array[i]

        i, l, r = low, low, mid + 1
        while l <= mid or r <= high:
            if l > mid:
                array[i] = aux[r]
                r += 1
            elif r > high:
                array[i] = aux[l]                
                l += 1
            elif aux[l] < aux[r]:
                array[i] = aux[l]
                l += 1
            else:
                array[i] = aux[r]
                r += 1
            i += 1        


def is_sorted(array):
    for i in range(len(array)):
        for j in range(i + 1, len(array)):
            if array[i] > array[j]:
                return False
    return True


def test_sort_func(array, func):
    short = array if len(array) <= 30 else array[0:10] + ['...'] + array[-10:]
    print('In %s' % short)
    
    func(array)
    
    short = array if len(array) <= 30 else array[0:10] + ['...'] + array[-10:]
    print('Out %s' % short)
    if not is_sorted(array):
        raise Exception('Error on fixture %s' % short)
    print('Sorted!')
    print()


def test_suite(func):
    from random import randint
    
    fixtures =[[], [1], [1, 1], [1, 1, 1], [1, 2, 3], [3, 2, 1]]
    for size in (10, 50, 100, 1000):
        fixtures.append([randint(0, 1000) for _ in range(size)])
        
    print('Testing sort function [' + func.__name__ + '] ...')
    for a in fixtures:
        test_sort_func(a, func)
   

if __name__ == '__main__':
    test_suite(merge_sort_top_down)
    test_suite(merge_sort_bottom_up)
