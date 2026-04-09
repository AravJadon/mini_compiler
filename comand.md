# How to Build and Run the Mini Compiler

## Build (one-time, or after any code change)
```bash
make clean && make
```

## Or build manually (without Makefile)
```bash
bison -d parser.y
lex lexer.l
gcc -w -o compiler main.c symtab.c tac.c lex.yy.c parser.tab.c
```

## Run on a test file
```bash
./compiler < test_simple.c
```

## Save output to a file
```bash
./compiler < test_simple.c > output.txt
cat output.txt
```

## Run on other test files
```bash
./compiler < test_case_all.c > output_full.txt
cat output_full.txt
```