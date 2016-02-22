from merge_sort import merge_sort_top_down, merge_sort_bottom_up
from quick_sort import quick_sort, quick_sort_3way


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


def is_sorted(array):
    for i in range(len(array)):
        for j in range(i + 1, len(array)):
            if array[i] > array[j]:
                return False
    return True
   

if __name__ == '__main__':
    test_suite(merge_sort_top_down)
    test_suite(merge_sort_bottom_up)
    test_suite(quick_sort)
    test_suite(quick_sort_3way)
