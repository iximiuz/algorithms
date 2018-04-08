(define (make-leaf s w)
  (list 'leaf s w)
)

(define (leaf? x)
  (eq? (car x) 'leaf)
)

(define (symbol-leaf x) (cadr x))

(define (weight-leaf x) (caddr x))

(define (symbols tree)
  (if (leaf? tree)
    (list (symbol-leaf tree)) 
    (caddr tree)
  )
)

(define (weight tree) 
  (if (leaf? tree)
    (weight-leaf tree)
    (cadddr tree)
  )
)

(define (make-code-tree left right)
  (list left
        right
        (append (symbols left) (symbols right))
        (+ (weight left) (weight right)))
)

(define (left-branch tree) (car tree)) 

(define (right-branch tree) (cadr tree)) 

(define (encode message tree) 
  (if (null? message)
    '()
     (append (encode-symbol (car message) tree)
             (encode (cdr message) tree))
  )
)

(define (contains? lst x)
  (if (null? lst)
    false
    (or (eq? x (car lst)) (contains? (cdr lst) x))
  )
)

(define (encode-symbol s tree)
  ; (newline)
  ; (display tree)
  (cond 
    ((leaf? tree)
      (if (eq? (symbol-leaf tree) s)
        '()
        (error "Bad leaf")
      )
    )
    ((contains? (symbols (left-branch tree)) s)
      ; (newline)
      ; (display "left b")
      (cons 0 (encode-symbol s (left-branch tree))))
    ((contains? (symbols (right-branch tree)) s)
      ; (newline)
      ; (display "right b")
      (cons 1 (encode-symbol s (right-branch tree))))
    (else (error "Unknown symbol"))
  )
)

(define (adjoin-set x set) 
  (cond 
    ((null? set) (list x))
    ((< (weight x) (weight (car set))) (cons x set))
    (else (cons (car set) (adjoin-set x (cdr set))))
  )
)

(define (make-leaf-set pairs) 
  (if (null? pairs)
    '()
    (let ((pair (car pairs)))
      (adjoin-set (make-leaf (car pair) (cadr pair)) 
                  (make-leaf-set (cdr pairs)))
    )
  )
)

(define (generate-huffman-tree pairs) 
  (successive-merge (make-leaf-set pairs))
)

(define (successive-merge set)
  (cond 
    ((null? set) (error "Pairs set must be non-empty"))
    ((null? (cdr set)) (car set))
    (else
      (let ((first (car set))
            (second (cadr set)))
        (successive-merge (adjoin-set (make-code-tree first second) (cddr set)))
      )
    )
  )
)

(define sample-tree (generate-huffman-tree '((WAH 1) (BOOM 1) (A 2) (GET 2) (JOB 2) (SHA 3) (YIP 9) (NA 16))))

(define sample-message '(GET A JOB
                         SHA NA NA NA NA NA NA NA NA
                         GET A JOB
                         SHA NA NA NA NA NA NA NA NA
                         WAH YIP YIP YIP YIP YIP YIP YIP YIP YIP
                         SHA BOOM))

(encode sample-message sample-tree) 

