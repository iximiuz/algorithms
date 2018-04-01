(define (cons a b)
  (* (expt 2 a) (expt 3 b))
)

(define (car z)
  (if (= 0 (remainder z 2))
    (+ 1 (car (/ z 2)))
    0
  )
)

(define (cdr z)
  (define (log3 x)
    (if (= 1 x) 0 (+ 1 (log3 (/ x 3))))
  )

  (log3 (/ z (expt 2 (car z))))
)

(car (cons 0 0))
(cdr (cons 0 0))

(car (cons 0 1))
(cdr (cons 0 1))

(car (cons 1 0))
(cdr (cons 1 0))

(car (cons 1 1))
(cdr (cons 1 1))

(car (cons 1 10))
(cdr (cons 1 10))

(car (cons 10 1))
(cdr (cons 10 1))

(car (cons 3 4))
(cdr (cons 3 4))

(car (cons 0 5))
(cdr (cons 0 5))

