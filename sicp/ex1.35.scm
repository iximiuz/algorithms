(define tolerance 0.00001)

(define (fixed-point f first-guess)
  (define (close-enough? v1 v2) (> tolerance (abs (- v1 v2))))
  
  (define (try guess)
    (let ((next (f guess)))
      (if (close-enough? next guess)
        next
        (try next)
      )
    )
  )

  (try first-guess)
)

(define (gf x) (+ 1.0 (/ 1.0 x)))
(fixed-point gf 1)

