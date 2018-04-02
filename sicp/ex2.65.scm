(define (make-tree entry left right)
  (list entry left right)
)

(define (entry tree)
  (car tree)
)

(define (left-branch tree)
  (cadr tree)
)

(define (right-branch tree)
  (caddr tree)
)

(define (tree->list-1 tree) 
  (if (null? tree)
   '()
    (append (tree->list-1 (left-branch tree))
            (cons (entry tree)
                  (tree->list-1 (right-branch tree))))
  )
)

(define (tree->list-2 tree)
  (define (copy-to-list tree result-list) 
    (if (null? tree)
      result-list
      (copy-to-list (left-branch tree)
                    (cons (entry tree)
                          (copy-to-list (right-branch tree) result-list)))
    )
  )

  (copy-to-list tree '())
)

(define (list->tree elements)
 (car (partial-tree elements (length elements)))
)

(define (partial-tree elts n) 
 (if (= n 0)
  (cons '() elts)
  (let ((left-size (quotient (- n 1) 2)))
   (let ((left-result (partial-tree elts left-size)))
    (let ((left-tree (car left-result)) 
          (non-left-elts (cdr left-result)) 
          (right-size (- n (+ left-size 1))))
     (let ((this-entry (car non-left-elts)) 
           (right-result (partial-tree (cdr non-left-elts) right-size)))
      (let ((right-tree (car right-result)) 
            (remaining-elts (cdr right-result)))
       (cons (make-tree this-entry left-tree right-tree) remaining-elts)
      )
     )
    )
   )
  )
 )
)

(define (union-list l1 l2)
  (cond
    ((null? l1) l2)
    ((null? l2) l1)
    (else 
      (let ((x1 (car l1))
            (x2 (car l2)))
        (cond
          ((= x1 x2) (cons x1 (union-list (cdr l1) (cdr l2))))
          ((> x1 x2) (cons x2 (union-list l1 (cdr l2))))
          ((< x1 x2) (cons x1 (union-list (cdr l1) l2)))
        )
      )
    )
  )
)

(define (intersection-list l1 l2)
  (if (or (null? l1) (null? l2)) 
    '()
    (let ((x1 (car l1))
          (x2 (car l2)))
      (cond
        ((= x1 x2) (cons x1 (intersection-list (cdr l1) (cdr l2))))
        ((> x1 x2) (intersection-list l1 (cdr l2)))
        ((< x1 x2) (intersection-list (cdr l1) l2))
      )
    )
  )
)

(define (union-set set1 set2)
  (list->tree
    (union-list 
      (tree->list-2 set1)
      (tree->list-2 set2)
    )
  )
)

(define (intersection-set set1 set2)
  (list->tree
    (intersection-list 
      (tree->list-2 set1)
      (tree->list-2 set2)
    )
  )
)

(define s1 '(5 (3 (2 (1 () ()) ()) (4 () ())) (8 (6 () (7 () ())) (9 () ()))))
(define s2 '(1 () (3 () (5 () (7 () (9 () ()))))))

(union-set s1 '())
(union-set '() s2)
(union-set s1 s2)

(intersection-set s1 '())
(intersection-set '() s2)
(intersection-set s1 s2)

