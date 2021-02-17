# TEST=test

# lextest: scanner_driver.o lex.yy.o error.o util.o
# 	gcc -g -o lextest scanner_driver.o lex.yy.o error.o util.o
# 	make clean

# scanner_driver.o: $(TEST)/scanner_driver.c token.h error.h util.h
# 	gcc -g -c $(TEST)/scanner_driver.c

# error.o: error.c error.h util.h
# 	gcc -g -c error.c

# lex.yy.o: lex.yy.c token.h error.h util.h
# 	gcc -g -c lex.yy.c

# lex.yy.c: scanner.l
# 	flex scanner.l

# util.o: util.c util.h
# 	gcc -g -c util.c

# clean: 
# 	rm -f a.out util.o $(TEST)/scanner_driver.o lex.yy.o lex.yy.c error.o

####################################3

a.out: parsetest.o y.tab.o lex.yy.o error.o util.o absyn.o table.o symbol.o 
	gcc -g parsetest.o y.tab.o lex.yy.o error.o util.o absyn.o table.o symbol.o 

parsetest.o: parsetest.c error.h util.h
	gcc -g -c parsetest.c

absyn.o: absyn.c util.h symbol.h absyn.h
	gcc -g -c absyn.c

table.o: table.c table.h util.h
	gcc -g -c table.c

symbol.o: symbol.c symbol.h util.h table.h
	gcc -g -c symbol.c

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

y.tab.c: tiger.y
	yacc -dv tiger.y

y.tab.h: y.tab.c 
	echo "y.tab.h was created at the same time as y.tab.c"

error.o: error.c error.h util.h
	gcc -g -c error.c

lex.yy.o: lex.yy.c y.tab.h error.h util.h
	gcc -g -c lex.yy.c

lex.yy.c: scanner.l
	lex scanner.l

util.o: util.c util.h
	gcc -g -c util.c

clean: 
	rm -f a.out parsetest.o y.tab.o lex.yy.o error.o util.o absyn.o symbol.o table.o
