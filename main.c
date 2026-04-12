#include "common.h"
#include "parser.tab.h"

int had_errors = 0;
int last_syntax_error_line = -1;

/* source line buffer */
char source_lines[MAX_SOURCE_LINES][MAX_LINE_LEN];
int source_line_count = 0;
int yycolno = 1;

void store_source_line(int lineno, const char *text) {
    if (lineno > 0 && lineno <= MAX_SOURCE_LINES) {
        strncpy(source_lines[lineno - 1], text, MAX_LINE_LEN - 1);
        source_lines[lineno - 1][MAX_LINE_LEN - 1] = '\0';
        if (lineno > source_line_count) source_line_count = lineno;
    }
}

/* prints a diagnostic the way gcc does: file:line:col + source line + caret */
void emit_diagnostic(const char *filename, int line, int col,
                     const char *severity, const char *fmt, ...) {
    va_list ap;
    char msg[512];
    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    printf("  %s:%d:%d: %s: %s\n", filename, line, col, severity, msg);

    if (line > 0 && line <= source_line_count && source_lines[line - 1][0] != '\0') {
        const char *src = source_lines[line - 1];
        int len = (int)strlen(src);
        int printlen = (len > 0 && src[len - 1] == '\n') ? len - 1 : len;
        printf("  %5d | %.*s\n", line, printlen, src);
        printf("        | ");
        int i;
        for (i = 1; i < col && i <= printlen; i++)
            printf("%c", (src[i - 1] == '\t') ? '\t' : ' ');
        printf("^\n");
    }
}

/* token + error logs */
TokenEntry token_log[MAX_TOKENS];
int token_count = 0;
ErrorEntry lex_errors[MAX_ERRORS];
int lex_error_count = 0;
ErrorEntry syntax_errors[MAX_ERRORS];
int syntax_error_count = 0;
ErrorEntry sem_errors[MAX_ERRORS];
int sem_error_count = 0;

void log_token(int tok, const char *text, int line, int col) {
    if (token_count < MAX_TOKENS) {
        token_log[token_count].token = tok;
        strncpy(token_log[token_count].lexeme, text, 127);
        token_log[token_count].lexeme[127] = '\0';
        token_log[token_count].line = line;
        token_log[token_count].col = col;
        token_count++;
    }
}

void log_lex_error(int line, int col, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    if (lex_error_count < MAX_ERRORS) {
        lex_errors[lex_error_count].line = line;
        lex_errors[lex_error_count].col = col;
        vsnprintf(lex_errors[lex_error_count].message, 256, fmt, ap);
        lex_error_count++;
    }
    va_end(ap);
    had_errors = 1;
    last_syntax_error_line = line;
}

void log_syntax_error(int line, int col, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    if (syntax_error_count < MAX_ERRORS) {
        syntax_errors[syntax_error_count].line = line;
        syntax_errors[syntax_error_count].col = col;
        vsnprintf(syntax_errors[syntax_error_count].message, 256, fmt, ap);
        syntax_error_count++;
    }
    va_end(ap);
    had_errors = 1;
}

void log_sem_error(int line, int col, const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    if (sem_error_count < MAX_ERRORS) {
        sem_errors[sem_error_count].line = line;
        sem_errors[sem_error_count].col = col;
        vsnprintf(sem_errors[sem_error_count].message, 256, fmt, ap);
        sem_error_count++;
    }
    va_end(ap);
    had_errors = 1;
}

