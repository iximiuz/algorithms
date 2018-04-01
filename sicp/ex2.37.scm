(define (accumulate op initial sequence) 
 (if (null? sequence)
  initial
  (op (car sequence)
   (accumulate op initial (cdr sequence)))))

(define (accumulate-n op init seqs) 
  (if (null? (car seqs))
    '()
    (cons (accumulate op init (map (lambda (x) (car x)) seqs)) 
          (accumulate-n op init (map (lambda (x) (cdr x)) seqs)))
  )
)

(define (dot-product v w)
  (accumulate + 0 (map * v w))
)

(define (matrix-*-vector m v) 
  (map 
    (lambda (row) 
      (accumulate 
        (lambda (p acc) 
          (+ acc (* (car p) (cdr p)))
        )
        0 
        (map (lambda (ri vi) (cons ri vi)) row v)
      )
    )
    m
  )
)

(define (transpose mat) 
  (accumulate-n (lambda (c row) (cons c row)) '() mat)
)

(define (matrix-*-matrix m n) 
  (let ((cols (transpose n)))
    (map 
      (lambda (row)
        (map 
          (lambda (col)
            (dot-product row col)
          ) 
          cols
        )
      )
      m
    )
  )
)

(define m (list (list 1 2 3) (list 4 5 6) (list 7 8 9)))
(define v (list 5 6 7))

(matrix-*-vector m v)
(transpose m)
(matrix-*-matrix m m)

