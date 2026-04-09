/* ================================================================
 *  lexer.l  —  PHASE 1: Lexical Analysis (Tokenization)
 * ================================================================ */
%{
#include "parser.tab.h"

static char *dup_text(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (!p) exit(1);
    memcpy(p, s, n);
    return p;
}

#define TOK(t) do { log_token(t, yytext, yylineno); return t; } while(0)
%}

%option noyywrap yylineno
%x COMMENT

%%

[ \t\r\n]+                                  { /* skip whitespace */ }

"#".*                                       { /* skip preprocessor */ }
"//".*                                      { /* skip single-line comment */ }

"/*"                                        { BEGIN(COMMENT); }
<COMMENT>"*/"                               { BEGIN(INITIAL); }
<COMMENT>.                                  { }
<COMMENT>\n                                 { }

"int"|"float"|"double"|"char"|"short"|"long"|"unsigned"|"signed"|"void"|"bool" { yylval.sval = dup_text(yytext); TOK(TYPE); }
"if"                                        { TOK(IF); }
"else"                                      { TOK(ELSE); }
"for"                                       { TOK(FOR); }
"while"                                     { TOK(WHILE); }
"do"                                        { TOK(DO); }
"switch"                                    { TOK(SWITCH); }
"case"                                      { TOK(CASE); }
"default"                                   { TOK(DEFAULT); }
"break"                                     { TOK(BREAK); }
"continue"                                  { TOK(CONTINUE); }
"return"                                    { TOK(RETURN); }
"printf"                                    { TOK(PRINTF); }
"scanf"                                     { TOK(SCANF); }
"=="                                        { TOK(EQ); }
"!="                                        { TOK(NE); }
"<="                                        { TOK(LE); }
">="                                        { TOK(GE); }
"&&"                                        { TOK(AND); }
"||"                                        { TOK(OR); }
"+="                                        { TOK(ADD_ASSIGN); }
"-="                                        { TOK(SUB_ASSIGN); }
"*="                                        { TOK(MUL_ASSIGN); }
"/="                                        { TOK(DIV_ASSIGN); }
"%="                                        { TOK(MOD_ASSIGN); }
"++"                                        { TOK(INC); }
"--"                                        { TOK(DEC); }

"="                                         { TOK(ASSIGN); }
"+"                                         { TOK(PLUS); }
"-"                                         { TOK(MINUS); }
"*"                                         { TOK(MUL); }
"/"                                         { TOK(DIV); }
"%"                                         { TOK(MOD); }
"<"                                         { TOK(LT); }
">"                                         { TOK(GT); }
"!"                                         { TOK(NOT); }
"("                                         { TOK(LPAREN); }
")"                                         { TOK(RPAREN); }
"{"                                         { TOK(LBRACE); }
"}"                                         { TOK(RBRACE); }
";"                                         { TOK(SEMI); }
","                                         { TOK(COMMA); }
":"                                         { TOK(COLON); }
"&"                                         { TOK(AMPER); }

\"([^\\\n]|\\.)*\"                          { yylval.sval = dup_text(yytext); TOK(STRING); }
\'([^\\\n]|\\.)*\'                          { yylval.sval = dup_text(yytext); TOK(STRING); }

[0-9]+\.[0-9]+                              { yylval.sval = dup_text(yytext); TOK(NUMBER); }
[0-9]+                                      { yylval.sval = dup_text(yytext); TOK(NUMBER); }

[A-Za-z_][A-Za-z0-9_]*                      { yylval.sval = dup_text(yytext); TOK(ID); }

.                                           { report_lex_error(yytext); TOK(INVALID); }

%%
