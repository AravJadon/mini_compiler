/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "parser.y"

#include "common.h"

#line 53 "y.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    NUMBER = 259,                  /* NUMBER  */
    STRING = 260,                  /* STRING  */
    TYPE = 261,                    /* TYPE  */
    IF = 262,                      /* IF  */
    ELSE = 263,                    /* ELSE  */
    RETURN = 264,                  /* RETURN  */
    FOR = 265,                     /* FOR  */
    WHILE = 266,                   /* WHILE  */
    DO = 267,                      /* DO  */
    SWITCH = 268,                  /* SWITCH  */
    CASE = 269,                    /* CASE  */
    DEFAULT = 270,                 /* DEFAULT  */
    BREAK = 271,                   /* BREAK  */
    CONTINUE = 272,                /* CONTINUE  */
    PRINTF = 273,                  /* PRINTF  */
    SCANF = 274,                   /* SCANF  */
    AMPER = 275,                   /* AMPER  */
    ASSIGN = 276,                  /* ASSIGN  */
    ADD_ASSIGN = 277,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 278,              /* SUB_ASSIGN  */
    MUL_ASSIGN = 279,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 280,              /* DIV_ASSIGN  */
    MOD_ASSIGN = 281,              /* MOD_ASSIGN  */
    PLUS = 282,                    /* PLUS  */
    MINUS = 283,                   /* MINUS  */
    MUL = 284,                     /* MUL  */
    DIV = 285,                     /* DIV  */
    MOD = 286,                     /* MOD  */
    LT = 287,                      /* LT  */
    GT = 288,                      /* GT  */
    LE = 289,                      /* LE  */
    GE = 290,                      /* GE  */
    EQ = 291,                      /* EQ  */
    NE = 292,                      /* NE  */
    AND = 293,                     /* AND  */
    OR = 294,                      /* OR  */
    NOT = 295,                     /* NOT  */
    INC = 296,                     /* INC  */
    DEC = 297,                     /* DEC  */
    LPAREN = 298,                  /* LPAREN  */
    RPAREN = 299,                  /* RPAREN  */
    LBRACE = 300,                  /* LBRACE  */
    RBRACE = 301,                  /* RBRACE  */
    SEMI = 302,                    /* SEMI  */
    COMMA = 303,                   /* COMMA  */
    COLON = 304,                   /* COLON  */
    INVALID = 305,                 /* INVALID  */
    LOWER_THAN_ELSE = 306,         /* LOWER_THAN_ELSE  */
    UMINUS = 307                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define ID 258
#define NUMBER 259
#define STRING 260
#define TYPE 261
#define IF 262
#define ELSE 263
#define RETURN 264
#define FOR 265
#define WHILE 266
#define DO 267
#define SWITCH 268
#define CASE 269
#define DEFAULT 270
#define BREAK 271
#define CONTINUE 272
#define PRINTF 273
#define SCANF 274
#define AMPER 275
#define ASSIGN 276
#define ADD_ASSIGN 277
#define SUB_ASSIGN 278
#define MUL_ASSIGN 279
#define DIV_ASSIGN 280
#define MOD_ASSIGN 281
#define PLUS 282
#define MINUS 283
#define MUL 284
#define DIV 285
#define MOD 286
#define LT 287
#define GT 288
#define LE 289
#define GE 290
#define EQ 291
#define NE 292
#define AND 293
#define OR 294
#define NOT 295
#define INC 296
#define DEC 297
#define LPAREN 298
#define RPAREN 299
#define LBRACE 300
#define RBRACE 301
#define SEMI 302
#define COMMA 303
#define COLON 304
#define INVALID 305
#define LOWER_THAN_ELSE 306
#define UMINUS 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 8 "parser.y"

    char *sval;
    AAttr *a;
    BAttr *b;
    IntList *list;
    int ival;

#line 185 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
