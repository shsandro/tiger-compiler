SRC_FOLDER=src

SRC_OBJS=$(SRC_FOLDER)/tiger.tab.o  $(SRC_FOLDER)/lex.yy.o $(SRC_FOLDER)/error.o $(SRC_FOLDER)/util.o $(SRC_FOLDER)/absyn.o $(SRC_FOLDER)/table.o $(SRC_FOLDER)/symbol.o $(SRC_FOLDER)/parse.o $(SRC_FOLDER)/semant.o $(SRC_FOLDER)/types.o $(SRC_FOLDER)/env.o $(SRC_FOLDER)/prabsyn.o $(SRC_FOLDER)/temp.o $(SRC_FOLDER)/translate.o $(SRC_FOLDER)/mipsframe.o $(SRC_FOLDER)/escape.o $(SRC_FOLDER)/printtree.o $(SRC_FOLDER)/tree.o $(SRC_FOLDER)/canon.o $(SRC_FOLDER)/assem.o $(SRC_FOLDER)/mipscodegen.o

all: src $(SRC_OBJS)
	cd $(SRC_FOLDER) && make
	gcc -g tigerc.o $(SRC_OBJS) -o tigerc

.PHONY src: tigerc.c
	gcc -g -c tigerc.c

clean:
	cd $(SRC_FOLDER) && make clean
	rm -rf tigerc tigerc.o
