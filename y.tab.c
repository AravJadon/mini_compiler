/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1





# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 108 "y.tab.c"

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

#line 240 "y.tab.c"

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
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_NUMBER = 4,                     /* NUMBER  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_TYPE = 6,                       /* TYPE  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_RETURN = 9,                     /* RETURN  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_WHILE = 11,                     /* WHILE  */
  YYSYMBOL_DO = 12,                        /* DO  */
  YYSYMBOL_SWITCH = 13,                    /* SWITCH  */
  YYSYMBOL_CASE = 14,                      /* CASE  */
  YYSYMBOL_DEFAULT = 15,                   /* DEFAULT  */
  YYSYMBOL_BREAK = 16,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 17,                  /* CONTINUE  */
  YYSYMBOL_PRINTF = 18,                    /* PRINTF  */
  YYSYMBOL_SCANF = 19,                     /* SCANF  */
  YYSYMBOL_AMPER = 20,                     /* AMPER  */
  YYSYMBOL_ASSIGN = 21,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 22,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 23,                /* SUB_ASSIGN  */
  YYSYMBOL_MUL_ASSIGN = 24,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 25,                /* DIV_ASSIGN  */
  YYSYMBOL_MOD_ASSIGN = 26,                /* MOD_ASSIGN  */
  YYSYMBOL_PLUS = 27,                      /* PLUS  */
  YYSYMBOL_MINUS = 28,                     /* MINUS  */
  YYSYMBOL_MUL = 29,                       /* MUL  */
  YYSYMBOL_DIV = 30,                       /* DIV  */
  YYSYMBOL_MOD = 31,                       /* MOD  */
  YYSYMBOL_LT = 32,                        /* LT  */
  YYSYMBOL_GT = 33,                        /* GT  */
  YYSYMBOL_LE = 34,                        /* LE  */
  YYSYMBOL_GE = 35,                        /* GE  */
  YYSYMBOL_EQ = 36,                        /* EQ  */
  YYSYMBOL_NE = 37,                        /* NE  */
  YYSYMBOL_AND = 38,                       /* AND  */
  YYSYMBOL_OR = 39,                        /* OR  */
  YYSYMBOL_NOT = 40,                       /* NOT  */
  YYSYMBOL_INC = 41,                       /* INC  */
  YYSYMBOL_DEC = 42,                       /* DEC  */
  YYSYMBOL_LPAREN = 43,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 44,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 45,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 46,                    /* RBRACE  */
  YYSYMBOL_SEMI = 47,                      /* SEMI  */
  YYSYMBOL_COMMA = 48,                     /* COMMA  */
  YYSYMBOL_COLON = 49,                     /* COLON  */
  YYSYMBOL_INVALID = 50,                   /* INVALID  */
  YYSYMBOL_LOWER_THAN_ELSE = 51,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_UMINUS = 52,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 53,                  /* $accept  */
  YYSYMBOL_program = 54,                   /* program  */
  YYSYMBOL_external_list = 55,             /* external_list  */
  YYSYMBOL_external = 56,                  /* external  */
  YYSYMBOL_function_def = 57,              /* function_def  */
  YYSYMBOL_function_proto = 58,            /* function_proto  */
  YYSYMBOL_opt_param_list = 59,            /* opt_param_list  */
  YYSYMBOL_param_list = 60,                /* param_list  */
  YYSYMBOL_param_decl = 61,                /* param_decl  */
  YYSYMBOL_compound_stmt = 62,             /* compound_stmt  */
  YYSYMBOL_stmt_list = 63,                 /* stmt_list  */
  YYSYMBOL_statement = 64,                 /* statement  */
  YYSYMBOL_non_if_stmt = 65,               /* non_if_stmt  */
  YYSYMBOL_arg_list = 66,                  /* arg_list  */
  YYSYMBOL_switch_stmt = 67,               /* switch_stmt  */
  YYSYMBOL_68_1 = 68,                      /* $@1  */
  YYSYMBOL_case_list = 69,                 /* case_list  */
  YYSYMBOL_case_entries = 70,              /* case_entries  */
  YYSYMBOL_case_entry = 71,                /* case_entry  */
  YYSYMBOL_72_2 = 72,                      /* $@2  */
  YYSYMBOL_73_3 = 73,                      /* $@3  */
  YYSYMBOL_matched_stmt = 74,              /* matched_stmt  */
  YYSYMBOL_75_4 = 75,                      /* $@4  */
  YYSYMBOL_76_5 = 76,                      /* $@5  */
  YYSYMBOL_unmatched_stmt = 77,            /* unmatched_stmt  */
  YYSYMBOL_78_6 = 78,                      /* $@6  */
  YYSYMBOL_decl_stmt = 79,                 /* decl_stmt  */
  YYSYMBOL_decl_items = 80,                /* decl_items  */
  YYSYMBOL_decl_item = 81,                 /* decl_item  */
  YYSYMBOL_assign_stmt = 82,               /* assign_stmt  */
  YYSYMBOL_inc_stmt = 83,                  /* inc_stmt  */
  YYSYMBOL_return_stmt = 84,               /* return_stmt  */
  YYSYMBOL_M = 85,                         /* M  */
  YYSYMBOL_N = 86,                         /* N  */
  YYSYMBOL_bexpr = 87,                     /* bexpr  */
  YYSYMBOL_bor_expr = 88,                  /* bor_expr  */
  YYSYMBOL_band_expr = 89,                 /* band_expr  */
  YYSYMBOL_bnot_expr = 90,                 /* bnot_expr  */
  YYSYMBOL_bprimary = 91,                  /* bprimary  */
  YYSYMBOL_rel_bool = 92,                  /* rel_bool  */
  YYSYMBOL_aexpr = 93,                     /* aexpr  */
  YYSYMBOL_term = 94,                      /* term  */
  YYSYMBOL_factor = 95                     /* factor  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   490

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  124
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  293

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   307


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    45,    45,    49,    50,    54,    55,    56,    60,    65,
      70,    71,    75,    76,    80,    95,    96,   105,   106,   110,
     111,   115,   116,   117,   118,   119,   125,   131,   133,   139,
     147,   160,   161,   162,   163,   169,   175,   181,   190,   202,
     203,   204,   219,   218,   251,   255,   256,   266,   265,   311,
     310,   340,   341,   347,   353,   360,   359,   376,   382,   389,
     388,   401,   406,   411,   418,   417,   434,   438,   439,   443,
     455,   470,   480,   495,   516,   532,   542,   553,   567,   579,
     591,   603,   615,   630,   641,   652,   663,   677,   678,   681,
     682,   684,   687,   692,   696,   701,   705,   706,   716,   718,
     720,   738,   747,   748,   749,   750,   751,   752,   756,   758,
     760,   765,   767,   769,   771,   776,   777,   789,   801,   817,
     833,   840,   845,   863,   873
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "NUMBER",
  "STRING", "TYPE", "IF", "ELSE", "RETURN", "FOR", "WHILE", "DO", "SWITCH",
  "CASE", "DEFAULT", "BREAK", "CONTINUE", "PRINTF", "SCANF", "AMPER",
  "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN",
  "MOD_ASSIGN", "PLUS", "MINUS", "MUL", "DIV", "MOD", "LT", "GT", "LE",
  "GE", "EQ", "NE", "AND", "OR", "NOT", "INC", "DEC", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "SEMI", "COMMA", "COLON", "INVALID",
  "LOWER_THAN_ELSE", "UMINUS", "$accept", "program", "external_list",
  "external", "function_def", "function_proto", "opt_param_list",
  "param_list", "param_decl", "compound_stmt", "stmt_list", "statement",
  "non_if_stmt", "arg_list", "switch_stmt", "$@1", "case_list",
  "case_entries", "case_entry", "$@2", "$@3", "matched_stmt", "$@4", "$@5",
  "unmatched_stmt", "$@6", "decl_stmt", "decl_items", "decl_item",
  "assign_stmt", "inc_stmt", "return_stmt", "M", "N", "bexpr", "bor_expr",
  "band_expr", "bnot_expr", "bprimary", "rel_bool", "aexpr", "term",
  "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-254)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-91)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -254,    19,   210,  -254,    56,   434,    26,    12,   221,   -33,
      24,  -254,    28,    41,    47,    32,    54,    42,   105,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,
       5,    10,    17,    66,  -254,  -254,   132,   221,   221,   221,
     221,   221,  -254,  -254,   221,    -1,    75,  -254,  -254,    20,
      63,  -254,   221,   221,   221,   121,   122,   221,    83,    70,
    -254,   124,  -254,    84,  -254,   221,  -254,  -254,   123,   126,
    -254,  -254,   343,  -254,  -254,  -254,  -254,  -254,  -254,  -254,
    -254,   231,   221,    83,    83,    83,    83,    83,    83,    -7,
      83,   163,   128,   129,    98,    45,   398,   398,    99,   106,
     115,  -254,  -254,  -254,   293,  -254,  -254,  -254,  -254,  -254,
    -254,  -254,     0,   221,   221,   221,   221,   221,   129,   108,
     392,   362,    46,     2,   109,    69,  -254,  -254,   221,   112,
     446,   111,   221,  -254,   426,   221,    83,   159,   119,   130,
    -254,   148,  -254,  -254,   221,  -254,  -254,   133,   446,  -254,
    -254,  -254,   221,   221,   221,   221,   221,   221,  -254,    70,
      70,  -254,  -254,  -254,  -254,   135,   136,   165,  -254,   137,
     221,   150,  -254,   138,  -254,  -254,    83,   221,   142,  -254,
      37,   128,   405,    83,  -254,   362,   398,   398,    83,    83,
      83,    83,    83,    83,   398,  -254,  -254,  -254,   143,  -254,
      65,   186,  -254,   151,  -254,  -254,  -254,  -254,    25,    24,
    -254,  -254,   184,   115,  -254,   147,   405,  -254,  -254,   153,
     103,   154,   156,  -254,    20,   155,   189,   194,  -254,  -254,
     405,    58,   398,   205,   166,   168,   103,  -254,  -254,   183,
     187,   392,  -254,  -254,   221,  -254,   169,  -254,  -254,   192,
     190,  -254,  -254,  -254,  -254,  -254,   196,   405,   362,  -254,
     293,   398,   198,   195,  -254,  -254,   405,  -254,  -254,  -254,
    -254,   199,   200,   398,  -254,  -254,   277,  -254,  -254,  -254,
     202,   296,   239,   405,   362,  -254,  -254,  -254,   184,   405,
     242,  -254,    58
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     1,     0,     0,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    18,
      33,     3,     5,     6,    32,     7,    51,    31,    19,    20,
       0,     0,     0,     0,    38,    37,     0,     0,     0,     0,
       0,     0,    83,    84,    39,    69,    66,    67,    54,     0,
     123,   124,     0,     0,     0,     0,     0,     0,    88,   110,
     114,     0,    58,     0,    59,     0,    25,    26,     0,     0,
      85,    86,     0,    34,    21,    35,    22,    36,    23,    24,
      75,     0,     0,    74,    78,    79,    80,    81,    82,     0,
      40,     0,    10,     0,     0,   123,     0,     0,     0,    91,
      93,    95,    97,    99,   101,   118,   119,   121,   120,   122,
     116,   117,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    15,    17,     0,     0,
       0,     0,     0,    71,     0,     0,    70,     0,     0,    11,
      12,    69,    68,    89,     0,    96,   114,     0,   101,    89,
      89,    89,     0,     0,     0,     0,     0,     0,   115,   108,
     109,   111,   112,   113,    89,     0,     0,     0,    42,     0,
       0,     0,    16,     0,    76,    27,    41,     0,     0,    14,
       0,     0,     0,   100,    98,     0,     0,     0,   102,   103,
     104,   105,   106,   107,     0,    89,    55,    89,     0,    28,
       0,     0,    77,     0,    72,     9,     8,    13,     0,     0,
      90,    62,    19,    92,    94,     0,     0,    89,    89,     0,
       0,     0,     0,    73,     0,     0,     0,     0,    89,    57,
       0,     0,     0,     0,     0,     0,    44,    45,    29,     0,
       0,     0,    89,    89,     0,    56,     0,    89,    65,     0,
       0,    49,    43,    46,    30,    89,     0,     0,     0,    90,
       0,     0,     0,     0,    47,    18,     0,    55,    53,    52,
      63,     0,     0,     0,    60,    18,     0,    90,    89,    89,
       0,     0,     0,     0,     0,    64,    89,    61,    19,     0,
       0,    89,     0
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,    72,    74,
    -253,    -2,  -254,  -254,  -254,  -254,  -254,  -254,    29,  -254,
    -254,  -177,  -254,  -254,  -222,  -254,   206,  -254,   167,  -254,
    -254,  -254,   -10,  -205,   -80,  -254,    80,   -92,  -254,   -79,
      -6,     7,   -38
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    21,    22,    23,   138,   139,   140,    24,
      72,   127,    26,    89,    27,   198,   235,   236,   237,   275,
     265,    28,   217,   121,    29,   218,    30,    46,    47,    31,
      32,    33,    63,   226,    98,    99,   100,   101,   102,   103,
     104,    59,    60
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    64,    58,   129,   145,   210,    73,   227,   212,   248,
      61,    75,   276,    48,   107,   108,   109,   147,    77,     3,
      91,    94,   281,    95,    51,    62,    48,   113,   114,    45,
      83,    84,    85,    86,    87,    88,   270,   131,    90,   229,
     166,   132,    92,   109,   158,    70,   169,    52,    53,   173,
     170,   112,    74,   245,   271,    49,   178,    76,   146,   122,
      96,    55,    56,    97,    78,   246,   144,   -89,   224,   247,
     270,    65,   282,   113,   114,    68,   130,   161,   162,   163,
     268,   269,    19,   290,   205,   136,   105,   106,    66,   277,
     168,   148,   113,   114,    67,   214,   109,    69,   203,   115,
     116,   117,    34,    35,   105,   106,   287,   288,    71,   221,
     113,   114,   269,    79,   215,   172,    35,   233,   234,   167,
     159,   160,   130,    93,   110,   111,   176,   120,   123,   130,
     118,   124,   141,   182,   137,    50,    51,    80,   183,   185,
     186,   187,   143,   149,   240,   150,   188,   189,   190,   191,
     192,   193,   249,   151,   194,   164,   174,   171,   175,    52,
      53,   256,   179,   180,   200,   259,    50,    51,   133,    91,
     201,   130,    81,    55,    56,    82,   197,   184,   181,   195,
     196,   272,   202,   211,   199,   216,   204,   219,   220,   222,
      52,    53,   -90,   280,   228,   223,   232,   242,   241,   225,
     239,   238,   243,   134,    55,    56,   135,   230,   231,   250,
      -2,     4,   261,     5,   252,   251,     6,     7,   244,     8,
       9,    10,    11,    12,    50,    51,    13,    14,    15,    16,
     254,   255,   257,   258,    50,    51,   263,   262,   260,   264,
     267,   273,   274,   278,   279,   266,   285,   286,    52,    53,
     291,    17,    18,   207,   206,    19,     0,    20,    52,    53,
     142,    54,    55,    56,    57,   253,   213,   119,   283,   284,
       0,    54,    55,    56,   128,     0,   289,     0,     4,     0,
       5,   292,   211,   118,     7,     0,     8,     9,    10,    11,
      12,   -50,   -50,    13,    14,    15,    16,     4,     0,     5,
       0,     0,   118,     7,     0,     8,     9,    10,    11,    12,
     -48,   -48,    13,    14,    15,    16,     0,     0,    17,    18,
     113,   114,    19,   -50,    20,   152,   153,   154,   155,   156,
     157,     0,     0,     0,     0,     0,     0,    17,    18,     0,
       0,    19,   -48,    20,   125,     0,     5,     0,     0,   118,
       7,     0,     8,     9,    10,    11,    12,     0,     0,    13,
      14,    15,    16,     4,     0,     5,     0,     0,   118,     7,
       0,     8,     9,    10,    11,    12,     0,     0,    13,    14,
      15,    16,     0,     0,    17,    18,     0,     0,    19,   126,
      20,     0,     0,   165,     0,    95,    51,     0,     0,     0,
       0,    95,    51,    17,    18,     0,     4,    19,     5,    20,
       0,   118,   208,     0,     8,     9,   209,    11,    12,    52,
      53,    13,    14,    15,    16,    52,    53,     0,     0,    50,
      51,     0,    96,    55,    56,    97,     0,     0,    96,    55,
      56,    97,     0,     0,     0,     0,    17,    18,     0,     0,
      19,     0,    20,    52,    53,    36,    37,    38,    39,    40,
      41,     0,     0,     0,     0,     0,    54,    55,    56,   177,
       0,     0,     0,   113,   114,    42,    43,    44,   152,   153,
     154,   155,   156,   157,     0,     0,     0,     0,     0,     0,
     158
};

static const yytype_int16 yycheck[] =
{
       2,    11,     8,    82,    96,   182,     1,   212,   185,   231,
      43,     1,   265,     1,    52,    53,    54,    97,     1,     0,
      21,     1,   275,     3,     4,     1,     1,    27,    28,     3,
      36,    37,    38,    39,    40,    41,   258,    44,    44,   216,
     120,    48,    43,    81,    44,     3,    44,    27,    28,   128,
      48,    57,    47,   230,   259,    43,   135,    47,    96,    65,
      40,    41,    42,    43,    47,     7,    21,    43,    43,    11,
     292,    43,   277,    27,    28,    43,    82,   115,   116,   117,
     257,   258,    45,   288,    47,    91,    41,    42,    47,   266,
      44,    97,    27,    28,    47,   187,   134,    43,   177,    29,
      30,    31,    46,    47,    41,    42,   283,   284,     3,    44,
      27,    28,   289,    47,   194,    46,    47,    14,    15,   121,
     113,   114,   128,    48,     3,     3,   132,    43,     5,   135,
       6,     5,     3,   143,     6,     3,     4,     5,   144,   149,
     150,   151,    44,    44,   224,    39,   152,   153,   154,   155,
     156,   157,   232,    38,   164,    47,    44,    48,    47,    27,
      28,   241,     3,    44,   170,   244,     3,     4,     5,    21,
      20,   177,    40,    41,    42,    43,    11,    44,    48,    44,
      44,   261,    44,   185,    47,   195,    44,   197,    45,     3,
      27,    28,     8,   273,    47,    44,    43,     8,    43,   209,
      44,    47,     8,    40,    41,    42,    43,   217,   218,     4,
       0,     1,    43,     3,    46,    49,     6,     7,   228,     9,
      10,    11,    12,    13,     3,     4,    16,    17,    18,    19,
      47,    44,   242,   243,     3,     4,    44,   247,   244,    49,
      44,    43,    47,    44,    44,   255,    44,     8,    27,    28,
       8,    41,    42,   181,   180,    45,    -1,    47,    27,    28,
      93,    40,    41,    42,    43,   236,   186,    61,   278,   279,
      -1,    40,    41,    42,    43,    -1,   286,    -1,     1,    -1,
       3,   291,   284,     6,     7,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,     1,    -1,     3,
      -1,    -1,     6,     7,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    41,    42,
      27,    28,    45,    46,    47,    32,    33,    34,    35,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    45,    46,    47,     1,    -1,     3,    -1,    -1,     6,
       7,    -1,     9,    10,    11,    12,    13,    -1,    -1,    16,
      17,    18,    19,     1,    -1,     3,    -1,    -1,     6,     7,
      -1,     9,    10,    11,    12,    13,    -1,    -1,    16,    17,
      18,    19,    -1,    -1,    41,    42,    -1,    -1,    45,    46,
      47,    -1,    -1,     1,    -1,     3,     4,    -1,    -1,    -1,
      -1,     3,     4,    41,    42,    -1,     1,    45,     3,    47,
      -1,     6,     7,    -1,     9,    10,    11,    12,    13,    27,
      28,    16,    17,    18,    19,    27,    28,    -1,    -1,     3,
       4,    -1,    40,    41,    42,    43,    -1,    -1,    40,    41,
      42,    43,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,
      45,    -1,    47,    27,    28,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,
      -1,    -1,    -1,    27,    28,    41,    42,    43,    32,    33,
      34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      44
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    54,    55,     0,     1,     3,     6,     7,     9,    10,
      11,    12,    13,    16,    17,    18,    19,    41,    42,    45,
      47,    56,    57,    58,    62,    64,    65,    67,    74,    77,
      79,    82,    83,    84,    46,    47,    21,    22,    23,    24,
      25,    26,    41,    42,    43,     3,    80,    81,     1,    43,
       3,     4,    27,    28,    40,    41,    42,    43,    93,    94,
      95,    43,     1,    85,    85,    43,    47,    47,    43,    43,
       3,     3,    63,     1,    47,     1,    47,     1,    47,    47,
       5,    40,    43,    93,    93,    93,    93,    93,    93,    66,
      93,    21,    43,    48,     1,     3,    40,    43,    87,    88,
      89,    90,    91,    92,    93,    41,    42,    95,    95,    95,
       3,     3,    93,    27,    28,    29,    30,    31,     6,    79,
      43,    76,    93,     5,     5,     1,    46,    64,    43,    92,
      93,    44,    48,     5,    40,    43,    93,     6,    59,    60,
      61,     3,    81,    44,    21,    90,    95,    87,    93,    44,
      39,    38,    32,    33,    34,    35,    36,    37,    44,    94,
      94,    95,    95,    95,    47,     1,    87,    64,    44,    44,
      48,    48,    46,    92,    44,    47,    93,    43,    92,     3,
      44,    48,    85,    93,    44,    85,    85,    85,    93,    93,
      93,    93,    93,    93,    85,    44,    44,    11,    68,    47,
      93,    20,    44,    92,    44,    47,    62,    61,     7,    11,
      74,    64,    74,    89,    90,    87,    85,    75,    78,    85,
      45,    44,     3,    44,    43,    85,    86,    86,    47,    74,
      85,    85,    43,    14,    15,    69,    70,    71,    47,    44,
      87,    43,     8,     8,    85,    74,     7,    11,    77,    87,
       4,    49,    46,    71,    47,    44,    87,    85,    85,    92,
      93,    43,    85,    44,    49,    73,    85,    44,    74,    74,
      77,    86,    87,    43,    47,    72,    63,    74,    44,    44,
      87,    63,    86,    85,    85,    44,     8,    74,    74,    85,
      86,     8,    85
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    55,    55,    56,    56,    56,    57,    58,
      59,    59,    60,    60,    61,    62,    62,    63,    63,    64,
      64,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    66,
      66,    66,    68,    67,    69,    70,    70,    72,    71,    73,
      71,    74,    74,    74,    74,    75,    74,    74,    74,    76,
      74,    74,    77,    77,    78,    77,    79,    80,    80,    81,
      81,    81,    81,    81,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    83,    83,    83,    83,    84,    84,    85,
      86,    87,    88,    88,    89,    89,    90,    90,    91,    91,
      91,    91,    92,    92,    92,    92,    92,    92,    93,    93,
      93,    94,    94,    94,    94,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     6,     6,
       0,     1,     1,     3,     2,     3,     4,     2,     0,     1,
       1,     2,     2,     2,     2,     2,     2,     5,     5,     7,
       8,     1,     1,     1,     2,     2,     2,     2,     2,     0,
       1,     3,     0,     8,     1,     1,     2,     0,     5,     0,
       4,     1,    10,    10,     2,     0,     8,     7,     2,     0,
      10,    13,     6,    10,     0,     8,     2,     1,     3,     1,
       3,     3,     5,     6,     3,     3,     5,     6,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     1,     2,     0,
       0,     1,     4,     1,     4,     1,     2,     1,     3,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     3,     1,     3,     2,     2,     2,     2,
       2,     2,     2,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 8: /* function_def: TYPE ID LPAREN opt_param_list RPAREN compound_stmt  */
