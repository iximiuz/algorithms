(define (same-parity a . z)
  (define (same-parity-acc l acc)
    (cond
      ((null? l) acc)
      ((= (remainder a 2) (remainder (car l) 2)) (same-parity-acc (cdr l) (append acc (list (car l)))))
      (else (same-parity-acc (cdr l) acc))
    )
  )
  (same-parity-acc z (list a))
)

(same-parity 1 2 3 4 5 6 7)
(same-parity 2 3 4 5 6 7)

