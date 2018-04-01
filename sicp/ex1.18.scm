(define (mul a b)
  (define (mul-iter a b c)
    (cond
      ((= b 0) c)
      ((= (remainder b 2) 0) (mul-iter (* a 2) (/ b 2) c))
      (else (mul-iter a (- b 1) (+ c a)))
    )
  )

  (mul-iter a b 0)
)

(mul 31 29)

