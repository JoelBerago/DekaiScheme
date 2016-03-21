(define > (lambda (x y) (< y x)))
(define >= (lambda (x y) (not (< x y))))
(define <= (lambda (x y) (not (< y x))))
(define = (lambda (x y) (if (< x y) 0 (not (< y x)))))
(define abs (lambda (x) (if (< x 0) (- 0 x) x)))
(define factorial (lambda (x) (if (< x 2) 1 (* x (factorial (- x 1))))))
(define square (lambda (x) (print (* x x))))

(define for-each 
  (lambda (x y) 
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
	(quote ()))))

(define make-list (lambda args args))
(define ' (lambda args args))

(define first (lambda args (car args)))

(define is-great 
  (lambda (x) 
    (print (cons x (quote (is great))))))

(define flavors 
  (lambda flavors 
    (for-each is-great (first flavors))))

(define length
  (lambda (list)
    (if (nullp list)
	0
	(+ 1 (length (cdr list))))))

(define list-tail
  (lambda (list k) 
    (if (= k 0)
	list
	(list-tail (cdr list) (- k 1)))))

(define list-ref
  (lambda (list k) 
    (car (list-tail list k))))

(define appendback 
  (lambda (list1 list2)
    (if (nullp list1) 
	list2
	(cons (car list1) (append (cdr list1) list2)))))

(define appendfront
  (lambda (list1 list2)
    (if (nullp list1)
	list2
	(append list2 list1))))

(define append
  (lambda (list1 list2)
    (appendback list1 list2)))

(define reverse
  (lambda (list)
    (if (nullp (list-tail list 2))
	(cons (car (list-tail list 1))
	      (' (list-ref list 0)))
	(append (reverse (list-tail list 1)) (' (list-ref list 0))))))

(define % 
  (lambda (n m)
    (if (< n m)
	n
	(% (- n m) m))))

(define even?
  (lambda (x)
    (if (= (% x 2) 0)
	1
	0)))

(define odd?
  (lambda (x)
    (not (even? x))))

(define list-partition-recurse
  (lambda (proc orig_list left right)
    (if (nullp (list-tail orig_list 1))
	(if (proc (car orig_list))
	    (' (append left (' (car orig_list))) right)
	    (' left (append right (' (car orig_list)))))
	(if (proc (car orig_list))
	    (list-partition-recurse proc (list-tail orig_list 1) (append left (' (car orig_list))) right)
	    (list-partition-recurse proc (list-tail orig_list 1)  left (append right (' (car orig_list))))))))

(define list-partition 
  (lambda (proc list)
    (list-partition-recurse proc list (' ) (' ))))

(define lazy-equal? 
  (lambda (obj1 obj2)
    (if (= obj1 obj2) 1 0)))

(define list-equal? 
  (lambda (list1 list2)
    (if (nullp list1)
	(define proceed 0)
	(define proceed (not (nullp list2))))
    (if proceed
	(define a (car list1))
	0)
    (if proceed
	(define b (car list2))
	0)
    (if proceed
	(if (nullp (list-tail list1 1))
	    (if (nullp (list-tail list2 1))
		(equal? a b)
		0)
	    (if (equal? a b)
		(list-equal? (list-tail list1 1) (list-tail list2 1))
		0))
	0)))

(define equal? 
  (lambda (obj1 obj2)
    (if (nullp obj1)
	(if (nullp obj2)
	    1
	    0)
	(if (intp obj1)
	    (if (intp obj2)
		(lazy-equal? obj1 obj2)
		0)
	    (if (doublep obj1)
		(if (doublep obj2)
		    (lazy-equal? obj1 obj2)
		    0)
		(if (symbolp obj1)
		    (if (symbolp obj2)
			(lazy-equal? obj1 obj2)
			0)
		    (if (listp obj1)
			(if (listp obj2)
			    (list-equal? obj1 obj2)
			    0))))))))

(define assoc 
  (lambda (obj alist)
    (if (nullp alist)
	(define proceed 0)
	(define proceed 1))
    (if proceed
	(define pair (car alist))
	0)
    (if proceed
	(if (equal? (car pair) obj)
	    pair
	    (assoc obj (list-tail alist 1)))
	0)))

(define get-last 
  (lambda (list)
    (if (nullp (list-tail list 1))
	(car list)
	(get-last (list-tail list 1)))))

(define remove-last 
  (lambda (list)
    (reverse (list-tail (reverse list) 1))))

(define list-to-sort 
  (quote (4 5 1 2 7 9 6)))

(define list-sort-broken
  (lambda (comparator list)
    (if (nullp list)
	(define proceed1 0)
	(define proceed1 1))

    (if proceed1
	(if (nullp (list-tail list 1))
	    (define proceed2 0)
	    (define proceed2 1))
	(define proceed2 0))

    (if proceed2
	(define pivot (get-last list))
	0)

    (if proceed2
	(define part-by-pivot
	  (lambda (to_compare)
	    ((eval comparator) to_compare pivot)))
	0)
	  
    (if proceed2
	(define result
	  (list-partition part-by-pivot (remove-last list)))
	0)

    (if proceed2
	(define left (append (list-sort comparator (car result)) (' pivot)))
	0)
    
    (if proceed2
	(if (nullp (cdr result))
	    (define proceed3 0)
	    (define proceed3 1))
	(define proceed3 0))

    (if proceed3
	(define right (list-sort comparator (car (cdr result))))
	0)

    (if proceed3
	(append left right)
	list)))
