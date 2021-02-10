include ../../Makefile.compilation

all: parser.tab.cc scanner.yy.cc

scanner.yy.cc: scanner.yy
	$(LEX) -o scanner.yy.cc scanner.yy

parser.tab.cc $(GEN_H): parser.yy
	$(YACC) parser.yy

scanner.yy:;

parser.yy:;