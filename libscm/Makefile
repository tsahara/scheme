SRCS=fixnum.c string.c
OBJS=fixnum.o string.o

CPPFLAGS=-Wall

libscm.a: ${OBJS}
	ar -r libscm.a ${OBJS}

clean:
	-rm libscm.a ${OBJS}
