(define (enumerate-interval a b)
  (if (> a b)
    '()
    (cons a (enumerate-interval (+ a 1) b))
  )
)
;(enumerate-interval 0 10)
;(enumerate-interval 5 6)
(define (accumulate proc init seq)
  (if (null? seq)
    init
    (proc (car seq) (accumulate proc init (cdr seq)))
  )
)

(define (flatmap proc seq)
  (accumulate append '() (map proc seq))
)

(define (find-triples n s)
  (filter 
    (lambda (triple)
      (= s (accumulate + 0 triple))
    )
    (flatmap 
      (lambda (x) x)
      (flatmap
        (lambda (x) x)
        (map 
          (lambda (i)
            (map 
              (lambda (j)
                (map 
                  (lambda (k) (list i j k))
                  (enumerate-interval 0 (- j 1))
                )
              )
              (enumerate-interval 0 (- i 1))
            )
          )
          (enumerate-interval 0 n)
        )
      )
    )
  )
)

(find-triples 5 5)

