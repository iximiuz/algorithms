(define (variable? x) (symbol? x))

(define (same-variable? x y)
  (and (variable? x) (variable? y) (eq? x y))
)

(define (=number? exp num)
  (and (number? exp) (= exp num))
)

(define (make-sum a b) 
  (cond
    ((=number? a 0) b)
     ((=number? b 0) a)
    ((and (number? a) (number? b)) (+ a b))
    (else (list '+ a b))
  )
)

(define (addend exp) 
  (cadr exp)
)

(define (augend exp) 
  (let ((m (cddr exp)))
    (if (null? (cdr m)) 
      (car m)
      (cons '+ m)
    )
  )
)

(define (sum? exp)
  (and (pair? exp) (eq? '+ (car exp)))
)

(define (make-product a b) 
  (cond
    ((or (=number? a 0) (=number? b 0)) 0)
    ((=number? a 1) b)
    ((=number? b 1) a)
    (else (list '* a b))
  )
)

(define (multiplicand exp) 
  (cadr exp)
)

(define (multiplier exp) 
  (let ((m (cddr exp)))
    (if (null? (cdr m))
      (car m)
      (cons '* m)
    )
  )
)

(define (product? exp)
  (and (pair? exp) (eq? '* (car exp)))
)

(define (make-exponentiation base exponent)
  (cond
    ((=number? exponent 0) 1)
    ((=number? exponent 1) base)
    (else (list '** base exponent))
  )
)

(define (base exp) (cadr exp))

(define (exponent exp) (caddr exp))

(define (exponentiation? exp)
  (and (pair? exp) (eq? '** (car exp)))
)

(define (deriv exp var) 
  (cond 
    ((number? exp) 0)
    ((variable? exp) 
      (if (same-variable? exp var) 1 0)) 
    ((sum? exp) 
      (make-sum (deriv (addend exp) var)
                (deriv (augend exp) var)))
    ((product? exp)
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

(multiplier '(* 1 2 3 4))
(deriv '(+ x 3) 'x)
(deriv '(* x y) 'x)
(deriv '(* (* x y) (+ x 3)) 'x)
(deriv '(* x y (+ x 3)) 'x)
(deriv '(+ (+ (+ (* 3 (** x 3)) (* 2 (* 3 (* 4 (** x 2))))) (** x 1)) y) 'x)
(deriv '(+ (* 3 (** x 3)) (* 2 3 4 (** x 2)) (** x 1) y) 'x)
(deriv '(** x 1) 'x)
(deriv '(** x 3) 'x)