#line 61 "parser.y"
      { free((yyvsp[-4].sval)); }
#line 1949 "y.tab.c"
    break;

  case 9: /* function_proto: TYPE ID LPAREN opt_param_list RPAREN SEMI  */
#line 66 "parser.y"
      { free((yyvsp[-4].sval)); }
#line 1955 "y.tab.c"
    break;

  case 14: /* param_decl: TYPE ID  */
#line 81 "parser.y"
      {
          if (sym_declare((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column)) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "redefinition of '%s'", (yyvsp[0].sval));
              SymEntry *prev = sym_find((yyvsp[0].sval));
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", (yyvsp[0].sval));
          }
          free((yyvsp[0].sval));
      }
#line 1971 "y.tab.c"
    break;

  case 16: /* compound_stmt: LBRACE stmt_list error RBRACE  */
#line 97 "parser.y"
      {
          log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
              "expected '}' at end of compound statement");
          yyerrok;
      }
#line 1981 "y.tab.c"
    break;

  case 25: /* non_if_stmt: BREAK SEMI  */
#line 120 "parser.y"
      {
          /* defer the jump; the enclosing loop/switch backpatches it on exit */
          int g = emit_goto(-1);
          breaklist_add(g);
      }
#line 1991 "y.tab.c"
    break;

  case 26: /* non_if_stmt: CONTINUE SEMI  */
