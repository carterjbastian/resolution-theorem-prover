CC = gcc
CFLAGS = -g
BISONFL = -d -v
FLEXFLAGS = -ll

.PHONY: clean 

.SUFFIXES: .c

parse : lex.yy.o parser.tab.o lst.o lst_main.o
	$(CC) -o $@ $(CFLAGS) lex.yy.o parser.tab.o lst.o lst_main.o $(FLEXFLAGS)

lst.o :
	$(CC) -c lst.c

lex.yy.o : KBscan.l
	flex $<
	$(CC) -c $@ lex.yy.c

parser.tab.o : parser.tab.c
	$(CC) -c $(CFLAGS) $<

parser.tab.h : parseKB.y
	bison $(BISONFL) $<

parser.tab.c : parseKB.y
	bison $(BISONFL) $<

lst_main.o : lst_main.c
	$(CC) -c $(CFLAGS) $<

clean :
	rm -f parser *.o parser.tab.* lex.yy.c

depend :
	makedepend -- $(CFLAGS) -- ast_main.c

# DO NOT DELETE THIS LINE -- make depend depends on it.



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
