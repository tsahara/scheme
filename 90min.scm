(use srfi-1)

(define (read-toplevel-exps)
  (until (read) eof-object? => exp
	 (print exp)))

(define (main args)
  (with-input-from-file (second args)
    read-toplevel-exps))
