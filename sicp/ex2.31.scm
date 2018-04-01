(define (map-tree f tree)
  (map 
    (lambda (sub-tree)
      (if (pair? sub-tree)
        (map-tree f sub-tree)
        (f sub-tree)
      )
    )
    tree
  )
)

(define (square-tree tree)
  (map-tree square tree)
)

(square-tree
  (list 1
    (list 2 
      (list 3 4) 
    5)
    (list 6 7))
)
;; (1 (4 (9 16) 25) (36 49))
