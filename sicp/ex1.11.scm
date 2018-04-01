(define (f n) 
  (if (> 3 n) 
    n
    (+ (f (- n 1)) (* 2 (f (- n 2))) (* 3 (f (- n 3))))))

(define (f n)
  (define (f-iter fn-3 fn-2 fn-1 i) 
    (if (= i (- n 2)) 
      fn-1
      (f-iter fn-2 fn-1 (+ fn-1 (* 2 fn-2) (* 3 fn-3))  (+ i 1))))

  (if (< n 3) n (f-iter 0 1 2 0))
)

(f 0)
(f 1)
(f 2)
(f 3)
(f 4)
(f 5)
(f 6)
(f 7)
(f 8)
(f 9)
(f 10)
(f 11)
(f 12)
(f 13)
(f 14)
(f 15)

