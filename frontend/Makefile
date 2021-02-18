INCLUDE_FOLDER=include
TEST=test

lextest: scanner_driver.o lex.yy.o error.o util.o
	gcc -g -o lextest.out scanner_driver.o lex.yy.o error.o util.o
	rm -f lex.yy.o error.o util.o scanner_driver.o

scanner_driver.o: $(TEST)/scanner_driver.c $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/util.h
	gcc -g -c $(TEST)/scanner_driver.c

parsetest: parsetest.o tiger.tab.o lex.yy.o error.o util.o absyn.o table.o symbol.o 
	gcc -g parsetest.o tiger.tab.o lex.yy.o error.o util.o absyn.o table.o symbol.o -o parsetest.out
	rm -f parsetest.o tiger.tab.o lex.yy.o error.o util.o absyn.o symbol.o table.o

parsetest.o: parsetest.c $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/util.h
	gcc -g -c parsetest.c

absyn.o: absyn.c $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/absyn.h
	gcc -g -c absyn.c

table.o: table.c $(INCLUDE_FOLDER)/table.h $(INCLUDE_FOLDER)/util.h
	gcc -g -c table.c

symbol.o: symbol.c $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/table.h
	gcc -g -c symbol.c

tiger.tab.o: tiger.tab.c
	gcc -g -c tiger.tab.c

tiger.tab.c: tiger.y
	bison -dv tiger.y

tiger.tab.h: tiger.tab.c 
	echo "tiger.tab.h was created at the same time as tiger.tab.c"

error.o: error.c $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/util.h
	gcc -g -c error.c

lex.yy.o: lex.yy.c tiger.tab.h $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/util.h
	gcc -g -c lex.yy.c

lex.yy.c: scanner.l
	flex scanner.l

util.o: util.c $(INCLUDE_FOLDER)/util.h
	gcc -g -c util.c

clean: 
	rm -f *.out *.o 
