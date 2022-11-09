#######
# Makefile for peep compiler
#######

.SUFFIXES: .h .c .o

CC= GCC -I.

all: clean peepAll

peepAll: peepcc.o testAll
#### executable must be called peepcc

exec: peepcc.o
	./peepcc ex1.peep

peepcc.o: peepcc.c peepcc.h
	gcc stack.c infix-to-postfix.c peepcc.c -o peepcc

infix.o: stack.c infix-to-postfix.c infix-to-postfix.h

stack.o: stack.c stack.h
	gcc stack.c -o stack.o

testAll: testStack.o testInfix.o

test: testAll
	./testStack.o
	./i2pTest.o

testInfix.o: stack.c infix.o
	gcc stack.c infix-to-postfix.c testi2p.c -o i2pTest.o

testStack.o: stack.c stack.h stacktest.c
	gcc stack.c stacktest.c -o testStack.o
	
peep: peepcc.o

stacktest: testStack.o
	./testStack.o
clean:
	rm -f *.o
	rm -f *.out
	rm -f *.hml
	rm -f peepcc
	rm -f i2pTest
	