#line 126 "parser.y"
      {
          /* same deal, but target is the loop header (not the end) */
          int g = emit_goto(-1);
          continuelist_add(g);
      }
#line 2001 "y.tab.c"
    break;

  case 27: /* non_if_stmt: ID LPAREN arg_list RPAREN SEMI  */
#line 132 "parser.y"
      { free((yyvsp[-4].sval)); }
#line 2007 "y.tab.c"
    break;

  case 28: /* non_if_stmt: PRINTF LPAREN STRING RPAREN SEMI  */
#line 134 "parser.y"
      {
          emit_text("param %s", (yyvsp[-2].sval));
          emit_text("call printf, 1");
          free((yyvsp[-2].sval));
      }
#line 2017 "y.tab.c"
    break;

  case 29: /* non_if_stmt: PRINTF LPAREN STRING COMMA aexpr RPAREN SEMI  */
#line 140 "parser.y"
      {
          emit_text("param %s", (yyvsp[-2].a)->place);
          emit_text("param %s", (yyvsp[-4].sval));
          emit_text("call printf, 2");
          free_aattr((yyvsp[-2].a));
          free((yyvsp[-4].sval));
      }
#line 2029 "y.tab.c"
    break;

  case 30: /* non_if_stmt: SCANF LPAREN STRING COMMA AMPER ID RPAREN SEMI  */
