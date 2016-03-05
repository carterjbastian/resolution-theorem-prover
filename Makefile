CC = gcc
CFLAGS = -g
BISONFL = -d -v
FLEXFLAGS = -ll

.PHONY: clean 

.SUFFIXES: .c

parse : lex.yy.o parseKB.tab.o lst.o parseKB_main.o
	$(CC) -o $@ $(CFLAGS) lex.yy.o parseKB.tab.o lst.o parseKB_main.o $(FLEXFLAGS)

lst.o :
	$(CC) -c lst.c

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
	rm -f parse *.o parseKB.tab.* lex.yy.c *.output

depend :
	makedepend -- $(CFLAGS) -- parseKB_main.c

# DO NOT DELETE THIS LINE -- make depend depends on it.
