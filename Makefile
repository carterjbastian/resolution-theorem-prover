CC = gcc
FLEXFLAGS = -ll
COMPLETEFLAGS = -D FULL_BUILD_
# use -lfl for Linux; -ll for OSX

.SUFFIXES: .c

lextest: lex.yy.c KBscan_main.c lst.c
	$(CC) -o $@ lex.yy.c lst.c KBscan_main.c $(FLEXFLAGS)

lex.yy.c: KBscan.l
	flex $<

clean:
	rm -f lex.yy.* *.o lextest
