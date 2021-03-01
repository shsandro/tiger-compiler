FRONTEND_FOLDER=frontend

FRONTEND_OBJS=$(FRONTEND_FOLDER)/tiger.tab.o  $(FRONTEND_FOLDER)/lex.yy.o $(FRONTEND_FOLDER)/error.o $(FRONTEND_FOLDER)/util.o $(FRONTEND_FOLDER)/absyn.o $(FRONTEND_FOLDER)/table.o $(FRONTEND_FOLDER)/symbol.o $(FRONTEND_FOLDER)/parse.o $(FRONTEND_FOLDER)/semant.o $(FRONTEND_FOLDER)/types.o $(FRONTEND_FOLDER)/env.o $(FRONTEND_FOLDER)/prabsyn.o

all: frontend $(FRONTEND_OBJS)
	cd $(FRONTEND_FOLDER) && make
	gcc -g tigerc.o $(FRONTEND_OBJS) -o tigerc

frontend: tigerc.c
	gcc -g -c tigerc.c

clean:
	cd $(FRONTEND_FOLDER) && make clean
	rm -rf tigerc tigerc.o
