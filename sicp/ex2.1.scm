(define (gcd a b)
  (if (= b 0) 
    a
    (gcd b (remainder a b))
  )
)

(define (sign x)
  (if (> 0 x) -1 1)
)

(define (make-rat n d)
  (let ((g (gcd (abs n) (abs d)))
        (s (* (sign n) (sign d))))
    (cons (/ (abs n) g s) (/ (abs d) g))
  )
)

(define (numer x) (car x))

(define (denom x) (cdr x))

(define (print-rat x)
  (newline)
  (display (numer x))
  (display "/")
  (display (denom x))
)

(define (add-rat x y)
  (make-rat 
    (+ (* (numer x) (denom y)) 
       (* (numer y) (denom x)))
    (* (denom x) (denom y))
  )
)

(define (sub-rat x y)
  (make-rat 
    (- (* (numer x) (denom y)) 
       (* (numer y) (denom x)))
    (* (denom x) (denom y))
  )
)

(define (mul-rat x y)
  (make-rat
    (* (numer x) (numer y))
    (* (denom x) (denom y))
  )
)

(define (div-rat x y)
  (make-rat
    (* (numer x) (denom y))
    (* (denom x) (numer y))
  )
)

(define (equal-rat? x y)
  (= (* (numer x) (denom y)) (* (numer y) (denom x)))
)

(define one-half (make-rat 1 2)) 
(print-rat one-half)

(define one-third (make-rat 1 3)) 
(print-rat one-third)

(print-rat (add-rat one-half one-third))
(print-rat (mul-rat one-half one-third))
(print-rat (add-rat one-third one-third))
(print-rat (make-rat -1 10))
(print-rat (make-rat 1 -10))
(print-rat (make-rat -1 -10))

