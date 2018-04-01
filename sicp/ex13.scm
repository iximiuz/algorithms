(define (sq x) (* x x))

(define (max a b) (if (> a b) a b))

(define (ex13 a b c)
 (if (> a b) 
  (+ (sq a) (sq (max b c))) 
  (+ (sq b) (sq (max a c)))))

(ex13 3 4 5)
(ex13 5 4 3)

