(define (queens board-size) 
  (define (queen-cols k)
    (if (= k 0)
      (list empty-board) 
      (filter
        (lambda (positions) (safe? k positions)) 
        (flatmap
          (lambda (rest-of-queens) 
            (map 
              (lambda (new-row) (adjoin-position new-row k rest-of-queens))
              (enumerate-interval 1 board-size)
            )
          )
          (queen-cols (- k 1))
        )
      )
    )
  )

 (queen-cols board-size)
)

(define (enumerate-interval a b)
  (if (> a b)
    '()
    (cons a (enumerate-interval (+ a 1) b))
  )
)

(define (accumulate agg init seq)
  (if (null? seq)
    init
    (agg (car seq) (accumulate agg init (cdr seq)))
  )
)

(define (flatmap func seq)
  (accumulate append '() (map func seq))
)

(define empty-board '())

(define (adjoin-position row col positions)
  (cons (cons row col) positions)
)

(define (safe? col positions)
  (let ((new-pos (car positions))
        (remaining (cdr positions)))
    (or (= 1 col) (and (safe-row? new-pos remaining) (safe-diag? new-pos remaining)))
  )
)

(define (safe-row? pos remaining)
  (let ((row (car pos)))
    (accumulate 
      (lambda (prev-pos acc)
        (and acc (not (= row (car prev-pos))))
      )
      true
      remaining
    )
  )
)

(define (safe-diag? new-pos postions)
  (define (check-col delta remaining)
    (or 
      (null? remaining) 
      (and 
        (not (= (+ (car new-pos) delta) (car (car remaining))))
        (not (= (- (car new-pos) delta) (car (car remaining))))
        (check-col (+ delta 1) (cdr remaining))
      )
    )
  )

  (check-col 1 postions)
)

(queens 1)
(queens 2)
(queens 3)
(queens 6)
(queens 8)

