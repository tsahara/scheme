(use srfi-1)

;; (define (regmap-alloc ireg) ...)
;; (define (regmap-release mreg))
;; (define (regmap-used-registers regmap))

(define-class <register-map> ()
  ()
  )

(define (make-register-map)
  (let1 regmap (make <register-map>)
      ))

(define a-icode
  ; (print (+ 1 2))
  '(proc sa_toplevel 4
	 ((fixnum r1 1)
	  (fixnum r2 2)
	  (add r3 r1 r2)
	  (call _print r4 r3)
	  (return r4)
	  )))

(define (reg-get ir)
  (case ir
    ((r1) "%r11")
    ((r2) "%r10")
    ((r3) "%r9")
    ((r4) "%r8")
    (else (error "xxx: reg-get"))))

;; prologue: save registers, make stack frame
;; body: main
;; epilogue: restore everything then return

(define (codegen-a-icode out icode-block)
  (let ((label (second icode-block))
	(nregs (third  icode-block)))
    (format out ".global ~a\n" label)
    (format out "~a:\n" label)

    (format out "    pushq %rbp\n")
    (if (> nregs 0)
	(format out "    subq $~a, %rbp\n" (* nregs 8)))

    (for-each
     (^ (icode)
       (case (car icode)
	 ((fixnum) (let ((mr1 (reg-get (second icode)))
			 (num (third icode)))
		     (format out "    movq $~d, ~a\n" num mr1)))
	 ((add) (let ((mr1 (reg-get (second icode)))
		      (mr2 (reg-get (third  icode)))
		      (mr3 (reg-get (fourth icode))))
		  (format out "    movq ~a, %rax\n" mr2)
		  (format out "    addq ~a, %rax\n" mr3)
		  (format out "    mvoq %rax, ~a\n" mr1)))

       ))
     (fourth icode-block))

    (if (> nregs 0)
	(format out "    addq $~a, %rbp\n" (* nregs 8)))
    (format out "    popq %rbp\n")
    (format out "    retq\n")
    ))

(codegen-a-icode (current-output-port) a-icode)
