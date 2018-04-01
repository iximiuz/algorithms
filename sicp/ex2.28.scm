(define (fringe x)
  (define (walk el)
    (cond
      ((null? el) el)
      ((pair? el) (append (walk (car el)) (walk (cdr el))))
      (else (list el))
    )
  )
  (walk x)
)

(define x (list (list 1 2) (list 3 4))) 
(fringe x)
(fringe (list x x))

