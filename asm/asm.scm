(use binary.io)
(use gauche.uvector)

;; emit one-word instruction, and add something to reloc/...
(define (assemble-an-instruction line)
  (case (car line)
    ((ret) #x00000004)
    ((nop) #xffffffff)
    (else (errorf "unknown instruction: ~a" (car line)))))

(define (assemble code out)
  (for-each (lambda (line)
	      (write-u32 (assemble-an-instruction line) out))
	    code))

(define (assemble-to-file code outfile)
  (call-with-output-file outfile
    (cut assemble code <>)))

(begin
  (use file.util)
  (current-directory "/Users/sahara/src/scheme")
  (let1 program '((ret))
    (assemble-to-file program "a.sobj"))
  )

(define (main args)
  (default-endian 'big-endian)
  (assemble-to-file (port->string (open-input-file (car args)))
		    "a.sobj"))
