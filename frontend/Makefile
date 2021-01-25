TEST=test

lextest: scanner_driver.o lex.yy.o error.o util.o
	cc -g -o lextest scanner_driver.o lex.yy.o error.o util.o
	make clean

scanner_driver.o: $(TEST)/scanner_driver.c token.h error.h util.h
	cc -g -c $(TEST)/scanner_driver.c

error.o: error.c error.h util.h
	cc -g -c error.c

lex.yy.o: lex.yy.c token.h error.h util.h
	cc -g -c lex.yy.c

lex.yy.c: scanner.l
	flex scanner.l

util.o: util.c util.h
	cc -g -c util.c

clean: 
	rm -f a.out util.o $(TEST)/scanner_driver.o lex.yy.o lex.yy.c error.o
