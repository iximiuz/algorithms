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
            while low < size - step:
                mid = low + step - 1               
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
