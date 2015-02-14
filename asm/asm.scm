(define (assemble code out)
  (for-each (lambda (line)
	      (print line))
	    code))

(define program
  '((ret)))

(define (main args)
  (call-with-output-file "a.obj"
    (lambda (out)
      (assemble program out))))
