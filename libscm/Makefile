SRCS=fixnum.c init.c string.c
OBJS=fixnum.o init.o string.o

CPPFLAGS=-Wall

a.out: libscm.a main.c
	cc -o a.out main.c -L. -lscm

libscm.a: ${OBJS}
	ar -r libscm.a ${OBJS}

clean:
	-rm libscm.a ${OBJS}
