(define (smallets-divisor n)
  (define (find-divisor a) 
    (cond 
      ((> (* a a) n) n)
      ((= (remainder n a) 0) a)
      (else (find-divisor (+ a 1)))
    )
  )

  (find-divisor 2)
)

(smallets-divisor 1)
(smallets-divisor 2)
(smallets-divisor 3)
(smallets-divisor 4)
(smallets-divisor 5)
(smallets-divisor 6)
(smallets-divisor 199)
(smallets-divisor 1024)
(smallets-divisor 1999)
(smallets-divisor 19999)

