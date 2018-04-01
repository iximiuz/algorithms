(define us-coins (list 50 25 10 5 1))
(define uk-coins (list 100 50 20 10 5 2 1 0.5))

(define (cc amount coin-values)
  ;;; (+ (cc amount (crd coin-values)) (cc (- amount (car coin-values) coin-values)))
  (cond
    ((null? coin-values) 0)
    ((> 0 amount) 0)
    ((= 0 amount) 1)
    (else (+ (cc amount (cdr coin-values)) (cc (- amount (car coin-values)) coin-values)))
  )
)

(cc 100 us-coins)