#line 148 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_text("param &%s", (yyvsp[-2].sval));
          emit_text("param %s", (yyvsp[-5].sval));
          emit_text("call scanf, 2");
          free((yyvsp[-2].sval)); free((yyvsp[-5].sval));
      }
#line 2046 "y.tab.c"
    break;

  case 34: /* non_if_stmt: decl_stmt error  */
#line 164 "parser.y"
      {
          log_syntax_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "expected ';' after declaration");
          yyerrok;
      }
#line 2056 "y.tab.c"
    break;

  case 35: /* non_if_stmt: assign_stmt error  */
#line 170 "parser.y"
      {
          log_syntax_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "expected ';' after assignment");
          yyerrok;
      }
#line 2066 "y.tab.c"
    break;

  case 36: /* non_if_stmt: inc_stmt error  */
#line 176 "parser.y"
      {
          log_syntax_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "expected ';' after expression");
          yyerrok;
      }
#line 2076 "y.tab.c"
    break;

  case 37: /* non_if_stmt: error SEMI  */
#line 182 "parser.y"
      {
          if ((yylsp[-1]).first_line != last_syntax_error_line) {
              log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "expected expression before ';' token");
              last_syntax_error_line = (yylsp[-1]).first_line;
          }
          yyerrok;
      }
#line 2089 "y.tab.c"
    break;

  case 38: /* non_if_stmt: error RBRACE  */
