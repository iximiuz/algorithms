(define (cont-frac n d k)
  (define (term i)
    (if (> i k)
      1
      (/ (n i) (+ (d (+ i 1)) (term (+ i 1))))
    )
  )
  (term 0)
)

(define (cont-frac n d k)
  (define (term-iter acc i)
    (if (= i 0) 
      acc
      (term-iter (+ (d i) (/ (n i) acc)) (- i 1))
    )
  )

  (/ (n 0) (term-iter 1 k))
)

(cont-frac (lambda (i) 1.0) (lambda (i) 1.0) 10)

