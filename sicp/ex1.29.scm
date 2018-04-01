(define (sum term a next b) 
  (if (> a b) 
    0
    (+ (term a) (sum term (next a) next b))
  )
)

;;; (define (sum-int a b)
;;;   (define (identity x) x)
;;;   (define (inc x) (+ x 1))
;;;   (sum identity a inc b)
;;; )
;;; 
;;; (sum-int 0 0)
;;; (sum-int 0 1)
;;; (sum-int 0 3)
;;; (sum-int 0 4)
;;; (sum-int 0 5)
;;; (sum-int 0 6)
;;; (sum-int 0 7)
;;; (sum-int 0 8)
;;; (sum-int 0 9)

(define (simpsone f a b n)
  (define h (/ (- b a) (* 1.0 n)))
  (define (y k) (f (+ a (* k h))))

  (define (term x) 
    (define (t x)
      (cond 
        ((or (= 0 x) (= n x)) 1)
        ((= (remainder x 2) 0) 2)
        (else 4)
      )
    )
    (* (t x) (y x))
  )

  (define (inc x) (+ x 1))

  (* (/ h 3) (sum term 0 inc n))
)

(define (cube x) (* x x x))

(simpsone cube 0 1 100)
(simpsone cube 0 1 1000)

(define (integral f a b dx) 
  (define (add-dx x) (+ x dx))
  (* (sum f (+ a (/ dx 2.0)) add-dx b) dx)
)

(integral cube 0 1 0.01)
(integral cube 0 1 0.001)

