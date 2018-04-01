;;; (define (product term a next b) 
;;;   (if (> a b) 
;;;     1
;;;     (* (term a) (product term (next a) next b))
;;;   )
;;; )

(define (product term a next b)
  (define (product-iter a acc)
    (if (> a b) 
      acc
      (product-iter (next a) (* acc (term a)))
    )
  )

  (product-iter a 1)
)

(define (factorial b)
  (define (identity x) x)
  (define (inc x) (+ x 1))
  (product identity 1 inc b)
)

(factorial 0)
(factorial 1)
(factorial 3)
(factorial 4)
(factorial 5)
(factorial 6)
(factorial 7)
(factorial 8)
(factorial 9)

(define (pi-approx n)
  (define (numerator x) ;;; 0 2 2 4 4 6
    (if (= (remainder x 2) 0)
      x
      (+ x 1)
    )
  )

  (define (denominator x) ;;; 0 0 2 2 4 4
    (if (= (remainder x 2) 0)
      x
      (- x 1)
    )
  )

  (define (term x)
    (/ (+ 2.0 (numerator x)) (+ 3.0 (denominator x)))
  )

  (define (next x) (+ x 1))

  (* 4 (product term 0 next n))
)

(pi-approx 100000)

