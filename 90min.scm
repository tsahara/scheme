(use srfi-1)

(define (read-toplevel-exps)
  (until (read) eof-object? => exp
	 (print exp)))

(define (main0 args)
  (with-input-from-file (second args)
    read-toplevel-exps))

(if #f
    (let loop ((v1 (exp1)))
      (if (exp2 v1)
	  #f
	  (begin
	    body
	    (loop (exp1))))))

(let ((exp1 read-line)
      (exp2 eof-object?))
  ((lambda (loop) (loop loop (exp1)))
   (lambda (loop v1)
     (if (exp2 v1)
	 #f
	 (begin
	   (print v1)
	   (loop loop (exp1)))))))
