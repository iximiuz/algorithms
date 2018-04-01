(define (variable? x) (symbol? x))

(define (same-variable? x y)
  (and (variable? x) (variable? y) (eq? x y))
)

(define (=number? exp num)
  (and (number? exp) (= exp num))
)

(define (compound-expression? exp)
  (and 
    (not (null? exp) 
    (or 
      (operator? (car exp)) 
      (compound-expression? (cdr exp)))
    )
  )
)

(define (operator? x)
  (or (eq? '+ x) (eq? '* x) (eq? '** x))
)

(define (left-operand exp op)
  (define (iter tail acc)
    (if (or (null? tail) (eq? (car tail) op))
      acc
      (iter (cdr tail) (append acc (list (car tail))))
    )
  )

  (let ((lv (iter exp '())))
    (if (null? (cdr lv)) (car lv) lv)
  )
)

(define (right-operand exp op)
  (define (skip-till-op tail)
    (if (eq? op (car tail))
      (cdr tail)
      (skip-till-op (cdr tail))
    )
  )

  (let ((rv (skip-till-op exp)))
    (if (null? (cdr rv)) (car rv) rv)
  )
)

(define (next-operation exp)
  (define LEAST_KNOWN_PRIORITY 10)
  
  (define (priority op)
    (cond 
      ((eq? '+ op) 10)
      ((eq? '* op) 15)
      ((eq? '** op) 20)
      (else (error "unknown op"))
    )
  )

  (define (find-first-op e)
    (if (operator? (car e))
      (car e)
      (find-first-op (cdr e))
    )
  )

  (define (find-least-priority-op tail prev-op)
    (cond
      ((null? tail) prev-op)
      ((= (priority prev-op) LEAST_KNOWN_PRIORITY) prev-op)
      ((and (operator? (car tail)) (> (priority prev-op) (priority (car tail))))
        (find-least-priority-op (cdr tail) (car tail))
      )
      (else (find-least-priority-op (cdr tail) prev-op))
    )
  )

  (find-least-priority-op exp (find-first-op exp))
)

(define (make-sum a b) 
  (cond
    ((=number? a 0) b)
     ((=number? b 0) a)
    ((and (number? a) (number? b)) (+ a b))
    (else (list (if (compound-expression? a) (list a) a) '+ (if (compound-expression? b) (list b) b)))
  )
)

(define (addend exp) 
  (define rv (left-operand exp '+))
  (newline)
  (display "exp ")
  (display exp)
  (display " addend ")
  (display rv)
  rv
)

(define (augend exp) 
  (define rv (right-operand exp '+))
  (newline)
  (display "exp ")
  (display exp)
  (display " augend ")
  (display rv)
  rv
)

(define (sum? exp)
  (and (pair? exp) (eq? '+ (next-operation exp)))
)

(define (make-product a b) 
  (cond
    ((or (=number? a 0) (=number? b 0)) 0)
    ((=number? a 1) b)
    ((=number? b 1) a)
    (else (list (if (compound-expression? a) (list a) a) '* (if (compound-expression? b) (list b) b)))
  )
)

(define (multiplicand exp) 
  (define rv (left-operand exp '*))
  (newline)
  (display "exp ")
  (display exp)
  (display " multiplicand ")
  (display rv)
  rv
)

(define (multiplier exp) 
  (define rv (right-operand exp '*))
  (newline)
  (display "exp ")
  (display exp)
  (display " multiplier ")
  (display rv)
  rv
)

(define (product? exp)
  (and (pair? exp) (eq? '* (next-operation exp)))
)

(define (make-exponentiation base exponent)
  (cond
    ((=number? exponent 0) 1)
    ((=number? exponent 1) base)
    (else (list base '** exponent))
  )
)

(define (base exp) 
  (left-operand exp '**)
)

(define (exponent exp) 
  (right-operand exp '**)
)

(define (exponentiation? exp)
  (and (pair? exp) (eq? '** (next-operation exp)))
)

(define (deriv exp var) 
  (newline)
  (display "deriv ")
  (display exp)
  (cond 
    ((number? exp) 0)
    ((variable? exp) 
      (if (same-variable? exp var) 1 0)) 
    ((sum? exp) 
      (newline)
      (display "IS SUM ")
      (display exp)
      (make-sum (deriv (addend exp) var)
                (deriv (augend exp) var)))
    ((product? exp)
      (newline)
      (display "IS PRODCUT ")
      (display exp)
      (make-sum (make-product (multiplier exp)
                              (deriv (multiplicand exp) var))
                (make-product (deriv (multiplier exp) var)
                              (multiplicand exp))))
    ((exponentiation? exp) 
      (make-product
        (make-product (exponent exp) 
                      (make-exponentiation (base exp) (- (exponent exp) 1)))
        (deriv (base exp) var)
      )
    )
    (else (error "unknown expression type: DERIV" exp))
  )
)

(deriv '(x + 3 * (x + y + 2)) 'x)
(deriv '(4 * x + 3 * (x + y + 2)) 'x)

