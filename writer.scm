(use gauche.uvector)

(define (write-integer out num)
  (write-block (u32vector num) out 0 -1 'big-endian))

(define (write-scheme-object out obj)
  (cond ((integer? obj) (write-integer out obj))
	(else (errorf "cannot write object ~a" obj))))

(call-with-output-file "a.sobj"
  (lambda (out)
    (write-scheme-object out 1)))
