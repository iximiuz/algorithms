(define (cont-frac n d k)
  (define (term-iter acc i)
    (if (= 0 i) 
      acc
      (term-iter (+ (d i) (/ (n i) acc)) (- i 1))
    )
  )

  (/ (n 0) (term-iter 1 k))
)

(define (tan-cf x k)
  (define (n i)
    (if (= 0 i)
      x
      (* -1 x x)
    )
  )

  (define (d i)
    (+ 1 (* 2 (- i 1)))
  )

  (define (dd i)
    (display i)
    (display " ")
    (display (d i))
    (newline)
    (d i)
  )

  (cont-frac n d k)
)

(tan-cf 0.0 100)
(tan-cf (/ 3.14 4) 100)
(tan-cf (/ 3.14 2) 100)
(tan-cf (* 3 (/ 3.14 4)) 100)
(tan-cf 3.14 100)

