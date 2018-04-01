(define (smallets-divisor n)
  (define (find-divisor a) 
    (cond 
      ((> (* a a) n) n)
      ((= (remainder n a) 0) a)
      (else (find-divisor (+ a 1)))
    )
  )

  (find-divisor 2)
)

(define (prime? n) 
  (= n (smallets-divisor n))
)

(define (timed-prime-test n) 
  ;;;(newline)
  ;;;(display n)
  (start-prime-test n (runtime)))

(define (start-prime-test n start-time) 
  (if (prime? n)
    (report-prime (- (runtime) start-time)) 
    false
  )
)

(define (report-prime elapsed-time)
  (display " *** ")
  (display elapsed-time)
  (newline)
  true
)

(define (search-for-primes start-from amount)
  (if (= 0 (remainder start-from 2)) 
    (search-for-primes (+ start-from 1) amount)
    (if (> amount 0)
      (if (timed-prime-test start-from)
        (search-for-primes (+ start-from 2) (- amount 1))
        (search-for-primes (+ start-from 2) amount)
      )
    )
  )
  false
)

(search-for-primes 10000000000 3)
(search-for-primes 100000000000000 3)
(search-for-primes 10000000000000000 3)

