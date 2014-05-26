(use binary.io)
(use gauche.uvector)

(load "./writer.scm")

(define (instruction->u8vector insn)
  (rlet1 uv (make-u8vector 4)
    (put-u32! uv 0 insn)))

(define (assemble-a-instruction line)
  (case (car line)
    ((RET) #x00000004)

    ((LOADI) (let1 num (cadr line)
	       (unless (integer? num)
		 (errorf "y must be an integer: ~a" line))
	       (unless (= (remainder num 8) 0)
		 (errorf "y must be a multiple of 8: ~a" line))
	       num))

    (else (error "unknown instruction"))))

(define (assemble lines)
  (let* ((insn-list (map assemble-a-instruction lines))
	 (bytes     (make-u8vector (* (length insn-list) 4))))
    (begin
      (map-with-index (lambda (i insn)
			(u8vector-copy! bytes (* i 4)
					(instruction->u8vector insn)))
		      insn-list)
      bytes)))
  
(define (make-empty-closure as)
  (let1 sym-write (as-put as 'write)
    (list (assemble '((LOADI 0)
		      (RET)))
	  #()
	  `((sym-write 0)))))

(default-endian 'big-endian)
(call-with-output-file "a.sobj"
  (lambda (out)
    (let1 as (make-address-space)
      (as-put as (cons #f #f))
      (let1 toplevel-closure (as-put as (make-empty-closure as))
	(as-word-set! as 4 toplevel-closure))
      (print as)
      (write-address-space as out))))