#line 191 "parser.y"
      {
          if ((yylsp[-1]).first_line != last_syntax_error_line) {
              log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "expected expression before '}' token");
              last_syntax_error_line = (yylsp[-1]).first_line;
          }
          yyerrok;
      }
#line 2102 "y.tab.c"
    break;

  case 42: /* $@1: %empty  */
#line 219 "parser.y"
      {
          /* hold the switch expression in a stable temp so the
             dispatch can reference it repeatedly. copy into a temp
             in case the aexpr was a variable that gets modified
             inside the switch body. */
          char *tsw = new_temp();
          emit_text("%s = %s", tsw, (yyvsp[-1].a)->place);
          free_aattr((yyvsp[-1].a));
          /* stash the temp name and a placeholder end label that
             we'll fill in after the body. we pass the temp via $$
             of the mid-rule action. bison supports this via %union
             but the easier route is a static in this file. */
          extern char __sw_temp[64];
          extern IntList *__sw_dispatch_gotos;
          extern int __sw_has_default;
          strncpy(__sw_temp, tsw, 63); __sw_temp[63] = '\0';
          __sw_dispatch_gotos = NULL;
          __sw_has_default = 0;
          free(tsw);
          breaklist_push();
      }
#line 2128 "y.tab.c"
    break;

  case 43: /* switch_stmt: SWITCH LPAREN aexpr RPAREN $@1 LBRACE case_list RBRACE  */
#line 241 "parser.y"
      {
          /* nothing left except patching break. the cases already
             emitted their bodies and dispatch entries inline.
             the dispatch "goto L_end" falls through to here. */
          int end_lbl = nextinstr();
          breaklist_pop(end_lbl);
      }
#line 2140 "y.tab.c"
    break;

  case 47: /* $@2: %empty  */
#line 266 "parser.y"
      {
          /* emit: if t_sw == <const> goto L_caseN
             where L_caseN is the instruction right after this if.
             but the body comes next, so we make the dispatch jump
             forward to the body. the body is the very next instr
             after this if+goto pair. */
          extern char __sw_temp[64];
          char *cond = mkstr("%s == %s", __sw_temp, (yyvsp[-1].sval));
          int ifi = emit_if(cond);
          free(cond);
          /* the 'skip body' goto: if this case doesn't match, we
             want to skip OVER the body to the next dispatch test.
             but in C fall-through, a matched case runs the body
             and everything after. so we only need the conditional
             entry; no 'skip' goto here.

             mechanism: the if-taken target is the next instruction
             (body start). the if-not-taken falls through to the
             NEXT case's dispatch test. for that to work, we need
             the body to not run when falling through — we achieve
             this by emitting a goto past the body right after the
             if, which the body itself then lands after. */
          int skip_body = emit_goto(-1);
          /* patch the if to jump to the body start */
          patch_one(ifi, nextinstr());
          /* body starts here — record the skip_body index so we
             can patch it to the NEXT case's dispatch */
          free((yyvsp[-1].sval));
          /* stash skip_body in a small stack so case_list can chain */
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp < 128) __sw_skip_stack[__sw_skip_sp++] = skip_body;
      }
#line 2178 "y.tab.c"
    break;

  case 48: /* case_entry: CASE NUMBER COLON $@2 stmt_list  */
#line 300 "parser.y"
      {
          /* at end of this case's statements, patch the skip-body
             goto to point HERE (where the next dispatch starts). */
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp > 0) {
              int skip = __sw_skip_stack[--__sw_skip_sp];
              patch_one(skip, nextinstr());
          }
      }
#line 2193 "y.tab.c"
    break;

  case 49: /* $@3: %empty  */
#line 311 "parser.y"
      {
          /* default: no dispatch test, just a skip-body around the
             body for fall-through consistency. but default is
             reachable ONLY via fall-through from a prior unmatched
             case, OR from an explicit dispatch-miss jump. we handle
             the dispatch-miss by emitting a goto-to-default at the
             tail of all dispatches, which is done via patching at
             switch close time. */
          extern int __sw_has_default;
          extern int __sw_default_entry;
          __sw_has_default = 1;
          int skip_body = emit_goto(-1);
          __sw_default_entry = nextinstr();
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp < 128) __sw_skip_stack[__sw_skip_sp++] = skip_body;
      }
#line 2215 "y.tab.c"
    break;

  case 50: /* case_entry: DEFAULT COLON $@3 stmt_list  */
#line 329 "parser.y"
      {
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp > 0) {
              int skip = __sw_skip_stack[--__sw_skip_sp];
              patch_one(skip, nextinstr());
          }
      }
#line 2228 "y.tab.c"
    break;

  case 52: /* matched_stmt: IF LPAREN bexpr RPAREN M matched_stmt N ELSE M matched_stmt  */
#line 342 "parser.y"
      {
          backpatch((yyvsp[-7].b)->truelist, (yyvsp[-5].ival));
          backpatch((yyvsp[-7].b)->falselist, (yyvsp[-1].ival));
          backpatch((yyvsp[-3].list), nextinstr());
      }
#line 2238 "y.tab.c"
    break;

  case 53: /* matched_stmt: IF LPAREN error RPAREN M matched_stmt N ELSE M matched_stmt  */
#line 348 "parser.y"
      {
          log_syntax_error((yylsp[-7]).first_line, (yylsp[-7]).first_column,
              "expected expression in 'if' condition");
          yyerrok;
      }
#line 2248 "y.tab.c"
    break;

  case 54: /* matched_stmt: IF error  */
#line 354 "parser.y"
      {
          log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column + 2,
              "expected '(' after 'if'");
          yyerrok;
      }
#line 2258 "y.tab.c"
    break;

  case 55: /* $@4: %empty  */
#line 360 "parser.y"
      {
          /* push break/continue frames right before the body.
             continue targets the M at position $2, break targets
             whatever comes after the body. */
          breaklist_push();
          continuelist_push();
      }
#line 2270 "y.tab.c"
    break;

  case 56: /* matched_stmt: WHILE M LPAREN bexpr RPAREN $@4 M matched_stmt  */
#line 368 "parser.y"
      {
          backpatch((yyvsp[-4].b)->truelist, (yyvsp[-1].ival));
          emit_goto((yyvsp[-6].ival));
          int end_lbl = nextinstr();
          backpatch((yyvsp[-4].b)->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop((yyvsp[-6].ival));
      }
#line 2283 "y.tab.c"
    break;

  case 57: /* matched_stmt: WHILE M LPAREN error RPAREN M matched_stmt  */
#line 377 "parser.y"
      {
          log_syntax_error((yylsp[-3]).first_line, (yylsp[-3]).first_column,
              "expected expression in 'while' condition");
          yyerrok;
      }
#line 2293 "y.tab.c"
    break;

  case 58: /* matched_stmt: WHILE error  */
#line 383 "parser.y"
      {
          log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column + 5,
              "expected '(' after 'while'");
          yyerrok;
      }
