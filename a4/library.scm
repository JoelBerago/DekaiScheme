(define > (lambda (x y) (< y x)))
(define >= (lambda (x y) (not (< x y))))
(define <= (lambda (x y) (not (< y x))))
(define = (lambda (x y) (if (< x y) 0 (not (< y x)))))
(define abs (lambda (x) (if (< x 0) (- 0 x) x)))
(define factorial (lambda (x) (if (< x 2) 1 (* x (factorial (- x 1))))))
(define square (lambda (x) (print (* x x))))
(define for-each (lambda (x y) 
		   (if (nullp y)
		       (define a 0)
		       (define a 1))
		   (if a
		       (apply x (list (car y)))
		       (quote ()))
		   (if a
		       (if (nullp (cdr y))
			   (quote ())
			   (for-each x (cdr y)))
		       (quote ()))
		   ))

(define list--comment
  (quote ("(list args1 args2) function is essentially doing the same thing as (quote (args1 args2))")))
(define list (lambda args args))


(define first (lambda args (car args)))
(define last (lambda args 
	       (if (nullp (cdr args))
		   (car args)
		   (last (cdr args)))
	       ))

(define is-great (lambda (x) 
		   (print (cons x (quote (is great))))
		   ))
(define flavors (lambda flavors 
		  (for-each is-great (first flavors))
		  ))
