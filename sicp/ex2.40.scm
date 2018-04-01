(define (enumerate-interval a b)
  (if (> a b) 
    '()
    (cons a (enumerate-interval (+ a 1) b))
  )
)
; (enumerate-interval 5 6)
; (enumerate-interval 0 10)

(define (accumulate proc init seq)
   (if (null? seq)
     init
     (proc (car seq) (accumulate proc init (cdr seq)))
   )
)

(define (flatmap proc seq)
 (accumulate append '() (map proc seq))
)

(define (unique-pairs n)
  (flatmap 
    (lambda (x) x)
    (map 
      (lambda (i) 
        (map (lambda (j) (list j i)) 
             (enumerate-interval 0 (- i 1)))
      )
      (enumerate-interval 0 n)
    )
  )
)

(unique-pairs 3)
(unique-pairs 10)

