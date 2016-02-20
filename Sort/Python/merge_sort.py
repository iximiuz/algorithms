from math import ceil


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
    size = len(array)
    if size > 1:
        aux = [0]*size
        step = 1
        while step < size:
            # print('while')
            low = 0
            while low < size:
                mid = min(low + step, size) - 1               
                high = min(mid + step, size - 1)
                # print('s={s} l={l} m={m} h={h}'.format(**{'s': step, 'l': low, 'm': mid, 'h': high}))
                _merge(array, aux, low, mid, high)
                # print(array)
                low += 2*step
            step += step


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
    short_in = list(array) if len(array) <= 30 else array[0:10] + ['...'] + array[-10:]
    print('In (len=%s) %s' % (len(array), short_in))
    
    func(array)
    
    short_out = list(array) if len(array) <= 30 else array[0:10] + ['...'] + array[-10:]
    print('Out %s' % short_out)
    if not is_sorted(array):
        raise Exception('Error on fixture %s -> %s' % (short_in, short_out))
    print('Sorted!')
    print('')


def test_suite(func):
    from random import randint
    
    fixtures =[[], [1], [1, 1], [0, 1], [1, 0], [1, 1, 1], [1, 2, 3], [3, 2, 1]]
    for size in (4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 50, 100, 1000):
        fixtures.append([randint(0, 1000) for _ in range(size)])
        
    print('Testing sort function [' + func.__name__ + '] ...')
    for a in fixtures:
        test_sort_func(a, func)
    print('Success!')
   

if __name__ == '__main__':
    test_suite(merge_sort_top_down)
    test_suite(merge_sort_bottom_up)
