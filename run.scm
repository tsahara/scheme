(use gauche.sequence)
(use srfi-13)
(use srfi-60)

;;
;; direct compile version
;;
(define (compile-scm-file0 scm-file asm-file)
  (format #t "~a => ~a\n" scm-file asm-file)
  (call-with-input-file scm-file
    (lambda (in)
      (call-with-output-file asm-file
	(lambda (out)
	  (format out ".globl _main\n")
	  (format out "_main:\n")

	  (let ((exp (read in)))
	    (cond ((integer? exp)
		   (compile-integer-expression exp out))
		  (else (error "cannot compile ~a" exp))))

	  (format out "retq\n"))))))

(define (integer->sval num)
  (ash num 3))


;;
;; tac = Three Address Code
;;
(define (scheme-exp->tac exp)
  (cond ((integer? exp)
	 (list `(value ,exp)))))

(define (assemble-tac list-of-tac)
  (for-each (lambda (tac)
	      (case (car tac)
		((value) (print (cadr tac))))
	      )
	    list-of-tac))

(define (tac-result-register tac)
  ;; result register of the last tac.
  (car (last tac)))

;;
;; tac-block
;;
(define-class <tac-block> ()
  (procedure-name regs))



;;
;; regs
;;
(define (make-regs)
  0)

(define (newregs regs)
  (values (format #f "t~a" regs) (+ 1 regs)))

;;
;; intermediate code version
;;
(define (compile-scm-file scm-file asm-file)
  (format #t "compile: ~a => ~a\n" scm-file asm-file)
  (call-with-input-file scm-file
    (lambda (in)
      (call-with-output-file asm-file
	(lambda (out)
	  (let ((exp  (read in))
		(regs (make-regs)))
	    (codegen (compile-expression-into-tac exp regs)
		     out))
	  )))))

(define (compile-expression-into-tac exp regs)
  (cond ((integer? exp) (compile-integer-into-tac exp regs))
	((pair? exp)    (compile-pair-into-tac exp regs))
	(else (error "cannot compile ~a" exp))))

(define (compile-integer-expression)
  (format out "movq $~d, %rax\n"
	  (integer->sval exp)))

(define (compile-integer-into-tac exp regs)
  (receive (t regs)
      (newregs regs)
    (values (list (list t 'imm (ash exp 4)))
	    regs)))

(define (scheme-symbol->asm-label sym)
  (case sym
    ((+) 'plus)
    (else (errorf "unknown scheme-symbol: ~a" sym))))

(define (compile-pair-into-tac exp regs)
  ;; TODO: it can be syntax

  ;; 1. evaluate parameters
  (let ((param-tacs (map (lambda (e)
			   (receive (t r)
			       (compile-expression-into-tac e regs)
			     (set! regs r)
			     t))
			 (cdr exp))))
    (print "param-tacs => " param-tacs)

    ;; 2. evaluate first argument
    ;; XXX assumes 1st argument is a symbol.

    ;; 3. set parameters conforming calling convensions.
    (let ((push-tacs (map-with-index (lambda (reg idx)
				       (list #f 'param idx reg))
				     (map tac-result-register param-tacs))))
      (print "push-tacs => " push-tacs)

      (append param-tacs
	      push-tacs
	      (list (list #f 'call (scheme-symbol->asm-label (car exp))))))))

;;
;; codegen - Machine Code Generator
;;
(define (codegen tac-list port)
  (format port ".globl _main\n")
  (format port "_main:\n")
  (format port "movq $123, %rax\n")
  (format port "retq\n"))

;;

(define (assemble-and-link asm-file)
  (let ((cmd (format #f "cc -Wall ~a -Lbuild/clib -lclib" asm-file)))
    (format #t "~a => a.out : ~a\n" asm-file cmd)
    (sys-system cmd)))

(define (run-aout)
  (print "./a.out")
  (sys-exec "./a.out" '("./a.out")))

(define (run-a-script scm-file)
  (define (scm->asm filename)
    (string-append (if (string-suffix? ".scm" filename)
		       (string-drop-right filename 4)
		       filename)
		   ".s"))

  (let ((asm-file (scm->asm scm-file)))
    (compile-scm-file scm-file asm-file)
    (assemble-and-link asm-file)
    (run-aout)))

(define (usage)
  (print "usage: run.scm <a.scm>"))

(define (main args)
  (unless (= (length args) 2)
	  (begin
	    (usage)
	    (exit 1)))
  (run-a-script (cadr args)))
