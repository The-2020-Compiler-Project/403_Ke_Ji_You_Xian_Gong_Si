CC = g++

OBJS =main.o analyze.o cgen.o code.o parse.o scan.o symtab.o util.o

cx-compiler: $(OBJS)
	$(CC) $(OBJS) -o cx-compiler

main.o: main.cpp globals.h util.h scan.h analyze.h cgen.h parse.h
	$(CC) -c main.cpp

analyze.o: analyze.cpp globals.h symtab.h analyze.h util.h
	$(CC) -c analyze.cpp

cgen.o: cgen.cpp cgen.h code.h util.h symtab.h globals.h
	$(CC) -c cgen.cpp

code.o: code.cpp code.h globals.h
	$(CC) -c code.cpp

parse.o: parse.cpp parse.h scan.h util.h globals.h
	$(CC) -c parse.cpp

scan.o: scan.cpp scan.h util.h globals.h
	$(CC) -c scan.cpp

symtab.o: globals.h symtab.h symtab.cpp
	$(CC) -c symtab.cpp

util.o: util.cpp globals.h util.h
	$(CC) -c util.cpp

clean:
	-rm cx-compiler
	-rm $(OBJS)

test: cm-compile
	-./cx-compiler test.cm

all: cx-compiler
