(define (sqrt x) 
 (define (good-enough? guess x) (< (abs (- (* guess guess) x)) 0.0001))
 (define (sqrt-iter x guess)
  (if (good-enough? guess x) 
   guess 
   (sqrt-iter x (/ (+ guess (/ x guess)) 2.0))))

 (sqrt-iter x 1))

(sqrt 0.000001)
(sqrt 0.01)
(sqrt 0.1)
(sqrt 1)
(sqrt 4)
(sqrt 9)
(sqrt 1000000)
(sqrt 1000000000000)
(sqrt 1000000000000000000)

(define (sqrt2 x) 
 (define (good-enough? guess prevGuess) (< (abs (- guess prevGuess)) 0.0001))
 (define (sqrt-iter x guess prevGuess)
  (if (good-enough? guess prevGuess) 
   guess 
   (sqrt-iter x (/ (+ guess (/ x guess)) 2.0) guess)))

 (sqrt-iter x 1 10))

(sqrt2 0.000001)
(sqrt2 0.01)
(sqrt2 0.1)
(sqrt2 1)
(sqrt2 4)
(sqrt2 9)
(sqrt2 1000000)
(sqrt2 1000000000000)
(sqrt2 1000000000000000000)

