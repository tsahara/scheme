(use gauche.sequence)
(use gauche.uvector)

;;; address-space   (buffer size relocation-list symbol-table)
;;; relocation-list (address ...)
;;; symbol-table    ()

(define (make-address-space)
  (list (make-u8vector 30 0) 0 '() '()))

(define (as-allocate as size)
  (let1 address (cadr as)
    (set! (cadr as) (+ address size (remainder size 8)))
    address))

(define (as-put as obj)
  (cond ((integer? obj) (as-put-integer as obj))
	((pair?    obj) (as-put-pair    as obj))
	((null?    obj) (as-put-null    as obj))
	(else (errorf "cannot as-put object ~a" obj))))

(define (as-put-integer as num)
  (ash num 3))

(define (as-put-pair as pair)
  (let* ((addr (as-allocate as 8))
	 (a    (as-put as (car pair)))
	 (b    (as-put as (cdr pair))))
    (as-word-set! as addr a)
    (as-word-set! as (+ 4 addr) b)
    (logior addr 1)))

(define (as-put-null as num)
  #x0f)

(define (as-word-set! as address value)
  (for-each-with-index (lambda (i byte)
			 (u8vector-set! (car as)
					(+ address i)
					(remainder byte 256)))
		       (list (quotient value (* 256 256 256))
			     (quotient value (* 256 256))
			     (quotient value 256)
			     value)))

(define (write-address-space as out)
  (write-block (car as) out 0 (cadr as)))

(call-with-output-file "a.sobj"
  (lambda (out)
    (let1 as (make-address-space)
      (as-put as (list 1 2))
      (print as)
      (write-address-space as out))))
