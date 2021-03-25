INCLUDE_FOLDER=include
TEST=test

all: semantictest parsertest lextest

tests: semantictest parsertest lextest
	rm -f scanner_driver.o parser_driver.o tiger.tab.o lex.yy.o error.o util.o absyn.o symbol.o table.o prabsyn.o parse.o semant.o types.o env.o mipsframe.o temp.o translate.o escape.o

semantictest: semantic_driver.o tiger.tab.o lex.yy.o error.o util.o absyn.o table.o symbol.o parse.o semant.o types.o env.o mipsframe.o temp.o translate.o escape.o
	gcc -g semantic_driver.o tiger.tab.o lex.yy.o error.o util.o absyn.o table.o symbol.o parse.o semant.o types.o env.o mipsframe.o temp.o translate.o escape.o -o semantictest

semantic_driver.o: $(TEST)/semantic_driver.c $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/absyn.h $(INCLUDE_FOLDER)/parse.h $(INCLUDE_FOLDER)/semant.h $(INCLUDE_FOLDER)/symbol.h
	gcc -g -c $(TEST)/semantic_driver.c

mipsframe.o: mipsframe.c $(INCLUDE_FOLDER)/frame.h $(INCLUDE_FOLDER)/util.h 
	gcc -g -c mipsframe.c

temp.o: temp.c $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/table.h $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/temp.h 
	gcc -g -c temp.c

translate.o: translate.c $(INCLUDE_FOLDER)/frame.h $(INCLUDE_FOLDER)/translate.h
	gcc -g -c translate.c

escape.o: escape.c $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/absyn.h $(INCLUDE_FOLDER)/escape.h 
	gcc -g -c escape.c

semant.o: semant.c $(INCLUDE_FOLDER)/env.h $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/types.h $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/semant.h $(INCLUDE_FOLDER)/absyn.h $(INCLUDE_FOLDER)/frame.h $(INCLUDE_FOLDER)/translate.h $(INCLUDE_FOLDER)/semant.h
	gcc -g -c semant.c

types.o: types.c $(INCLUDE_FOLDER)/types.h $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/types.h
	gcc -g -c types.c

env.o: env.c $(INCLUDE_FOLDER)/env.h $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/types.h $(INCLUDE_FOLDER)/translate.h
	gcc -g -c env.c

lextest: scanner_driver.o lex.yy.o error.o util.o
	gcc -g -o lextest scanner_driver.o lex.yy.o error.o util.o
	
scanner_driver.o: $(TEST)/scanner_driver.c $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/util.h
	gcc -g -c $(TEST)/scanner_driver.c

parsertest: parser_driver.o tiger.tab.o lex.yy.o error.o util.o absyn.o table.o symbol.o prabsyn.o parse.o
	gcc -g parser_driver.o tiger.tab.o lex.yy.o error.o util.o absyn.o table.o symbol.o prabsyn.o parse.o -o parsertest

parser_driver.o: $(TEST)/parser_driver.c $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/parse.h $(INCLUDE_FOLDER)/absyn.h
	gcc -g -c $(TEST)/parser_driver.c

absyn.o: absyn.c $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/absyn.h
	gcc -g -c absyn.c

prabsyn.o: prabsyn.c $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/absyn.h $(INCLUDE_FOLDER)/prabsyn.h
	gcc -g -c prabsyn.c

parse.o: parse.c $(INCLUDE_FOLDER)/util.h $(INCLUDE_FOLDER)/symbol.h $(INCLUDE_FOLDER)/error.h $(INCLUDE_FOLDER)/absyn.h $(INCLUDE_FOLDER)/parse.h
	gcc -g -c parse.c

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
	rm -f *.out *.o lextest parsertest semantictest