#line 2303 "y.tab.c"
    break;

  case 59: /* $@5: %empty  */
#line 389 "parser.y"
      {
          breaklist_push();
          continuelist_push();
      }
#line 2312 "y.tab.c"
    break;

  case 60: /* matched_stmt: DO M $@5 statement WHILE M LPAREN bexpr RPAREN SEMI  */
#line 394 "parser.y"
      {
          backpatch((yyvsp[-2].b)->truelist, (yyvsp[-8].ival));
          int end_lbl = nextinstr();
          backpatch((yyvsp[-2].b)->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop((yyvsp[-4].ival));
      }
#line 2324 "y.tab.c"
    break;

  case 61: /* matched_stmt: FOR LPAREN decl_stmt SEMI M bexpr SEMI M rel_bool N RPAREN M matched_stmt  */
#line 402 "parser.y"
      { /* for loop stub — left as-is from original, pending real implementation */ }
#line 2330 "y.tab.c"
    break;

  case 62: /* unmatched_stmt: IF LPAREN bexpr RPAREN M statement  */
#line 407 "parser.y"
      {
          backpatch((yyvsp[-3].b)->truelist, (yyvsp[-1].ival));
          backpatch((yyvsp[-3].b)->falselist, nextinstr());
      }
#line 2339 "y.tab.c"
    break;

  case 63: /* unmatched_stmt: IF LPAREN bexpr RPAREN M matched_stmt N ELSE M unmatched_stmt  */
#line 412 "parser.y"
      {
          backpatch((yyvsp[-7].b)->truelist, (yyvsp[-5].ival));
          backpatch((yyvsp[-7].b)->falselist, (yyvsp[-1].ival));
          backpatch((yyvsp[-3].list), nextinstr());
      }
#line 2349 "y.tab.c"
    break;

  case 64: /* $@6: %empty  */
#line 418 "parser.y"
      {
          breaklist_push();
          continuelist_push();
      }
#line 2358 "y.tab.c"
    break;

  case 65: /* unmatched_stmt: WHILE M LPAREN bexpr RPAREN $@6 M unmatched_stmt  */
#line 423 "parser.y"
      {
          backpatch((yyvsp[-4].b)->truelist, (yyvsp[-1].ival));
          emit_goto((yyvsp[-6].ival));
          int end_lbl = nextinstr();
          backpatch((yyvsp[-4].b)->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop((yyvsp[-6].ival));
      }
#line 2371 "y.tab.c"
    break;

  case 69: /* decl_item: ID  */
#line 444 "parser.y"
      {
          if (sym_declare((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column)) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "redefinition of '%s'", (yyvsp[0].sval));
              SymEntry *prev = sym_find((yyvsp[0].sval));
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", (yyvsp[0].sval));
          }
          free((yyvsp[0].sval));
      }
#line 2387 "y.tab.c"
    break;

  case 70: /* decl_item: ID ASSIGN aexpr  */
#line 456 "parser.y"
      {
          if (sym_declare((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column)) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "redefinition of '%s'", (yyvsp[-2].sval));
              SymEntry *prev = sym_find((yyvsp[-2].sval));
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", (yyvsp[-2].sval));
          } else {
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_text("%s = %s", (yyvsp[-2].sval), (yyvsp[0].a)->place);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2406 "y.tab.c"
    break;

  case 71: /* decl_item: ID ASSIGN STRING  */
#line 471 "parser.y"
      {
          if (sym_declare((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column)) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "redefinition of '%s'", (yyvsp[-2].sval));
          }
          log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "incompatible types when initializing type 'int' using type 'char *'");
          free((yyvsp[-2].sval)); free((yyvsp[0].sval));
      }
#line 2420 "y.tab.c"
    break;

  case 72: /* decl_item: ID ASSIGN LPAREN rel_bool RPAREN  */
#line 481 "parser.y"
      {
          if (sym_declare((yyvsp[-4].sval), (yylsp[-4]).first_line, (yylsp[-4]).first_column)) {
              log_sem_error((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                  "redefinition of '%s'", (yyvsp[-4].sval));
              SymEntry *prev = sym_find((yyvsp[-4].sval));
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", (yyvsp[-4].sval));
          } else {
              sym_mark_init((yyvsp[-4].sval));
          }
          emit_bool_assignment((yyvsp[-4].sval), (yyvsp[-1].b));
          free((yyvsp[-4].sval));
      }
#line 2439 "y.tab.c"
    break;

  case 73: /* decl_item: ID ASSIGN NOT LPAREN rel_bool RPAREN  */
#line 496 "parser.y"
      {
          if (sym_declare((yyvsp[-5].sval), (yylsp[-5]).first_line, (yylsp[-5]).first_column)) {
              log_sem_error((yylsp[-5]).first_line, (yylsp[-5]).first_column,
                  "redefinition of '%s'", (yyvsp[-5].sval));
              SymEntry *prev = sym_find((yyvsp[-5].sval));
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", (yyvsp[-5].sval));
          } else {
              sym_mark_init((yyvsp[-5].sval));
          }
          IntList *tmp = (yyvsp[-1].b)->truelist;
          (yyvsp[-1].b)->truelist = (yyvsp[-1].b)->falselist;
          (yyvsp[-1].b)->falselist = tmp;
          emit_bool_assignment((yyvsp[-5].sval), (yyvsp[-1].b));
          free((yyvsp[-5].sval));
      }
#line 2461 "y.tab.c"
    break;

  case 74: /* assign_stmt: ID ASSIGN aexpr  */
#line 517 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_init((yyvsp[-2].sval));
              if (strcmp((yyvsp[-2].sval), (yyvsp[0].a)->place) == 0)
                  emit_diagnostic("<stdin>", (yylsp[-2]).first_line, (yylsp[-2]).first_column,
                      "warning",
                      "explicitly assigning value of variable '%s' to itself [-Wself-assign]",
                      (yyvsp[-2].sval));
          }
          emit_text("%s = %s", (yyvsp[-2].sval), (yyvsp[0].a)->place);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2481 "y.tab.c"
    break;

  case 75: /* assign_stmt: ID ASSIGN STRING  */
#line 533 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          }
          log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "incompatible types when assigning to type 'int' from type 'char *'");
          free((yyvsp[-2].sval)); free((yyvsp[0].sval));
      }
#line 2495 "y.tab.c"
    break;

  case 76: /* assign_stmt: ID ASSIGN LPAREN rel_bool RPAREN  */
#line 543 "parser.y"
      {
          if (!sym_lookup((yyvsp[-4].sval))) {
              log_sem_error((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-4].sval));
          } else {
              sym_mark_init((yyvsp[-4].sval));
          }
          emit_bool_assignment((yyvsp[-4].sval), (yyvsp[-1].b));
          free((yyvsp[-4].sval));
      }
