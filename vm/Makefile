SRCS=env.c gen.c main.c mem.c s_bytevector.c s_cproc.c s_fixnum.c \
	s_nil.c s_pair.c s_symbol.c s_tag.c vcpu.c vm.c
OBJS=env.o gen.o main.o mem.o s_bytevector.o s_cproc.o s_fixnum.o \
	s_nil.o s_pair.o s_symbol.o s_tag.o vcpu.o vm.o
PROG=vm

CFLAGS= -g -O -Wall -Werror
LDFLAGS=

.c.o:
	$(CC) -c $(CFLAGS) -I. -o $@ $<

all: $(PROG) tags

$(OBJS): vm.h

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(OBJS) $(PROG)

tags: $(SRCS)
	ctags $(SRCS)

.PHONY: check-syntax
check-syntax:
	gcc -Wall -fsyntax-only $(CHK_SOURCES)
