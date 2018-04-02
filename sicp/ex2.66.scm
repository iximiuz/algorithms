(define (lookup key set)
  (cond
    ((null? set) false)
    ((= key (entry set)) key)
    ((> key (entry set) (lookup key (caddr set))))
    ((< key (entry set) (lookup key (cadr set))))
  )
)

