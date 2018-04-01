(define (equal? a b)
  (if (and (pair? a) (pair? b))
    (and (eq? (car a) (car b)) (equal? (cdr a) (cdr b)))
    (eq? a b)
  )
)

(equal? '(this is a list) '(this is a list))
(equal? '(this is a list) '(this (is a) list))

