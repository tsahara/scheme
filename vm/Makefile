SRCS=gen.c main.c mem.c s_bytevector.c s_nil.c s_pair.c vcpu.c vm.c
OBJS=gen.o main.o mem.o s_bytevector.o s_nil.o s_pair.o vcpu.o vm.o
PROG=vm

CFLAGS= -g -O -Wall -Werror
LDFLAGS=

.c.o:
	$(CC) -c $(CFLAGS) -I. -o $@ $<

all: $(PROG)

$(OBJS): vm.h

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(OBJS) $(PROG)

.PHONY: check-syntax
check-syntax:
	gcc -Wall -fsyntax-only $(CHK_SOURCES)
