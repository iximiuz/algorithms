(define (mul a b)
  (cond 
    ((= b 0) 0)
    ((= (remainder b 2) 1) (+ a (mul a (- b 1))))
    (else (mul (* a 2) (/ b 2)))
  )
)

(mul 31 29)

