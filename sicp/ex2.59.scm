(define (element-of-set set x)
  (and (not (null? set)) (or (equal? x (car set)) (element-of-set (cdr set) x)))
)

(define (union-set set1 set2)
  (cond
    ((null? set1) set2)
    ((null? set2) set1)
    ((element-of-set set2 (car set1)) (union-set (cdr set1) set2))
    (else (cons (car set1) (union-set (cdr set1) set2)))
  )
)

(union-set '(1) '())
(union-set '(1 2) '(3))
(union-set '(1 2) '(1))
(union-set '(1 2 3) '(1 2 4 5 6))

