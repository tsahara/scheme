(use gauche.uvector)

(load "./writer.scm")

;;; scheme-object-file = (toplevel-closure)
;;; toplevel-closure = closure
;;; closure = (bytecode display-vector symbol-list)

(define (assemble-a-instruction l)
  (case #?=(car l)
    ((RET) #x00000004)
    (else (error "unknown instruction"))))

(define (assemble lines)
  (map (lambda (insn)
	 (map (cut remainder <> 256)
	      (u8vector (quotient insn (* 256 256 256))
			(quotient insn (* 256 256))
			(quotient insn 256)
			insn)))
       (map assemble-a-instruction lines)))

(define (make-empty-closure)
  (list #?=(assemble '((RET)))
	#()
	'()))

(call-with-output-file "a.sobj"
  (lambda (out)
    (let1 as (make-address-space)
      (as-put as (make-empty-closure))
      (print as)
      (write-address-space as out))))
