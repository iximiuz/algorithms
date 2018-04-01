(define (reverse-deep d)
  (if (pair? d)
    (if (null? (cdr d))
      (reverse-deep (car d))
      (list (reverse-deep (cdr d)) (reverse-deep (car d)))
    )
    d
  )
)

(define x (list (list 1 2) (list 3 4)))
(reverse-deep x)

