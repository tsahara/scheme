(define-class <register-descriptor> ()
  ()
  )

(define (codegen out label arity nregs)
  (format out ".global ~a\n" label)
  (format out "~a:\n" label)
  (format out "    pushq %rbp\n")
  (if (> nregs 0)
      (format out "    subq $~a, %rbp\n" (* nregs 8)))

  (format out "    movq $123, %rax\n")
  (format out "    callq _scm_init\n")

  (if (> nregs 0)
      (format out "    addq $~a, %rbp\n" (* nregs 8)))
  (format out "    popq %rbp\n")
  (format out "    retq\n")
  )
