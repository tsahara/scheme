(use binary.io)
(use gauche.uvector)


;; emit one-word instruction, and add something to reloc/...
(define (assemble-an-instruction line)
  (case (car line)
    ((ret) #x00000004)
    (else (errorf "unknown instruction: ~a" (car line)))))

(define (assemble code out)
  (for-each (lambda (line)
	      (write-u32 (assemble-an-instruction line) out))
	    code))

(define program
  '((ret)))

(define (main args)
  (default-endian 'big-endian)
  (call-with-output-file "a.obj"
    (lambda (out)
      (assemble program out))))
