(define (cbrt x) 
 (define (good-enough? guess prevGuess) (< (abs (- guess prevGuess)) 0.0001))
 (define (cbrt-iter x guess prevGuess)
  (if (good-enough? guess prevGuess) 
   guess 
   (cbrt-iter x (/ (+ (/ x (* guess guess)) (* 2 guess)) 3.0) guess)))

 (cbrt-iter x 1 10))

(cbrt 0.0027)
(cbrt 1)
(cbrt 27)
(cbrt 1000)
(cbrt 1000000000)