#line 2510 "y.tab.c"
    break;

  case 77: /* assign_stmt: ID ASSIGN NOT LPAREN rel_bool RPAREN  */
#line 554 "parser.y"
      {
          if (!sym_lookup((yyvsp[-5].sval))) {
              log_sem_error((yylsp[-5]).first_line, (yylsp[-5]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-5].sval));
          } else {
              sym_mark_init((yyvsp[-5].sval));
          }
          IntList *tmp = (yyvsp[-1].b)->truelist;
          (yyvsp[-1].b)->truelist = (yyvsp[-1].b)->falselist;
          (yyvsp[-1].b)->falselist = tmp;
          emit_bool_assignment((yyvsp[-5].sval), (yyvsp[-1].b));
          free((yyvsp[-5].sval));
      }
#line 2528 "y.tab.c"
    break;

  case 78: /* assign_stmt: ID ADD_ASSIGN aexpr  */
#line 568 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "+", (yyvsp[0].a));
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2544 "y.tab.c"
    break;

  case 79: /* assign_stmt: ID SUB_ASSIGN aexpr  */
#line 580 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "-", (yyvsp[0].a));
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2560 "y.tab.c"
    break;

  case 80: /* assign_stmt: ID MUL_ASSIGN aexpr  */
#line 592 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "*", (yyvsp[0].a));
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2576 "y.tab.c"
    break;

  case 81: /* assign_stmt: ID DIV_ASSIGN aexpr  */
#line 604 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "/", (yyvsp[0].a));
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2592 "y.tab.c"
    break;

  case 82: /* assign_stmt: ID MOD_ASSIGN aexpr  */
#line 616 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "%", (yyvsp[0].a));
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2608 "y.tab.c"
    break;

  case 83: /* inc_stmt: ID INC  */
#line 631 "parser.y"
      {
          if (!sym_lookup((yyvsp[-1].sval))) {
              log_sem_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-1].sval));
          } else {
              sym_mark_used((yyvsp[-1].sval), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
              sym_mark_init((yyvsp[-1].sval));
          }
          emit_incdec((yyvsp[-1].sval), 1); free((yyvsp[-1].sval));
      }
#line 2623 "y.tab.c"
    break;

  case 84: /* inc_stmt: ID DEC  */
#line 642 "parser.y"
      {
          if (!sym_lookup((yyvsp[-1].sval))) {
              log_sem_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-1].sval));
          } else {
              sym_mark_used((yyvsp[-1].sval), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
              sym_mark_init((yyvsp[-1].sval));
          }
          emit_incdec((yyvsp[-1].sval), 0); free((yyvsp[-1].sval));
      }
#line 2638 "y.tab.c"
    break;

  case 85: /* inc_stmt: INC ID  */
#line 653 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
              sym_mark_init((yyvsp[0].sval));
          }
          emit_incdec((yyvsp[0].sval), 1); free((yyvsp[0].sval));
      }
#line 2653 "y.tab.c"
    break;

  case 86: /* inc_stmt: DEC ID  */
#line 664 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
              sym_mark_init((yyvsp[0].sval));
          }
          emit_incdec((yyvsp[0].sval), 0); free((yyvsp[0].sval));
      }
#line 2668 "y.tab.c"
    break;

  case 87: /* return_stmt: RETURN  */
#line 677 "parser.y"
                         { emit_text("return"); }
#line 2674 "y.tab.c"
    break;

  case 88: /* return_stmt: RETURN aexpr  */
#line 678 "parser.y"
                         { emit_text("return %s", (yyvsp[0].a)->place); free_aattr((yyvsp[0].a)); }
#line 2680 "y.tab.c"
    break;

  case 89: /* M: %empty  */
#line 681 "parser.y"
                { (yyval.ival) = nextinstr(); }
#line 2686 "y.tab.c"
    break;

  case 90: /* N: %empty  */
#line 682 "parser.y"
                { (yyval.list) = makelist(emit_goto(-1)); }
#line 2692 "y.tab.c"
    break;

  case 91: /* bexpr: bor_expr  */
#line 684 "parser.y"
                     { (yyval.b) = (yyvsp[0].b); }
#line 2698 "y.tab.c"
    break;

  case 92: /* bor_expr: bor_expr OR M band_expr  */
#line 688 "parser.y"
      {
          backpatch((yyvsp[-3].b)->falselist, (yyvsp[-1].ival));
          (yyval.b) = make_battr(merge_list((yyvsp[-3].b)->truelist, (yyvsp[0].b)->truelist), (yyvsp[0].b)->falselist);
      }
#line 2707 "y.tab.c"
    break;

  case 93: /* bor_expr: band_expr  */
#line 692 "parser.y"
                { (yyval.b) = (yyvsp[0].b); }
#line 2713 "y.tab.c"
    break;

  case 94: /* band_expr: band_expr AND M bnot_expr  */
#line 697 "parser.y"
      {
          backpatch((yyvsp[-3].b)->truelist, (yyvsp[-1].ival));
          (yyval.b) = make_battr((yyvsp[0].b)->truelist, merge_list((yyvsp[-3].b)->falselist, (yyvsp[0].b)->falselist));
      }
#line 2722 "y.tab.c"
    break;

  case 95: /* band_expr: bnot_expr  */
#line 701 "parser.y"
                { (yyval.b) = (yyvsp[0].b); }
#line 2728 "y.tab.c"
    break;

  case 96: /* bnot_expr: NOT bnot_expr  */
#line 705 "parser.y"
                    { (yyval.b) = make_battr((yyvsp[0].b)->falselist, (yyvsp[0].b)->truelist); }
#line 2734 "y.tab.c"
    break;

  case 97: /* bnot_expr: bprimary  */
#line 706 "parser.y"
                    { (yyval.b) = (yyvsp[0].b); }
#line 2740 "y.tab.c"
    break;

  case 98: /* bprimary: LPAREN bexpr RPAREN  */
#line 717 "parser.y"
      { (yyval.b) = (yyvsp[-1].b); }
#line 2746 "y.tab.c"
    break;

  case 99: /* bprimary: rel_bool  */
#line 719 "parser.y"
      { (yyval.b) = (yyvsp[0].b); }
#line 2752 "y.tab.c"
    break;

  case 100: /* bprimary: ID ASSIGN aexpr  */
#line 721 "parser.y"
      {
          /* someone wrote = instead of == inside a condition */
          emit_diagnostic("<stdin>", (yylsp[-1]).first_line, (yylsp[-1]).first_column,
              "warning",
              "suggest parentheses around assignment used as truth value [-Wparentheses]");
          emit_diagnostic("<stdin>", (yylsp[-1]).first_line, (yylsp[-1]).first_column,
              "note", "use '==' to check for equality");
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_text("%s = %s", (yyvsp[-2].sval), (yyvsp[0].a)->place);
          (yyval.b) = emit_truthy((yyvsp[-2].sval));
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2774 "y.tab.c"
    break;

  case 101: /* bprimary: aexpr  */
#line 739 "parser.y"
      {
          /* arithmetic-as-truthy: treat nonzero as true */
          (yyval.b) = emit_truthy((yyvsp[0].a)->place);
          free_aattr((yyvsp[0].a));
      }
#line 2784 "y.tab.c"
    break;

  case 102: /* rel_bool: aexpr LT aexpr  */
#line 747 "parser.y"
                      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, "<",  (yyvsp[0].a)->place); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2790 "y.tab.c"
    break;

  case 103: /* rel_bool: aexpr GT aexpr  */
