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

(define sample-tree (make-code-tree (make-leaf 'A 4)
                                    (make-code-tree (make-leaf 'B 2)
                                                    (make-code-tree (make-leaf 'D 1)
                                                                    (make-leaf 'C 1)))))
(display sample-tree)

(define sample-tree (generate-huffman-tree '((C 1) (D 1) (B 2) (A 4))))
(display sample-tree)

(define sample-message '(A D A B B C A))

(encode sample-message sample-tree) ; must be '(0 1 1 0 0 1 0 1 0 1 1 1 0)

