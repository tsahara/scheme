(use gauche.parseopt)
(use gauche.sequence)
(use srfi-13)
(use srfi-60)

(define-class <scheme-program> ()
  ((next-procedure-id :init-value 0)
   ))


(define (integer->sval num)
  (ash num 3))

;;
;; Compile-time Environment
;;
(define-class <compiler-environment> ()
  ((alist  :init-value '())
   (parent :init-value #f)
   ))

(define (make-cenv)
  (make <compiler-environment>))

(define (cenv-add cenv symbol value)
  (slot-set! cenv 'alist
	     (assoc-set! (slot-ref cenv 'alist) symbol value)))

(define (cenv-print cenv)
  (format #t "Compiler Environment ~a\n" cenv)
  (for-each (lambda (pair)
	      (format #t "  ~a : ~a\n" (car pair) (cdr pair)))
	    (slot-ref cenv 'alist)))

;;
;; Three Address Code
;; tac := (operator val1 val2 result)
;; operator := ...?
;; val1, val2 := (a scheme object)
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

(define (tac-result tac)
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
(define-class <registers> ()
  ((index :init-value 0)))

(define (make-registers)
  (make <registers>))

(define (registers-alloc regs)
  (let ((reg (slot-ref regs 'index)))
    (slot-set! regs 'index (+ reg 1))
    reg))

(define (newregs regs)
  (values (format #f "t~a" regs) (+ 1 regs)))

(define-class <compiler-procedure> ()
  ((asm-symbol :init-form (asm-gensym))))

(define (make-compiler-procedure)
  (make <compiler-procedure>))

(define (cproc-asm-symbol cproc)
  (slot-ref cproc 'asm-symbol))

;; Temporary Variable
(define (temporary-variable-alloc vars)
  )

;;
;; Compiler Entry Point
;;
(define (compile-scm-file scm-file asm-file)
  (format #t "compile: ~a => ~a\n" scm-file asm-file)
  (call-with-output-file asm-file
    (lambda (out)
      ;; XXX pass 1 to read all toplevel procedure definitions.
      (call-with-input-file scm-file
	(lambda (in)
	  (let ((prog (make <scheme-program>))
		(cenv (make-cenv))
		(exp  (read in)))
	    (cond ((equal? (car exp) 'define)
		   (compile-procedure out prog cenv exp))

		  (else
		   (error "not a (define)")))

	    (format out ".global _main\n")
	    (format out "_main:\n")
	    (format out "    movq $123, %rax\n")
	    (format out "    retq\n")
	    )))))

  (format #t "<<< ~a >>>\n" asm-file)
  (copy-port (open-input-file asm-file) (current-output-port))
  )

(define (compile-procedure out prog cenv exp)
  (let ((decl (cadr exp))
	(body (caddr exp))
	(regs (make-registers))
	(cproc (make-compiler-procedure)))
    (let ((arity (- (length decl) 1)))
      (define (reg-alloc!) (inc! regs))

      (format #t "procedure: ~a\n" (car decl))
      (format #t "args: ~a\n" (cdr decl))
      (cenv-add cenv (car exp) cproc)

      (let ((last-tac (compile-expression body regs)))
	;; register allocation?
	;; (target-code-generate tac)

	(format out "~a:\n" (cproc-asm-symbol cproc))
	;; movq (register-tac-to-target (tac-result last-tac)), %rax
	(format out "    retq\n")))
    ))

(define (compile-expression exp regs)
  (values `(add ,(registers-alloc regs) 1 2)))

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
(define asm-gensym
  (let ((symbol-id 0))
    (lambda ()
      (string-append "_s_" (number->string (inc! symbol-id))))))

;;

(define (assemble-and-link asm-file)
  (let ((cmd (format #f "cc -Wall ~a -Lbuild/clib -lclib" asm-file)))
    (format #t "~a => a.out : ~a\n" asm-file cmd)
    (sys-system cmd)))

(define (run-aout)
  (print "./a.out")
  (sys-exec "./a.out" '("./a.out")))

(define (run-a-script scm-filename)
  (let ((asm-filename (string-append (if (string-suffix? ".scm" scm-filename)
					 (string-drop-right scm-filename 4)
					 scm-filename)
				     ".s")))
    (compile-scm-file scm-filename asm-filename)
    #;(assemble-and-link asm-filename)
    #;(run-aout)))

(define (print-usage prog)
  (format #t "usage: ~a <program.scm>\n" prog)
  (exit 0))

(define (main args)
  (let-args (cdr args)
      ((help        "h|help")
       . restargs)

    (if help
	(print-usage (car args)))

    (run-a-script (car restargs))))
