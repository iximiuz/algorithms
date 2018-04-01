(define (make-mobile left right) 
  (list left right)
)

(define (make-branch length structure) 
  (list length structure)
)

(define (left-branch mobile)
  (car mobile)
)

(define (right-branch mobile)
  (cdr mobile)
)

(define (branch-length branch)
  (car branch)
)

(define (branch-structure branch)
  (cdr branch)
)

(define (branch-weight b)
  (let ((stucture (branch-structure b)))
    (if (pair? structure)
      (total-weight structure)
      structure
    )
  )
)

(define (total-weight mobile)
  (+ (branch-weight (left-branch mobile)) (branch-weight (right-branch mobile)))
)

(define (balanced? mobile)
  (let ((lb (left-branch mobile))
        (rb (right-branch mobile))
        (lbs (branch-structure lb))
        (rbs (branch-structure rb)))
    (and
      (= (* (branch-length lb) (branch-weight lb)) (* (branch-length rb) (branch-weight rb)))
      (or (not (pair? lbs)) (balanced? lbs))
      (or (not (pair? rbs)) (balanced? rbs))
    )
  )
)

