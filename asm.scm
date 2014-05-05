(use gauche.uvector)

(load "./writer.scm")

;;; scheme-object-file = (toplevel-closure)
;;; toplevel-closure = closure
;;; closure = (bytecode display-vector symbol-list)

(define (assemble-a-instruction line)
  (case (car line)
    ((RET) #x00000004)
    (else (error "unknown instruction"))))

(define (instruction->u8vector insn)
  (apply u8vector
	 (map (cut remainder <> 256)
	      (list (quotient insn (* 256 256 256))
		    (quotient insn (* 256 256))
		    (quotient insn 256)
		    insn))))

(define (assemble lines)
  (let* ((insn-list (map assemble-a-instruction lines))
	 (bytes     (make-u8vector (* (length insn-list) 4))))
    (begin
      (map-with-index (lambda (i insn)
			(u8vector-copy! bytes (* i 4)
					(instruction->u8vector insn)))
		      insn-list)
      bytes)))
  
(define (make-empty-closure)
  (list (assemble '((RET)))
	#()
	'()))

(call-with-output-file "a.sobj"
  (lambda (out)
    (let1 as (make-address-space)
      (as-put as (make-empty-closure))
      (print as)
      (write-address-space as out))))
