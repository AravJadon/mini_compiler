/* ================================================================
 *  lexer.l  —  PHASE 1: Lexical Analysis (Tokenization)
 *
 *  Features:
 *    - Column tracking for GCC-style diagnostics
 *    - Source line buffering for caret display
 *    - Bison %locations integration (yylloc updated per token)
 *    - Verbose error messages
 *    - Scope tracking via sym_push_scope / sym_pop_scope on braces
 * ================================================================ */
%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>

/* ── Column Tracking ─────────────────────────────────────── */
extern int yycolno;

/* ── Bison %locations integration ────────────────────────── */
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolno; \
    yylloc.last_column = yycolno + yyleng - 1;

static void advance_col(void) {
    yycolno += yyleng;
}

static int cur_col(void) {
    return yycolno;
}

/* ── Source Line Buffering ───────────────────────────────── */
static char line_buf[MAX_LINE_LEN];
static int  line_buf_pos = 0;
static int  line_buf_lineno = 1;

static void buf_append(const char *text, int len) {
    int i;
    for (i = 0; i < len && line_buf_pos < MAX_LINE_LEN - 1; i++) {
        if (text[i] == '\n') {
            line_buf[line_buf_pos] = '\0';
            store_source_line(line_buf_lineno, line_buf);
            line_buf_pos = 0;
            line_buf_lineno++;
            yycolno = 1;
        } else {
            line_buf[line_buf_pos++] = text[i];
        }
    }
}

/* ── Helpers ─────────────────────────────────────────────── */
static char *dup_text(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (!p) exit(1);
    memcpy(p, s, n);
    return p;
}

#define TOK(t) do { \
    int _col = cur_col(); \
    log_token(t, yytext, yylineno, _col); \
    advance_col(); \
    return t; \
} while(0)

#define TOK_SVAL(t) do { \
    int _col = cur_col(); \
    yylval.sval = dup_text(yytext); \
    log_token(t, yytext, yylineno, _col); \
    advance_col(); \
    return t; \
} while(0)

%}

%option noyywrap yylineno
%x COMMENT

%%

    /* ── Whitespace & Newlines ───────────────────────────── */
[ \t\r]+                    { buf_append(yytext, yyleng); advance_col(); }
\n                          { buf_append(yytext, yyleng); /* col reset in buf_append */ }

    /* ── Preprocessor Directives (skip) ──────────────────── */
"#".*                       { buf_append(yytext, yyleng); advance_col(); }

    /* ── Comments ────────────────────────────────────────── */
"//".*                      { buf_append(yytext, yyleng); advance_col(); }

"/*"                        { buf_append(yytext, yyleng); advance_col(); BEGIN(COMMENT); }
<COMMENT>"*/"               { buf_append(yytext, yyleng); advance_col(); BEGIN(INITIAL); }
<COMMENT>\n                 { buf_append(yytext, yyleng); }
<COMMENT>.                  { buf_append(yytext, yyleng); advance_col(); }
<COMMENT><<EOF>>            {
                                log_lex_error(yylineno, yycolno, "unterminated comment");
                                yyterminate();
                            }

    /* ── Keywords ────────────────────────────────────────── */
"int"|"float"|"double"|"char"|"short"|"long"|"unsigned"|"signed"|"void"|"bool" {
                                buf_append(yytext, yyleng);
                                TOK_SVAL(TYPE);
                            }
"if"                        { buf_append(yytext, yyleng); TOK(IF); }
"else"                      { buf_append(yytext, yyleng); TOK(ELSE); }
"for"                       { buf_append(yytext, yyleng); TOK(FOR); }
"while"                     { buf_append(yytext, yyleng); TOK(WHILE); }
"do"                        { buf_append(yytext, yyleng); TOK(DO); }
"switch"                    { buf_append(yytext, yyleng); TOK(SWITCH); }
"case"                      { buf_append(yytext, yyleng); TOK(CASE); }
"default"                   { buf_append(yytext, yyleng); TOK(DEFAULT); }
"break"                     { buf_append(yytext, yyleng); TOK(BREAK); }
"continue"                  { buf_append(yytext, yyleng); TOK(CONTINUE); }
"return"                    { buf_append(yytext, yyleng); TOK(RETURN); }
"printf"                    { buf_append(yytext, yyleng); TOK(PRINTF); }
"scanf"                     { buf_append(yytext, yyleng); TOK(SCANF); }

    /* ── Multi-char Operators ────────────────────────────── */
