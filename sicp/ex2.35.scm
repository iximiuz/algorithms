(define (accumulate op initial sequence) 
 (if (null? sequence)
  initial
  (op (car sequence)
   (accumulate op initial (cdr sequence)))))

;; (define (count-leaves t)
;;   (define (enumerate sub-tree)
;;     (cond 
;;       ((null? sub-tree) sub-tree)
;;       ((pair? sub-tree)
;;         (append (enumerate (car sub-tree)) (enumerate (cdr sub-tree))))
;;       (else (list sub-tree))
;;     )
;;   )
;;   (display (enumerate t))
;;   (accumulate (lambda (stub acc) (+ 1 acc)) 0 (enumerate t))
;; )

(define (count-leaves t)
  (accumulate 
    (lambda (sub-tree acc) 
      (cond
        ((null? sub-tree) acc)
        ((pair? sub-tree)
          (+ acc (count-leaves sub-tree))
        )
        (else (+ acc 1))
      )
    ) 
    0 
    (map (lambda (sub-tree) sub-tree) t)
  )
)

(define x (cons (list 1 2) (list 3 4)))
(count-leaves x)
(count-leaves (list x x))

