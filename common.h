#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* tac instruction kinds */
typedef enum { INS_TEXT = 0, INS_IF, INS_GOTO } InsType;

typedef struct {
    InsType type;
    char text[256];
    int target;
} Instruction;

/* backpatching helpers */
typedef struct IntList { int index; struct IntList *next; } IntList;
typedef struct AAttr  { char *place; } AAttr;
typedef struct BAttr  { IntList *truelist; IntList *falselist; } BAttr;

/* symbol table */
#define SYM_SIZE 1024

typedef enum { SYM_INT = 0 } SymType;

typedef struct SymEntry {
    char name[64];
    SymType type;
    int  decl_line;
    int  decl_col;
    int  is_initialized;
    int  is_used;
    struct SymEntry *next;
} SymEntry;

extern SymEntry *sym_table[SYM_SIZE];

int       sym_declare(const char *name, int line, int col);
int       sym_lookup(const char *name);
SymEntry *sym_find(const char *name);
SymEntry *sym_mark_used(const char *name, int line, int col);
SymEntry *sym_mark_init(const char *name);
const char *sym_type_str(SymEntry *e);

/* tac globals */
#define MAX_CODE 4096
extern Instruction code_buf[MAX_CODE];
extern int code_len;
extern int temp_count;

/* tac functions */
char    *xstrdup(const char *s);
char    *mkstr(const char *fmt, ...);
AAttr   *make_aattr(char *place);
BAttr   *make_battr(IntList *t, IntList *f);
void     free_aattr(AAttr *a);
int      nextinstr(void);
int      line_of(int idx);
int      emit_text(const char *fmt, ...);
int      emit_if(const char *cond);
int      emit_goto(int target);
IntList *makelist(int idx);
IntList *merge_list(IntList *a, IntList *b);
void     patch_one(int idx, int target);
void     backpatch(IntList *list, int target);
char    *new_temp(void);
BAttr   *emit_truthy(const char *expr);
BAttr   *emit_relop(const char *lhs, const char *op, const char *rhs);
void     emit_compound_assign(const char *id, const char *op, AAttr *rhs);
void     emit_incdec(const char *id, int is_inc);
void     emit_bool_assignment(const char *id, BAttr *b);
void     print_tac(void);

/* error state */
extern int had_errors;
extern int last_syntax_error_line;
extern int yylineno;
extern int yycolno;

/* source line buffer — keeps a copy of each input line for error display */
#define MAX_SOURCE_LINES 8192
#define MAX_LINE_LEN 1024
extern char source_lines[MAX_SOURCE_LINES][MAX_LINE_LEN];
extern int source_line_count;
void store_source_line(int lineno, const char *text);

/* gcc-style diagnostic printer */
void emit_diagnostic(const char *filename, int line, int col,
                     const char *severity, const char *fmt, ...);

/* token and error logging */
#define MAX_TOKENS 16384
#define MAX_ERRORS 256

typedef struct { int token; char lexeme[128]; int line; int col; } TokenEntry;
typedef struct { int line; int col; char message[256]; } ErrorEntry;

extern TokenEntry token_log[MAX_TOKENS];
extern int token_count;
extern ErrorEntry lex_errors[MAX_ERRORS];
extern int lex_error_count;
extern ErrorEntry syntax_errors[MAX_ERRORS];
extern int syntax_error_count;
extern ErrorEntry sem_errors[MAX_ERRORS];
extern int sem_error_count;

void log_token(int tok, const char *text, int line, int col);
void log_lex_error(int line, int col, const char *fmt, ...);
void log_syntax_error(int line, int col, const char *fmt, ...);
void log_sem_error(int line, int col, const char *fmt, ...);

/* optimizer */
extern Instruction opt_buf[MAX_CODE];
extern int opt_len;
extern int stat_const_fold;
extern int stat_const_prop;
extern int stat_cse;
extern int stat_dead_elim;

void optimize(void);
void print_opt_tac(void);

/* codegen */
extern int asm_count;
void generate_assembly(void);
void print_assembly(void);

/* lexer/parser glue */
int  yylex(void);
void yyerror(const char *s);
void report_lex_error(const char *bad, int col);

#endif