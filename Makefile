CC = gcc
CFLAGS = -w

all: compiler

parser.tab.c parser.tab.h: parser.y common.h
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	lex lexer.l

compiler: lex.yy.c parser.tab.c main.c symtab.c tac.c tac_ast.c optimizer.c codegen.c common.h
	$(CC) $(CFLAGS) -o compiler main.c symtab.c tac.c tac_ast.c optimizer.c codegen.c lex.yy.c parser.tab.c

clean:
	rm -f compiler lex.yy.c parser.tab.c parser.tab.h *.o

run: compiler
	./compiler < test1_arithmetic.c
