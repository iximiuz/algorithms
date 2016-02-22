from time import time

from quick_sort import quick_sort, quick_sort_3way


import sys
sys.setrecursionlimit(15000)

fixture = [42]*10000

start = time()
quick_sort_3way(fixture)
print("3-Way Partition Quick Sort ends in [%s] ms" % (time() - start))

start = time()
quick_sort(fixture)
print("Quick Sort ends in [%s] ms" % (time() - start))
