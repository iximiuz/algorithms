from collections import deque

def tree2list(tree):
  if not tree:
    return []
  return tree2list(tree[1]) + [tree[0]] + tree2list(tree[2])

def list2tree(lst):
  if not lst:
    return ()
  mid = len(lst) / 2
  return (lst[mid], list2tree(lst[:mid]), list2tree(lst[mid + 1:]))

def rebalance_tree(tree):
  return list2tree(tree2list(tree))

def tree_height(tree):
  if not tree:
    return 0
  return 1 + max(tree_height(tree[1]), tree_height(tree[2]))

def print_tree(tree, unit=4):
  if not tree:
     print('<empty tree>')

  height = tree_height(tree)
  width = unit * (2**(height - 1))
  queue = deque([tree + (0, 0)])
  rows = []
  while queue:
    entry, left, right, row_no, col_no = queue.popleft()
    if row_no == height:
      break

    if row_no == len(rows):
      rows.append([])
    row = rows[row_no]
    spacing = (width - unit * (2**row_no)) / (2**row_no)
    if col_no == 0:
      row.append(' ' * (spacing/2))
    else:
      row.append(' ' * spacing)

    row.append(str(entry).rjust(unit, ' '))

    queue.append((left or (' ', (), ())) + (row_no + 1, 2 * col_no))
    queue.append((right or (' ', (), ())) + (row_no + 1, 2 * col_no + 1))

  print('\n'.join(map(lambda r: ''.join(r), rows)))

def main():
  t1 = (5, (3, (2, (1, (), ()), 
                   ()), 
               (4, (), ())),
           (8, (6, (), 
                   (7, (), (),)), 
               (10, (9, (), ()), 
                    (11, (), ()))))
  t2 = (9, (8, (7, (6, (5, (4, (3, (2, (1, (), ()), ()), ()), ()), ()), ()), ()), ()), ()) 
  t3 = (1, (), (2, (), (3, (), (4, (), (5, (), (6, (), (7, (), (8, (), (9, (), ())))))))))

  print_tree(t1, unit=6)
  print('')
  print_tree(t2, unit=1)
  print('')
  print_tree(t3, unit=1)
  print('')

  print(tree2list(t1))
  print(tree2list(t2))
  print(tree2list(t3))

  print_tree(rebalance_tree(t1))
  print('')
  print_tree(rebalance_tree(t2))
  print('')
  print_tree(rebalance_tree(t3))
  print('')

if __name__ == '__main__':
  main()

