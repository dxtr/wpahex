CC=clang
CFLAGS=-Wall -Os -pipe
LDFLAGS=-s -lcrypto
.SUFFIXES: .c .o
.c.o:
	${CC} ${CFLAGS} -c $<

wpahex: wpahex.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ wpahex.o

wpahex.o: wpahex.c
	${CC} ${CFLAGS} -c -o $@ $<

all: wpahex
clean:
	rm wpahex wpahex.o
