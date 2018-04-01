(define (cont-frac n d k)
  (define (term-iter acc i)
    (if (= 0 i) 
      acc
      (term-iter (+ (d i) (/ (n i) acc)) (- i 1))
    )
  )

  (/ (n 0) (term-iter 1 k))
)

(define (e-approx k)
  (define (d i)
    (cond 
      ((= i 0) 1)
      ((= i 1) 2)
      (else 
        (if (= 0 (remainder (- i 1) 3))
          (* 2 (+ 1 (/ (- i 1) 3)))
          1
        )
      )
    )
  )

  (define (dd i)
    (define rv (d (- i 1)))
    ;;; (display i)
    ;;; (display " ")
    ;;; (display rv)
    ;;; (newline)
    rv
  )
  (+ 2 (cont-frac (lambda (i) 1.0) dd k))
)

(e-approx 1000)

