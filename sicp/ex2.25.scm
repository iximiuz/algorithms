(define l1 (list 1 3 (list 5 7) 9))
(car (cdr (car (cdr (cdr l1)))))

(define l2 (list (list 7)))
(car (car l2))