#line 748 "parser.y"
                      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, ">",  (yyvsp[0].a)->place); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2796 "y.tab.c"
    break;

  case 104: /* rel_bool: aexpr LE aexpr  */
#line 749 "parser.y"
                      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, "<=", (yyvsp[0].a)->place); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2802 "y.tab.c"
    break;

  case 105: /* rel_bool: aexpr GE aexpr  */
#line 750 "parser.y"
                      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, ">=", (yyvsp[0].a)->place); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2808 "y.tab.c"
    break;

  case 106: /* rel_bool: aexpr EQ aexpr  */
#line 751 "parser.y"
                      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, "==", (yyvsp[0].a)->place); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2814 "y.tab.c"
    break;

  case 107: /* rel_bool: aexpr NE aexpr  */
#line 752 "parser.y"
                      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, "!=", (yyvsp[0].a)->place); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2820 "y.tab.c"
    break;

  case 108: /* aexpr: aexpr PLUS term  */
#line 757 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s + %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place); (yyval.a) = make_aattr(t); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2826 "y.tab.c"
    break;

  case 109: /* aexpr: aexpr MINUS term  */
#line 759 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s - %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place); (yyval.a) = make_aattr(t); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2832 "y.tab.c"
    break;

  case 110: /* aexpr: term  */
#line 761 "parser.y"
      { (yyval.a) = (yyvsp[0].a); }
#line 2838 "y.tab.c"
    break;

  case 111: /* term: term MUL factor  */
#line 766 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s * %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place); (yyval.a) = make_aattr(t); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2844 "y.tab.c"
    break;

  case 112: /* term: term DIV factor  */
#line 768 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s / %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place); (yyval.a) = make_aattr(t); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2850 "y.tab.c"
    break;

  case 113: /* term: term MOD factor  */
#line 770 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s %% %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place); (yyval.a) = make_aattr(t); free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2856 "y.tab.c"
    break;

  case 114: /* term: factor  */
#line 772 "parser.y"
      { (yyval.a) = (yyvsp[0].a); }
#line 2862 "y.tab.c"
    break;

  case 115: /* factor: LPAREN aexpr RPAREN  */
#line 776 "parser.y"
                          { (yyval.a) = (yyvsp[-1].a); }
#line 2868 "y.tab.c"
    break;

  case 116: /* factor: INC ID  */
#line 778 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
              sym_mark_init((yyvsp[0].sval));
          }
          /* pre-increment: bump first, then use the new value */
          emit_incdec((yyvsp[0].sval), 1); (yyval.a) = make_aattr((yyvsp[0].sval));
      }
#line 2884 "y.tab.c"
    break;

  case 117: /* factor: DEC ID  */
#line 790 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
              sym_mark_init((yyvsp[0].sval));
          }
          /* pre-decrement: bump first, then use the new value */
          emit_incdec((yyvsp[0].sval), 0); (yyval.a) = make_aattr((yyvsp[0].sval));
      }
#line 2900 "y.tab.c"
    break;

  case 118: /* factor: ID INC  */
#line 802 "parser.y"
      {
          if (!sym_lookup((yyvsp[-1].sval))) {
              log_sem_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-1].sval));
          } else {
              sym_mark_used((yyvsp[-1].sval), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
              sym_mark_init((yyvsp[-1].sval));
          }
          /* post-increment: use the old value, then bump */
          char *old = new_temp();
          emit_text("%s = %s", old, (yyvsp[-1].sval));
          emit_incdec((yyvsp[-1].sval), 1);
          (yyval.a) = make_aattr(old);
          free((yyvsp[-1].sval));
      }
#line 2920 "y.tab.c"
    break;

  case 119: /* factor: ID DEC  */
#line 818 "parser.y"
      {
          if (!sym_lookup((yyvsp[-1].sval))) {
              log_sem_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-1].sval));
          } else {
              sym_mark_used((yyvsp[-1].sval), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
              sym_mark_init((yyvsp[-1].sval));
          }
          /* post-decrement: use the old value, then bump */
          char *old = new_temp();
          emit_text("%s = %s", old, (yyvsp[-1].sval));
          emit_incdec((yyvsp[-1].sval), 0);
          (yyval.a) = make_aattr(old);
          free((yyvsp[-1].sval));
      }
#line 2940 "y.tab.c"
    break;

  case 120: /* factor: MINUS factor  */
#line 834 "parser.y"
      {
          char *t = new_temp();
          emit_text("%s = 0 - %s", t, (yyvsp[0].a)->place);
          (yyval.a) = make_aattr(t);
          free_aattr((yyvsp[0].a));
      }
#line 2951 "y.tab.c"
    break;

  case 121: /* factor: PLUS factor  */
#line 841 "parser.y"
      {
          /* unary plus is a no-op, just pass through */
          (yyval.a) = (yyvsp[0].a);
      }
#line 2960 "y.tab.c"
    break;

  case 122: /* factor: NOT factor  */
#line 846 "parser.y"
      {
          /* logical NOT in arithmetic context: !x becomes (x == 0 ? 1 : 0) */
          char *t = new_temp();
          int false_lbl = nextinstr();
          emit_text("%s = 1", t);
          char *cond = mkstr("%s != 0", (yyvsp[0].a)->place);
          int i = emit_if(cond);
          free(cond);
          int skip = emit_goto(-1);
          int true_lbl = nextinstr();
          emit_text("%s = 0", t);
          int end_lbl = nextinstr();
          patch_one(i, true_lbl);
          patch_one(skip, end_lbl);
          (yyval.a) = make_aattr(t);
          free_aattr((yyvsp[0].a));
      }
#line 2982 "y.tab.c"
    break;

  case 123: /* factor: ID  */
#line 864 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
          }
          (yyval.a) = make_aattr((yyvsp[0].sval));
      }
#line 2996 "y.tab.c"
    break;

  case 124: /* factor: NUMBER  */
#line 874 "parser.y"
      { (yyval.a) = make_aattr((yyvsp[0].sval)); }
#line 3002 "y.tab.c"
    break;


#line 3006 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 877 "parser.y"


/* ---- switch-state globals referenced from the grammar actions ----
   these live at file scope so the mid-rule actions in switch_stmt
   and case_entry can share state without threading it through
   %union. not the prettiest design but it keeps the grammar flat. */
char __sw_temp[64];
IntList *__sw_dispatch_gotos = NULL;
int __sw_has_default = 0;
int __sw_default_entry = -1;
int __sw_skip_stack[128];
int __sw_skip_sp = 0;