"=="                        { buf_append(yytext, yyleng); TOK(EQ); }
"!="                        { buf_append(yytext, yyleng); TOK(NE); }
"<="                        { buf_append(yytext, yyleng); TOK(LE); }
">="                        { buf_append(yytext, yyleng); TOK(GE); }
"&&"                        { buf_append(yytext, yyleng); TOK(AND); }
"||"                        { buf_append(yytext, yyleng); TOK(OR); }
"+="                        { buf_append(yytext, yyleng); TOK(ADD_ASSIGN); }
"-="                        { buf_append(yytext, yyleng); TOK(SUB_ASSIGN); }
"*="                        { buf_append(yytext, yyleng); TOK(MUL_ASSIGN); }
"/="                        { buf_append(yytext, yyleng); TOK(DIV_ASSIGN); }
"%="                        { buf_append(yytext, yyleng); TOK(MOD_ASSIGN); }
"++"                        { buf_append(yytext, yyleng); TOK(INC); }
"--"                        { buf_append(yytext, yyleng); TOK(DEC); }

    /* ── Single-char Operators ───────────────────────────── */
"="                         { buf_append(yytext, yyleng); TOK(ASSIGN); }
"+"                         { buf_append(yytext, yyleng); TOK(PLUS); }
"-"                         { buf_append(yytext, yyleng); TOK(MINUS); }
"*"                         { buf_append(yytext, yyleng); TOK(MUL); }
"/"                         { buf_append(yytext, yyleng); TOK(DIV); }
"%"                         { buf_append(yytext, yyleng); TOK(MOD); }
"<"                         { buf_append(yytext, yyleng); TOK(LT); }
">"                         { buf_append(yytext, yyleng); TOK(GT); }
"!"                         { buf_append(yytext, yyleng); TOK(NOT); }
"("                         { buf_append(yytext, yyleng); TOK(LPAREN); }
")"                         { buf_append(yytext, yyleng); TOK(RPAREN); }

    /* ── Braces also bump the scope stack so the symbol table
         knows a new block has started/ended. keeps the grammar
         free of mid-rule actions, which conflict with bison's
         LR resolution when compound_stmt is reachable through
         many different statement paths. */
"{"                         { buf_append(yytext, yyleng); sym_push_scope(); TOK(LBRACE); }
"}"                         { buf_append(yytext, yyleng); sym_pop_scope();  TOK(RBRACE); }

";"                         { buf_append(yytext, yyleng); TOK(SEMI); }
","                         { buf_append(yytext, yyleng); TOK(COMMA); }
":"                         { buf_append(yytext, yyleng); TOK(COLON); }
"&"                         { buf_append(yytext, yyleng); TOK(AMPER); }

    /* ── String Literals ─────────────────────────────────── */
\"([^\\\n]|\\.)*\"          { buf_append(yytext, yyleng); TOK_SVAL(STRING); }
\'([^\\\n]|\\.)*\'          { buf_append(yytext, yyleng); TOK_SVAL(STRING); }

    /* ── Unterminated String ─────────────────────────────── */
\"([^\\\"\n]|\\.)*$           {
                                buf_append(yytext, yyleng);
                                int col = cur_col();
                                log_lex_error(yylineno, col,
                                    "missing terminating '\"' character");
                                advance_col();
                            }

    /* ── Float Literal (not supported — error) ───────────── */
[0-9]+\.[0-9]+              {
                                buf_append(yytext, yyleng);
                                int col = cur_col();
                                const char *dot = strchr(yytext, '.');
                                log_lex_error(yylineno, col,
                                    "invalid suffix '%s' on integer constant "
                                    "(floating-point types are not supported)",
                                    dot);
                                advance_col();
                                yylval.sval = dup_text(yytext);
                                log_token(NUMBER, yytext, yylineno, col);
                                return NUMBER;
                            }

    /* ── Integer Literals ────────────────────────────────── */
[0-9]+                      { buf_append(yytext, yyleng); TOK_SVAL(NUMBER); }

    /* ── Identifiers ─────────────────────────────────────── */
[A-Za-z_][A-Za-z0-9_]*     { buf_append(yytext, yyleng); TOK_SVAL(ID); }

    /* ── Invalid Characters ──────────────────────────────── */
.                           {
                                buf_append(yytext, yyleng);
                                int col = cur_col();
                                report_lex_error(yytext, col);
                                advance_col();
                                log_token(INVALID, yytext, yylineno, col);
                                return INVALID;
                            }

%%