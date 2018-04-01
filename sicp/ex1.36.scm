(define tolerance 0.00001)

(define (fixed-point f first-guess)
  (define (close-enough? v1 v2) (> tolerance (abs (- v1 v2))))
  
  (define (try guess)
    (let ((next (f guess)))
      (display next)
      (newline)
      (if (close-enough? next guess)
        next
        (try next)
      )
    )
  )

  (try first-guess)
)

(fixed-point (lambda (x) (/ (log 1000.0) (log x))) 1.5)
(fixed-point (lambda (x) (/ (+ x (/ (log 1000.0) (log x))) 2)) 1.5)

