(define (element-of-set set x)
  (cond
    ((null? set) false)
    ((equal? x (car set)) true)
    ((> (car set) x) false)
    (else (element-of-set (cdr set) x))
  )
)

(define (adjoin-set set x)
  (if (element-of-set set x)
    set
    (cons x set)
  )
)

(adjoin-set '() 1)
(adjoin-set '(1 2) 3)
(adjoin-set '(1 2) 1)
(adjoin-set '(1 2 3) 5)