/* maps token enum to the short name used in the token table */
static const char *token_name(int tok) {
    switch(tok) {
        case ID: return "ID";
        case NUMBER: return "NUMBER";
        case STRING: return "STRING";
        case TYPE: return "TYPE";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case RETURN: return "RETURN";
        case FOR: return "FOR";
        case WHILE: return "WHILE";
        case DO: return "DO";
        case SWITCH: return "SWITCH";
        case CASE: return "CASE";
        case DEFAULT: return "DEFAULT";
        case BREAK: return "BREAK";
        case CONTINUE: return "CONTINUE";
        case PRINTF: return "PRINTF";
        case SCANF: return "SCANF";
        case AMPER: return "AMPER";
        case ASSIGN: return "ASSIGN";
        case ADD_ASSIGN: return "ADD_ASSIGN";
        case SUB_ASSIGN: return "SUB_ASSIGN";
        case MUL_ASSIGN: return "MUL_ASSIGN";
        case DIV_ASSIGN: return "DIV_ASSIGN";
        case MOD_ASSIGN: return "MOD_ASSIGN";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case MOD: return "MOD";
        case LT: return "LT";
        case GT: return "GT";
        case LE: return "LE";
        case GE: return "GE";
        case EQ: return "EQ";
        case NE: return "NE";
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        case INC: return "INC";
        case DEC: return "DEC";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case SEMI: return "SEMI";
        case COMMA: return "COMMA";
        case COLON: return "COLON";
        case INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

static void print_phase1(void) {
    int i;
    printf("============================================================\n");
    printf("       PHASE 1: LEXICAL ANALYSIS (Tokenization)\n");
    printf("============================================================\n\n");
    printf("  %-10s %-20s %s\n", "Line:Col", "Token Type", "Lexeme");
    printf("  %-10s %-20s %s\n", "--------", "----------", "------");
    for (i = 0; i < token_count; i++) {
        char loc[32];
        snprintf(loc, sizeof(loc), "%d:%d", token_log[i].line, token_log[i].col);
        printf("  %-10s %-20s %s\n",
            loc, token_name(token_log[i].token), token_log[i].lexeme);
    }
    printf("\n  Total tokens: %d\n", token_count);
    if (lex_error_count > 0) {
        int j;
        printf("\n  LEXICAL ERRORS:\n");
        for (j = 0; j < lex_error_count; j++)
            emit_diagnostic("<stdin>", lex_errors[j].line, lex_errors[j].col,
                            "error", "%s", lex_errors[j].message);
    } else {
        printf("  [OK] No lexical errors.\n");
    }
    printf("\n");
}

static void print_phase2(void) {
    printf("============================================================\n");
    printf("       PHASE 2: SYNTAX ANALYSIS (Parsing)\n");
    printf("============================================================\n\n");
    if (syntax_error_count > 0) {
        int i;
        printf("  SYNTAX ERRORS:\n");
        for (i = 0; i < syntax_error_count; i++)
            emit_diagnostic("<stdin>", syntax_errors[i].line, syntax_errors[i].col,
                            "error", "%s", syntax_errors[i].message);
    } else {
        printf("  [OK] Syntax analysis completed successfully.\n");
        printf("  The input program is syntactically valid.\n");
    }
    printf("\n");
}

static void print_phase3(void) {
    int i, found = 0;
    printf("============================================================\n");
    printf("       PHASE 3: SEMANTIC ANALYSIS\n");
    printf("============================================================\n\n");

    printf("  Symbol Table (Declared Variables):\n");
    printf("  %-20s %-6s %-10s %-6s %-6s\n", "Name", "Type", "Line:Col", "Init", "Used");
    printf("  %-20s %-6s %-10s %-6s %-6s\n", "----", "----", "--------", "----", "----");
    for (i = 0; i < SYM_SIZE; i++) {
        SymEntry *e = sym_table[i];
        while (e) {
            char loc[32];
            snprintf(loc, sizeof(loc), "%d:%d", e->decl_line, e->decl_col);
            printf("  %-20s %-6s %-10s %-6s %-6s\n",
                e->name, sym_type_str(e), loc,
                e->is_initialized ? "yes" : "no",
                e->is_used ? "yes" : "no");
            e = e->next;
            found++;
        }
    }
    if (!found) printf("  (none)\n");
    printf("\n  Total declared: %d variables\n\n", found);

    if (sem_error_count > 0) {
        int j;
        printf("  SEMANTIC ERRORS:\n");
        for (j = 0; j < sem_error_count; j++)
            emit_diagnostic("<stdin>", sem_errors[j].line, sem_errors[j].col,
                            "error", "%s", sem_errors[j].message);
    } else {
        printf("  [OK] Semantic analysis completed successfully.\n");
        printf("  All variables declared before use. No duplicate declarations.\n");
    }

    /* warn about variables that were never read */
    for (i = 0; i < SYM_SIZE; i++) {
        SymEntry *e = sym_table[i];
        while (e) {
            if (e->decl_line > 0 && !e->is_used) {
                if (e->is_initialized) {
                    emit_diagnostic("<stdin>", e->decl_line, e->decl_col,
                        "warning", "variable '%s' set but not used [-Wunused-but-set-variable]",
                        e->name);
                } else {
                    emit_diagnostic("<stdin>", e->decl_line, e->decl_col,
                        "warning", "unused variable '%s' [-Wunused-variable]",
                        e->name);
                }
            }
            e = e->next;
        }
    }
    printf("\n");
}

static void print_phase4(void) {
    printf("============================================================\n");
    printf("       PHASE 4: INTERMEDIATE CODE (Three Address Code)\n");
    printf("============================================================\n\n");
    if (had_errors) { printf("  Skipped due to errors in previous phases.\n\n"); return; }
    if (code_len == 0) { printf("  (no code generated)\n\n"); return; }
    print_tac();
    printf("\n  Total TAC instructions: %d\n\n", code_len);
}

static void print_phase5(void) {
    printf("============================================================\n");
    printf("       PHASE 5: OPTIMIZATION\n");
    printf("============================================================\n\n");
    if (had_errors) { printf("  Skipped due to errors in previous phases.\n\n"); return; }
    if (code_len == 0) { printf("  (no code to optimize)\n\n"); return; }

    optimize();

    printf("  Local optimizations:\n");
    printf("    - Constant Folding:         %d\n", stat_const_fold);
    printf("    - Algebraic Simplification: %d\n", stat_algebraic);
    printf("    - Strength Reduction:       %d\n", stat_strength_red);
    printf("    - Constant Propagation:     %d\n", stat_const_prop);
    printf("    - Copy Propagation:         %d\n", stat_copy_prop);
    printf("    - Common Sub-Expression:    %d\n", stat_cse);
    printf("    - Dead Code Elimination:    %d\n", stat_dead_elim);
    printf("\n  Global optimizations:\n");
    printf("    - Global Const Propagation: %d\n", stat_global_const);
    printf("    - Loop-Invariant Motion:    %d\n", stat_loop_inv);
    printf("    - Unreachable Code Removal: %d\n", stat_unreachable);
    printf("\n    Total instructions removed: %d\n", code_len - opt_len);
    printf("\n  Optimized TAC (%d instructions):\n\n", opt_len);
    print_opt_tac();
    printf("\n");
}

static void print_phase6(void) {
    printf("============================================================\n");
    printf("       PHASE 6: TARGET CODE GENERATION (Assembly)\n");
    printf("============================================================\n\n");
    if (had_errors) { printf("  Skipped due to errors in previous phases.\n\n"); return; }
    if (opt_len == 0) { printf("  (no code to generate)\n\n"); return; }

    generate_assembly();

    printf("  Target:  Simple RISC ISA (8 registers R0-R7)\n");
    printf("  Total assembly instructions: %d\n\n", asm_count);
    print_assembly();
    printf("\n");
}

/* bison calls this on parse errors */
void yyerror(const char *s) {
    if (yylineno != last_syntax_error_line) {
        log_syntax_error(yylineno, yycolno, "%s", s);
        emit_diagnostic("<stdin>", yylineno, yycolno, "error", "%s", s);
        last_syntax_error_line = yylineno;
    }
    had_errors = 1;
}

void report_lex_error(const char *bad, int col) {
    log_lex_error(yylineno, col, "stray '%s' in program", bad);
}

int main(void) {
    printf("\n");
    printf("************************************************************\n");
    printf("*       MINI COMPILER FOR TOY-C LANGUAGE                   *\n");
    printf("*       Target: Simple RISC ISA                            *\n");
    printf("************************************************************\n\n");

    yyparse();

    print_phase1();
    print_phase2();
    print_phase3();
    print_phase4();
    print_phase5();
    print_phase6();

    if (had_errors) {
        printf("Compilation FAILED with errors.\n");
        return 1;
    }
    printf("Compilation completed successfully.\n");
    return 0;
}