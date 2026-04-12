/* ================================================================
 *  lexer.l  —  PHASE 1: Lexical Analysis (Tokenization)
 *
 *  Features:
 *    - Column tracking for GCC-style diagnostics
 *    - Source line buffering for caret display
 *    - Verbose error messages (stray chars, invalid suffixes,
 *      unterminated comments/strings)
 * ================================================================ */
%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>

/* ── Column Tracking ─────────────────────────────────────── */
extern int yycolno;

static void update_col(void) {
    yycolno += yyleng;
}

static int save_col(void) {
    return yycolno;
}

/* ── Source Line Buffering ───────────────────────────────── */
static char line_buf[MAX_LINE_LEN];
static int  line_buf_pos = 0;
static int  line_buf_lineno = 1;

static void buf_flush(int lineno) {
    if (line_buf_pos > 0) {
        line_buf[line_buf_pos] = '\0';
        store_source_line(line_buf_lineno, line_buf);
        line_buf_pos = 0;
    }
    line_buf_lineno = lineno;
}

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
    int _col = save_col(); \
    log_token(t, yytext, yylineno, _col); \
    update_col(); \
    return t; \
} while(0)

#define TOK_SVAL(t) do { \
    int _col = save_col(); \
    yylval.sval = dup_text(yytext); \
    log_token(t, yytext, yylineno, _col); \
    update_col(); \
    return t; \
} while(0)

#define SKIP() do { \
    buf_append(yytext, yyleng); \
    update_col(); \
} while(0)

%}

%option noyywrap yylineno
%x COMMENT

%%

    /* ── Whitespace & Newlines ───────────────────────────── */
[ \t\r]+                    { buf_append(yytext, yyleng); update_col(); }
\n                          { buf_append(yytext, yyleng); /* col reset in buf_append */ }

    /* ── Preprocessor Directives (skip) ──────────────────── */
"#".*                       { buf_append(yytext, yyleng); update_col(); }

    /* ── Comments ────────────────────────────────────────── */
"//".*                      { buf_append(yytext, yyleng); update_col(); }

"/*"                        { buf_append(yytext, yyleng); update_col(); BEGIN(COMMENT); }
<COMMENT>"*/"               { buf_append(yytext, yyleng); update_col(); BEGIN(INITIAL); }
<COMMENT>\n                 { buf_append(yytext, yyleng); }
<COMMENT>.                  { buf_append(yytext, yyleng); update_col(); }
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

    /* ── Multi-char Operators (must come before single-char) */
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
"{"                         { buf_append(yytext, yyleng); TOK(LBRACE); }
"}"                         { buf_append(yytext, yyleng); TOK(RBRACE); }
";"                         { buf_append(yytext, yyleng); TOK(SEMI); }
","                         { buf_append(yytext, yyleng); TOK(COMMA); }
":"                         { buf_append(yytext, yyleng); TOK(COLON); }
"&"                         { buf_append(yytext, yyleng); TOK(AMPER); }

    /* ── String Literals ─────────────────────────────────── */
\"([^\\\n]|\\.)*\"          { buf_append(yytext, yyleng); TOK_SVAL(STRING); }
\'([^\\\n]|\\.)*\'          { buf_append(yytext, yyleng); TOK_SVAL(STRING); }

    /* ── Unterminated String ─────────────────────────────── */
\"([^\\\n]|\\.)*$           {
                                buf_append(yytext, yyleng);
                                int col = save_col();
                                log_lex_error(yylineno, col,
                                    "missing terminating '\"' character");
                                update_col();
                            }

    /* ── Float Literal (not supported — error) ───────────── */
[0-9]+\.[0-9]+              {
                                buf_append(yytext, yyleng);
                                int col = save_col();
                                /* find the dot position for precise message */
                                const char *dot = strchr(yytext, '.');
                                log_lex_error(yylineno, col,
                                    "invalid suffix '%s' on integer constant "
                                    "(floating-point types are not supported)",
                                    dot);
                                update_col();
                                /* still return as NUMBER so parser can continue */
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
                                int col = save_col();
                                report_lex_error(yytext, col);
                                update_col();
                                log_token(INVALID, yytext, yylineno, col);
                                return INVALID;
                            }

%%