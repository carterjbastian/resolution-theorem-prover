CC = gcc
CFLAGS = -g
BISONFL = -d -v
FLEXFLAGS = -ll

.PHONY: clean 

.SUFFIXES: .c

parse : lex.yy.o parseKB.tab.o lst.o transformations.o parseKB_main.o resolution.o set.o
	$(CC) -o $@ $(CFLAGS) lex.yy.o parseKB.tab.o lst.o transformations.o set.o resolution.o parseKB_main.o  $(FLEXFLAGS)

unifyTest : lex.yy.o parseKB.tab.o lst.o transformations.o unify_main.o resolution.o set.o
	$(CC) -o $@ $(CFLAGS) lex.yy.o parseKB.tab.o lst.o transformations.o set.o resolution.o unify_main.o  $(FLEXFLAGS)

unify_main.o : unify_main.c
	$(CC) -c $(CFLAGS) unify_main.c

resolution.o : resolution.c
	$(CC) -c $(CFLAGS) resolution.c

set.o : set.c
	$(CC) -c $(CFLAGS) set.c

lst.o : lst.c
	$(CC) -c $(CFLAGS) lst.c

transformations.o : transformations.c
	$(CC) -c $(CFLAGS) $<

lex.yy.o : KBscan.l
	flex $<
	$(CC) -c lex.yy.c

parseKB.tab.o : parseKB.tab.c
	$(CC) -c $(CFLAGS) $<

parseKB.tab.h : parseKB.y
	bison $(BISONFL) $<

parseKB.tab.c : parseKB.y
	bison $(BISONFL) $<

parseKB_main.o : parseKB_main.c
	$(CC) -c $(CFLAGS) $<

clean :
	rm -f parse *.o parseKB.tab.* lex.yy.c *.output unifyTest

depend :
	makedepend -- $(CFLAGS) -- parseKB_main.c

# DO NOT DELETE THIS LINE -- make depend depends on it.
