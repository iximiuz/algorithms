(define (last-pair l)
  (if (null? l) 
    (error "List cannot be empty")
    (if (null? (cdr l))
      l
      (last-pair (cdr l))
    )
  )
)

(last-pair (list 1 2 3 4 5))
;;; (last-pair '())

