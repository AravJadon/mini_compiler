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




/* First part of user prologue.  */
#line 1 "parser.y"

#include "common.h"

/* ---- AST construction helpers (parser-local) ----
   Grammar actions build an AST fragment for each reduction.
   Expressions carry their fragment up via AAttr->node / BAttr->node.
   Statements don't have an %union slot for nodes, so they push
   themselves onto the current "statement-list accumulator" node
   sitting on top of ast_stmt_stack. LBRACE pushes a new empty
   Block node; RBRACE pops it and hands the block up to whoever
   opened it (function body, compound statement, case body, etc.).
   The program-level accumulator is pushed once at start-up. */

#define AST_STMT_STACK_MAX 64
static ASTNode *ast_stmt_stack[AST_STMT_STACK_MAX];
static int      ast_stmt_sp = 0;

static void ast_push_block(ASTNode *block) {
    if (ast_stmt_sp < AST_STMT_STACK_MAX)
        ast_stmt_stack[ast_stmt_sp++] = block;
}
static ASTNode *ast_pop_block(void) {
    if (ast_stmt_sp == 0) return NULL;
    return ast_stmt_stack[--ast_stmt_sp];
}
static ASTNode *ast_top_block(void) {
    if (ast_stmt_sp == 0) return NULL;
    return ast_stmt_stack[ast_stmt_sp - 1];
}
static void ast_append_stmt(ASTNode *stmt) {
    ASTNode *top = ast_top_block();
    if (top && stmt) ast_add_child(top, stmt);
}

/* Switch-case bookkeeping: each switch pushes an AST "switch" node,
   and each case's body reduces into its own block that is then
   attached to the switch node. We reuse the same stmt-stack for
   the case bodies — a CASE/DEFAULT pushes a new block before its
   stmt_list and pops it back into a case_entry node afterward. */
#define AST_SWITCH_STACK_MAX 32
static ASTNode *ast_switch_stack[AST_SWITCH_STACK_MAX];
static int      ast_switch_sp = 0;
static void ast_push_switch(ASTNode *s) {
    if (ast_switch_sp < AST_SWITCH_STACK_MAX)
        ast_switch_stack[ast_switch_sp++] = s;
}
static ASTNode *ast_pop_switch(void) {
    if (ast_switch_sp == 0) return NULL;
    return ast_switch_stack[--ast_switch_sp];
}
static ASTNode *ast_top_switch(void) {
    if (ast_switch_sp == 0) return NULL;
    return ast_switch_stack[ast_switch_sp - 1];
}

/* Per-case label carried from the CASE/DEFAULT action to the
   closing action that packages the case body. Stacked because
   a case body can contain a nested switch. */
#define AST_CASE_STACK_MAX 64
static ASTNode *ast_case_label_stack[AST_CASE_STACK_MAX];
static int      ast_case_label_sp = 0;
static void ast_push_case_label(ASTNode *lbl) {
    if (ast_case_label_sp < AST_CASE_STACK_MAX)
        ast_case_label_stack[ast_case_label_sp++] = lbl;
}
static ASTNode *ast_pop_case_label(void) {
    if (ast_case_label_sp == 0) return NULL;
    return ast_case_label_stack[--ast_case_label_sp];
}

#line 142 "parser.tab.c"

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

#include "parser.tab.h"
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
  YYSYMBOL_55_1 = 55,                      /* $@1  */
  YYSYMBOL_external_list = 56,             /* external_list  */
  YYSYMBOL_external = 57,                  /* external  */
  YYSYMBOL_function_def = 58,              /* function_def  */
  YYSYMBOL_function_proto = 59,            /* function_proto  */
  YYSYMBOL_function_header = 60,           /* function_header  */
  YYSYMBOL_61_2 = 61,                      /* $@2  */
  YYSYMBOL_opt_param_list = 62,            /* opt_param_list  */
  YYSYMBOL_param_list = 63,                /* param_list  */
  YYSYMBOL_param_decl = 64,                /* param_decl  */
  YYSYMBOL_compound_stmt = 65,             /* compound_stmt  */
  YYSYMBOL_stmt_list = 66,                 /* stmt_list  */
  YYSYMBOL_statement = 67,                 /* statement  */
  YYSYMBOL_non_if_stmt = 68,               /* non_if_stmt  */
  YYSYMBOL_arg_list = 69,                  /* arg_list  */
  YYSYMBOL_switch_stmt = 70,               /* switch_stmt  */
  YYSYMBOL_71_3 = 71,                      /* $@3  */
  YYSYMBOL_case_list = 72,                 /* case_list  */
  YYSYMBOL_case_entries = 73,              /* case_entries  */
  YYSYMBOL_case_entry = 74,                /* case_entry  */
  YYSYMBOL_75_4 = 75,                      /* $@4  */
  YYSYMBOL_76_5 = 76,                      /* $@5  */
  YYSYMBOL_decl_stmt = 77,                 /* decl_stmt  */
  YYSYMBOL_decl_items = 78,                /* decl_items  */
  YYSYMBOL_decl_item = 79,                 /* decl_item  */
  YYSYMBOL_assign_stmt = 80,               /* assign_stmt  */
  YYSYMBOL_inc_stmt = 81,                  /* inc_stmt  */
  YYSYMBOL_return_stmt = 82,               /* return_stmt  */
  YYSYMBOL_M = 83,                         /* M  */
  YYSYMBOL_N = 84,                         /* N  */
  YYSYMBOL_push_block = 85,                /* push_block  */
  YYSYMBOL_push_capture = 86,              /* push_capture  */
  YYSYMBOL_loop_enter = 87,                /* loop_enter  */
  YYSYMBOL_bexpr = 88,                     /* bexpr  */
  YYSYMBOL_bor_expr = 89,                  /* bor_expr  */
  YYSYMBOL_band_expr = 90,                 /* band_expr  */
  YYSYMBOL_bnot_expr = 91,                 /* bnot_expr  */
  YYSYMBOL_bprimary = 92,                  /* bprimary  */
  YYSYMBOL_rel_bool = 93,                  /* rel_bool  */
  YYSYMBOL_aexpr = 94,                     /* aexpr  */
  YYSYMBOL_term = 95,                      /* term  */
  YYSYMBOL_factor = 96                     /* factor  */
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
#define YYLAST   398

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  260

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
       0,   112,   112,   112,   126,   127,   131,   132,   133,   137,
     147,   159,   158,   174,   175,   179,   180,   184,   204,   214,
     225,   226,   230,   231,   243,   259,   265,   271,   277,   293,
     299,   305,   320,   338,   339,   340,   341,   342,   349,   356,
     367,   376,   388,   405,   406,   407,   413,   419,   425,   431,
     443,   444,   445,   460,   459,   502,   506,   507,   517,   516,
     567,   566,   604,   608,   609,   613,   629,   649,   659,   679,
     706,   726,   736,   751,   769,   786,   803,   820,   837,   857,
     872,   887,   902,   920,   925,   935,   936,   942,   950,   958,
     965,   968,   974,   978,   984,   988,   990,  1000,  1002,  1004,
    1024,  1034,  1038,  1042,  1046,  1050,  1054,  1061,  1065,  1069,
    1074,  1078,  1082,  1086,  1091,  1092,  1107,  1122,  1139,  1156,
    1163,  1168,  1178
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
  "LOWER_THAN_ELSE", "UMINUS", "$accept", "program", "$@1",
  "external_list", "external", "function_def", "function_proto",
  "function_header", "$@2", "opt_param_list", "param_list", "param_decl",
  "compound_stmt", "stmt_list", "statement", "non_if_stmt", "arg_list",
  "switch_stmt", "$@3", "case_list", "case_entries", "case_entry", "$@4",
  "$@5", "decl_stmt", "decl_items", "decl_item", "assign_stmt", "inc_stmt",
  "return_stmt", "M", "N", "push_block", "push_capture", "loop_enter",
  "bexpr", "bor_expr", "band_expr", "bnot_expr", "bprimary", "rel_bool",
  "aexpr", "term", "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-236)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-86)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -236,    26,  -236,  -236,   149,   -10,   190,    50,    11,   342,
      16,    12,  -236,    22,    38,    40,    46,    54,    90,    91,
    -236,  -236,  -236,  -236,  -236,    23,  -236,  -236,  -236,  -236,
       4,     5,    10,    55,  -236,    71,   342,   342,   342,   342,
     342,  -236,  -236,   342,    -7,    53,  -236,  -236,    20,   -24,
    -236,   342,   342,   107,   113,   342,    14,    78,  -236,  -236,
    -236,    74,  -236,   342,  -236,  -236,   115,   116,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,    79,   200,    14,    14,    14,    14,    14,    14,   -28,
      14,   182,  -236,   121,    83,    -2,   200,   200,    84,    95,
      97,  -236,  -236,  -236,   262,  -236,  -236,  -236,  -236,  -236,
    -236,     0,   342,   342,   342,   342,   342,   130,    63,   331,
      51,   -19,    89,   312,   200,   100,   354,    98,   342,  -236,
     103,   200,    14,   142,   132,  -236,  -236,   342,  -236,   110,
    -236,  -236,  -236,   342,   342,   342,   342,   342,   342,  -236,
      78,    78,  -236,  -236,  -236,   121,   104,   119,   120,   158,
    -236,   123,   342,   151,    -1,  -236,  -236,   128,  -236,  -236,
      14,   200,   131,   171,   133,   134,  -236,   331,    14,  -236,
    -236,   200,   200,    14,    14,    14,    14,    14,    14,  -236,
    -236,  -236,  -236,   135,  -236,    56,   173,  -236,  -236,   139,
    -236,  -236,  -236,   142,   176,   331,    97,  -236,   200,   331,
    -236,   145,    67,   146,   148,  -236,  -236,  -236,   181,   150,
    -236,   331,   200,   191,   153,   152,    67,  -236,  -236,   160,
     331,  -236,  -236,  -236,   156,   159,  -236,  -236,  -236,  -236,
    -236,  -236,   342,   172,  -236,  -236,  -236,  -236,   262,  -236,
    -236,   246,   331,   174,   265,  -236,  -236,  -236,   331,  -236
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     5,     1,     0,     0,     0,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,    45,     4,     6,     7,     0,    44,     8,    22,    43,
       0,     0,     0,     0,    49,     0,     0,     0,     0,     0,
       0,    79,    80,    50,    65,    62,    63,    27,     0,   121,
     122,     0,     0,     0,     0,     0,    84,   109,   113,    88,
      30,     0,    89,     0,    37,    38,     0,     0,    81,    82,
      21,    10,     9,    46,    33,    47,    34,    48,    35,    36,
      71,     0,     0,    70,    74,    75,    76,    77,    78,     0,
      51,     0,    11,     0,     0,   121,     0,     0,     0,    90,
      92,    94,    96,    98,   100,   117,   118,   120,   119,   115,
     116,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,    67,
       0,     0,    66,    13,    65,    64,    85,     0,    95,     0,
      85,    85,    85,     0,     0,     0,     0,     0,     0,   114,
     107,   108,   110,   111,   112,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,    18,    20,     0,    72,    39,
      52,     0,     0,     0,     0,    14,    15,     0,    99,    97,
      88,     0,     0,   101,   102,   103,   104,   105,   106,    85,
      85,    89,    85,     0,    40,     0,     0,    19,    73,     0,
      68,    17,    12,     0,    25,     0,    91,    93,     0,     0,
      85,     0,     0,     0,     0,    69,    16,    85,    23,     0,
      29,     0,     0,     0,     0,     0,    55,    56,    41,     0,
       0,    86,    85,    28,     0,     0,    60,    54,    57,    42,
      26,    85,     0,     0,    58,    21,    88,    86,     0,    31,
      21,     0,     0,     0,     0,    24,    85,    88,     0,    32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,    17,   204,  -235,    -4,  -236,  -236,  -236,  -236,  -236,
    -236,     8,  -236,  -236,   118,  -236,   143,  -236,  -236,  -236,
     -11,    -9,  -236,  -177,    48,   -75,  -236,    65,   -94,  -236,
       2,    -5,     6,   -43
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     4,    22,    23,    24,    25,   133,   174,
     175,   176,    26,   123,   166,    28,    89,    29,   193,   225,
     226,   227,   250,   245,    30,    45,    46,    31,    32,    33,
      61,   241,    70,   117,   119,    98,    99,   100,   101,   102,
     103,   104,    57,    58
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      27,    62,   138,   205,    56,    73,    75,   125,   107,   108,
     251,    77,    47,    60,    91,   254,   127,   105,   106,   137,
     128,    94,   139,    95,    50,   161,     3,   112,   113,   162,
      83,    84,    85,    86,    87,    88,    92,    34,    90,   105,
     106,   112,   113,   158,   149,   197,    34,    51,    52,   167,
     111,    74,    76,    44,    48,   -85,   172,    78,   120,    59,
      96,    53,    54,    97,   157,    63,    95,    50,    20,   252,
      71,   152,   153,   154,    49,    50,    80,   126,   112,   113,
     258,   223,   224,   112,   113,    64,   132,    65,   207,    66,
      51,    52,   126,    68,    69,   160,   199,    67,    51,    52,
     213,    93,    79,    96,    53,    54,    97,   114,   115,   116,
     109,    81,    53,    54,    82,   159,   110,   118,   150,   151,
     121,   122,   124,   170,   134,   177,   126,   136,   140,   180,
     181,   182,   178,   219,   141,   142,   155,   163,   183,   184,
     185,   186,   187,   188,   168,   169,   171,   234,   173,    -3,
       5,   189,     6,    91,   179,     7,     8,   195,     9,    10,
      11,    12,    13,   190,   191,    14,    15,    16,    17,   192,
     194,   196,   198,   204,   201,   200,   214,   202,   208,   209,
     212,   211,   203,   215,   217,    49,    50,   129,   222,   231,
      18,    19,   229,   228,    20,   235,    21,   232,   237,   221,
     243,   218,   236,    95,    50,   220,   230,   239,   244,    51,
      52,    35,    36,    37,    38,    39,    40,   233,   256,   249,
     216,   242,   130,    53,    54,   131,   240,    51,    52,    72,
     246,    41,    42,    43,   238,   156,   135,   248,   253,   210,
      96,    53,    54,    97,   247,   257,   206,     5,   255,     6,
       0,     0,   155,     8,   259,     9,    10,    11,    12,    13,
     -61,   -61,    14,    15,    16,    17,     5,     0,     6,     0,
       0,   155,     8,     0,     9,    10,    11,    12,    13,   -59,
     -59,    14,    15,    16,    17,     0,     0,    18,    19,   112,
     113,    20,   -61,    21,   143,   144,   145,   146,   147,   148,
       0,     0,     0,     0,     0,     0,    18,    19,     0,     0,
      20,   -59,    21,   164,     0,     6,     0,     0,   155,     8,
       0,     9,    10,    11,    12,    13,     0,     0,    14,    15,
      16,    17,     5,     0,     6,     0,     0,   155,     8,     0,
       9,    10,    11,    12,    13,    49,    50,    14,    15,    16,
      17,     0,     0,    18,    19,     0,     0,    20,   165,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      52,     0,    18,    19,     0,     0,    20,     0,    21,     0,
       0,   112,   113,    53,    54,    55,   143,   144,   145,   146,
     147,   148,     0,     0,     0,     0,     0,     0,   149
};

static const yytype_int16 yycheck[] =
{
       4,    12,    96,   180,     9,     1,     1,    82,    51,    52,
     245,     1,     1,     1,    21,   250,    44,    41,    42,    21,
      48,     1,    97,     3,     4,    44,     0,    27,    28,    48,
      35,    36,    37,    38,    39,    40,    43,    47,    43,    41,
      42,    27,    28,   118,    44,    46,    47,    27,    28,   124,
      55,    47,    47,     3,    43,    43,   131,    47,    63,    43,
      40,    41,    42,    43,     1,    43,     3,     4,    45,   246,
      47,   114,   115,   116,     3,     4,     5,    82,    27,    28,
     257,    14,    15,    27,    28,    47,    91,    47,   182,    43,
      27,    28,    97,     3,     3,    44,   171,    43,    27,    28,
      44,    48,    47,    40,    41,    42,    43,    29,    30,    31,
       3,    40,    41,    42,    43,   119,     3,    43,   112,   113,
       5,     5,    43,   128,     3,   136,   131,    44,    44,   140,
     141,   142,   137,   208,    39,    38,     6,    48,   143,   144,
     145,   146,   147,   148,    44,    47,    43,   222,     6,     0,
       1,    47,     3,    21,    44,     6,     7,   162,     9,    10,
      11,    12,    13,    44,    44,    16,    17,    18,    19,    11,
      47,    20,    44,   177,     3,    44,     3,    44,   189,   190,
      45,   192,    48,    44,     8,     3,     4,     5,    43,     8,
      41,    42,    44,    47,    45,     4,    47,    47,    46,   210,
      44,   205,    49,     3,     4,   209,   217,    47,    49,    27,
      28,    21,    22,    23,    24,    25,    26,   221,    44,    47,
     203,   232,    40,    41,    42,    43,   230,    27,    28,    25,
     241,    41,    42,    43,   226,   117,    93,   242,   247,   191,
      40,    41,    42,    43,   242,   256,   181,     1,   252,     3,
      -1,    -1,     6,     7,   258,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     1,    -1,     3,    -1,
      -1,     6,     7,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    41,    42,    27,
      28,    45,    46,    47,    32,    33,    34,    35,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,
      45,    46,    47,     1,    -1,     3,    -1,    -1,     6,     7,
      -1,     9,    10,    11,    12,    13,    -1,    -1,    16,    17,
      18,    19,     1,    -1,     3,    -1,    -1,     6,     7,    -1,
       9,    10,    11,    12,    13,     3,     4,    16,    17,    18,
      19,    -1,    -1,    41,    42,    -1,    -1,    45,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    41,    42,    -1,    -1,    45,    -1,    47,    -1,
      -1,    27,    28,    41,    42,    43,    32,    33,    34,    35,
      36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    44
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    54,    55,     0,    56,     1,     3,     6,     7,     9,
      10,    11,    12,    13,    16,    17,    18,    19,    41,    42,
      45,    47,    57,    58,    59,    60,    65,    67,    68,    70,
      77,    80,    81,    82,    47,    21,    22,    23,    24,    25,
      26,    41,    42,    43,     3,    78,    79,     1,    43,     3,
       4,    27,    28,    41,    42,    43,    94,    95,    96,    43,
       1,    83,    83,    43,    47,    47,    43,    43,     3,     3,
      85,    47,    65,     1,    47,     1,    47,     1,    47,    47,
       5,    40,    43,    94,    94,    94,    94,    94,    94,    69,
      94,    21,    43,    48,     1,     3,    40,    43,    88,    89,
      90,    91,    92,    93,    94,    41,    42,    96,    96,     3,
       3,    94,    27,    28,    29,    30,    31,    86,    43,    87,
      94,     5,     5,    66,    43,    88,    94,    44,    48,     5,
      40,    43,    94,    61,     3,    79,    44,    21,    91,    88,
      44,    39,    38,    32,    33,    34,    35,    36,    37,    44,
      95,    95,    96,    96,    96,     6,    77,     1,    88,    67,
      44,    44,    48,    48,     1,    46,    67,    88,    44,    47,
      94,    43,    88,     6,    62,    63,    64,    83,    94,    44,
      83,    83,    83,    94,    94,    94,    94,    94,    94,    47,
      44,    44,    11,    71,    47,    94,    20,    46,    44,    88,
      44,     3,    44,    48,    67,    86,    90,    91,    83,    83,
      87,    83,    45,    44,     3,    44,    64,     8,    67,    88,
      67,    83,    43,    14,    15,    72,    73,    74,    47,    44,
      83,     8,    47,    67,    88,     4,    49,    46,    74,    47,
      67,    84,    83,    44,    49,    76,    83,    93,    94,    47,
      75,    66,    86,    84,    66,    67,    44,    83,    86,    67
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    55,    54,    56,    56,    57,    57,    57,    58,
      59,    61,    60,    62,    62,    63,    63,    64,    65,    65,
      66,    66,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      69,    69,    69,    71,    70,    72,    73,    73,    75,    74,
      76,    74,    77,    78,    78,    79,    79,    79,    79,    79,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    81,
      81,    81,    81,    82,    82,    83,    84,    85,    86,    87,
      88,    89,    89,    90,    90,    91,    91,    92,    92,    92,
      92,    93,    93,    93,    93,    93,    93,    94,    94,    94,
      95,    95,    95,    95,    96,    96,    96,    96,    96,    96,
      96,    96,    96
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     1,     1,     1,     2,
       2,     0,     6,     0,     1,     1,     3,     2,     4,     5,
       2,     0,     1,     7,    12,     6,     9,     2,     8,     7,
       2,    10,    15,     2,     2,     2,     2,     2,     2,     5,
       5,     7,     8,     1,     1,     1,     2,     2,     2,     2,
       0,     1,     3,     0,     8,     1,     1,     2,     0,     5,
       0,     4,     2,     1,     3,     1,     3,     3,     5,     6,
       3,     3,     5,     6,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     1,     2,     0,     0,     0,     0,     0,
       1,     4,     1,     4,     1,     2,     1,     3,     1,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     3,     1,     3,     2,     2,     2,     2,     2,
       2,     1,     1
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
  case 2: /* $@1: %empty  */
#line 112 "parser.y"
      {
          /* push the top-level program block before any externals reduce.
             each external (function def/proto/top-level stmt) appends
             itself here. */
          ast_push_block(ast_node("Program"));
      }
#line 1819 "parser.tab.c"
    break;

  case 3: /* program: $@1 external_list  */
#line 119 "parser.y"
      {
          /* pop the program block and hand it to ast_root. */
          ast_root = ast_pop_block();
      }
#line 1828 "parser.tab.c"
    break;

  case 9: /* function_def: function_header compound_stmt  */
#line 138 "parser.y"
      {
          /* compound_stmt already appended its Block to the FunctionDef.
             pop FunctionDef and hand it up to the enclosing scope. */
          ASTNode *fn = ast_pop_block();
          ast_append_stmt(fn);
      }
#line 1839 "parser.tab.c"
    break;

  case 10: /* function_proto: function_header SEMI  */
#line 148 "parser.y"
      {
          /* prototype: we pushed a FunctionDef-shaped node but since
             there's no body, re-label it and append. */
          ASTNode *fn = ast_pop_block();
          strncpy(fn->label, "FunctionProto", sizeof(fn->label) - 1);
          ast_append_stmt(fn);
      }
#line 1851 "parser.tab.c"
    break;

  case 11: /* $@2: %empty  */
#line 159 "parser.y"
      {
          /* unified prefix for function_def and function_proto so bison
             doesn't hit reduce/reduce conflicts choosing between them.
             we push a FunctionDef block here; function_proto relabels
             it post-hoc if no body follows. */
          ASTNode *fn = ast_node("FunctionDef");
          ast_add_child(fn, ast_leaf("return_type:int"));
          ast_add_child(fn, ast_leaf("name:%s", (yyvsp[-1].sval)));
          ast_push_block(fn);
          free((yyvsp[-1].sval));
      }
#line 1867 "parser.tab.c"
    break;

  case 17: /* param_decl: TYPE ID  */
#line 185 "parser.y"
      {
          if (sym_declare((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column)) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "redefinition of '%s'", (yyvsp[0].sval));
              SymEntry *prev = sym_find((yyvsp[0].sval));
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", (yyvsp[0].sval));
          }
          /* append a Param node to the enclosing FunctionDef/Proto block */
          ASTNode *p = ast_node("Param");
          ast_add_child(p, ast_leaf("type:int"));
          ast_add_child(p, ast_leaf("name:%s", (yyvsp[0].sval)));
          ast_append_stmt(p);
          free((yyvsp[0].sval));
      }
#line 1888 "parser.tab.c"
    break;

  case 18: /* compound_stmt: LBRACE push_block stmt_list RBRACE  */
#line 205 "parser.y"
      {
          /* pop the completed Block and append to the enclosing
             accumulator. FunctionDef's closing action overrides this
             by using the returned node directly — but we do the
             append unconditionally here. FunctionDef's logic below
             has been adjusted to accommodate. */
          ASTNode *blk = ast_pop_block();
          ast_append_stmt(blk);
      }
#line 1902 "parser.tab.c"
    break;

  case 19: /* compound_stmt: LBRACE push_block stmt_list error RBRACE  */
#line 215 "parser.y"
      {
          log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
              "expected '}' at end of compound statement");
          ASTNode *blk = ast_pop_block();
          ast_append_stmt(blk);
          yyerrok;
      }
#line 1914 "parser.tab.c"
    break;

  case 23: /* statement: IF LPAREN bexpr RPAREN M push_capture statement  */
#line 232 "parser.y"
      {
          backpatch((yyvsp[-4].b)->truelist, (yyvsp[-2].ival));
          backpatch((yyvsp[-4].b)->falselist, nextinstr());

          ASTNode *cap = ast_pop_block();
          ASTNode *then_body = (cap->nchildren == 1) ? cap->children[0] : cap;
          ASTNode *ifn = ast_node("If");
          ast_add_child(ifn, ast_node1("Cond", (yyvsp[-4].b)->node));
          ast_add_child(ifn, ast_node1("Then", then_body));
          ast_append_stmt(ifn);
      }
#line 1930 "parser.tab.c"
    break;

  case 24: /* statement: IF LPAREN bexpr RPAREN M push_capture statement ELSE N M push_capture statement  */
#line 244 "parser.y"
      {
          backpatch((yyvsp[-9].b)->truelist, (yyvsp[-7].ival));
          backpatch((yyvsp[-9].b)->falselist, (yyvsp[-2].ival));
          backpatch((yyvsp[-3].list), nextinstr());

          ASTNode *cap_else = ast_pop_block();
          ASTNode *cap_then = ast_pop_block();
          ASTNode *then_body = (cap_then->nchildren == 1) ? cap_then->children[0] : cap_then;
          ASTNode *else_body = (cap_else->nchildren == 1) ? cap_else->children[0] : cap_else;
          ASTNode *ifn = ast_node("If");
          ast_add_child(ifn, ast_node1("Cond", (yyvsp[-9].b)->node));
          ast_add_child(ifn, ast_node1("Then", then_body));
          ast_add_child(ifn, ast_node1("Else", else_body));
          ast_append_stmt(ifn);
      }
#line 1950 "parser.tab.c"
    break;

  case 25: /* statement: IF LPAREN error RPAREN M statement  */
#line 260 "parser.y"
      {
          log_syntax_error((yylsp[-3]).first_line, (yylsp[-3]).first_column,
              "expected expression in 'if' condition");
          yyerrok;
      }
#line 1960 "parser.tab.c"
    break;

  case 26: /* statement: IF LPAREN error RPAREN M statement ELSE M statement  */
#line 266 "parser.y"
      {
          log_syntax_error((yylsp[-6]).first_line, (yylsp[-6]).first_column,
              "expected expression in 'if' condition");
          yyerrok;
      }
#line 1970 "parser.tab.c"
    break;

  case 27: /* statement: IF error  */
#line 272 "parser.y"
      {
          log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column + 2,
              "expected '(' after 'if'");
          yyerrok;
      }
#line 1980 "parser.tab.c"
    break;

  case 28: /* statement: WHILE M LPAREN bexpr RPAREN loop_enter M statement  */
#line 278 "parser.y"
      {
          backpatch((yyvsp[-4].b)->truelist, (yyvsp[-1].ival));
          emit_goto((yyvsp[-6].ival));
          int end_lbl = nextinstr();
          backpatch((yyvsp[-4].b)->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop((yyvsp[-6].ival));

          ASTNode *cap = ast_pop_block();
          ASTNode *body = (cap->nchildren == 1) ? cap->children[0] : cap;
          ASTNode *wh = ast_node("While");
          ast_add_child(wh, ast_node1("Cond", (yyvsp[-4].b)->node));
          ast_add_child(wh, ast_node1("Body", body));
          ast_append_stmt(wh);
      }
#line 2000 "parser.tab.c"
    break;

  case 29: /* statement: WHILE M LPAREN error RPAREN M statement  */
#line 294 "parser.y"
      {
          log_syntax_error((yylsp[-3]).first_line, (yylsp[-3]).first_column,
              "expected expression in 'while' condition");
          yyerrok;
      }
#line 2010 "parser.tab.c"
    break;

  case 30: /* statement: WHILE error  */
#line 300 "parser.y"
      {
          log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column + 5,
              "expected '(' after 'while'");
          yyerrok;
      }
#line 2020 "parser.tab.c"
    break;

  case 31: /* statement: DO M loop_enter statement WHILE M LPAREN bexpr RPAREN SEMI  */
#line 306 "parser.y"
      {
          backpatch((yyvsp[-2].b)->truelist, (yyvsp[-8].ival));
          int end_lbl = nextinstr();
          backpatch((yyvsp[-2].b)->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop((yyvsp[-4].ival));

          ASTNode *cap = ast_pop_block();
          ASTNode *body = (cap->nchildren == 1) ? cap->children[0] : cap;
          ASTNode *dw = ast_node("DoWhile");
          ast_add_child(dw, ast_node1("Body", body));
          ast_add_child(dw, ast_node1("Cond", (yyvsp[-2].b)->node));
          ast_append_stmt(dw);
      }
#line 2039 "parser.tab.c"
    break;

  case 32: /* statement: FOR LPAREN push_capture decl_stmt SEMI M bexpr SEMI M rel_bool N RPAREN M push_capture statement  */
#line 321 "parser.y"
      {
          /* for loop stub — control flow not fully wired, but we still
             build an AST node so the tree reflects the source. */
          ASTNode *cap_body = ast_pop_block();
          ASTNode *cap_init = ast_pop_block();
          ASTNode *init = (cap_init->nchildren == 1) ? cap_init->children[0] : cap_init;
          ASTNode *body = (cap_body->nchildren == 1) ? cap_body->children[0] : cap_body;
          ASTNode *fr = ast_node("For");
          ast_add_child(fr, ast_node1("Init", init));
          ast_add_child(fr, ast_node1("Cond", (yyvsp[-8].b)->node));
          ast_add_child(fr, ast_node1("Update", (yyvsp[-5].b)->node));
          ast_add_child(fr, ast_node1("Body", body));
          ast_append_stmt(fr);
      }
#line 2058 "parser.tab.c"
    break;

  case 37: /* non_if_stmt: BREAK SEMI  */
#line 343 "parser.y"
      {
          /* defer the jump; the enclosing loop/switch backpatches it on exit */
          int g = emit_goto(-1);
          breaklist_add(g);
          ast_append_stmt(ast_node("Break"));
      }
#line 2069 "parser.tab.c"
    break;

  case 38: /* non_if_stmt: CONTINUE SEMI  */
#line 350 "parser.y"
      {
          /* same deal, but target is the loop header (not the end) */
          int g = emit_goto(-1);
          continuelist_add(g);
          ast_append_stmt(ast_node("Continue"));
      }
#line 2080 "parser.tab.c"
    break;

  case 39: /* non_if_stmt: ID LPAREN arg_list RPAREN SEMI  */
#line 357 "parser.y"
      {
          /* bare call as a statement. the arg_list rule (below) doesn't
             carry AST nodes up because bison %union has no slot for a
             list-of-AST, so we approximate with a FuncCall node tagged
             by name; arg details aren't shown. */
          ASTNode *c = ast_node("CallStmt");
          ast_add_child(c, ast_leaf("callee:%s", (yyvsp[-4].sval)));
          ast_append_stmt(c);
          free((yyvsp[-4].sval));
      }
#line 2095 "parser.tab.c"
    break;

  case 40: /* non_if_stmt: PRINTF LPAREN STRING RPAREN SEMI  */
#line 368 "parser.y"
      {
          emit_text("param %s", (yyvsp[-2].sval));
          emit_text("call printf, 1");
          ASTNode *p = ast_node("Printf");
          ast_add_child(p, ast_leaf("format:%s", (yyvsp[-2].sval)));
          ast_append_stmt(p);
          free((yyvsp[-2].sval));
      }
#line 2108 "parser.tab.c"
    break;

  case 41: /* non_if_stmt: PRINTF LPAREN STRING COMMA aexpr RPAREN SEMI  */
#line 377 "parser.y"
      {
          emit_text("param %s", (yyvsp[-2].a)->place);
          emit_text("param %s", (yyvsp[-4].sval));
          emit_text("call printf, 2");
          ASTNode *p = ast_node("Printf");
          ast_add_child(p, ast_leaf("format:%s", (yyvsp[-4].sval)));
          ast_add_child(p, ast_node1("Arg", (yyvsp[-2].a)->node));
          ast_append_stmt(p);
          free_aattr((yyvsp[-2].a));
          free((yyvsp[-4].sval));
      }
#line 2124 "parser.tab.c"
    break;

  case 42: /* non_if_stmt: SCANF LPAREN STRING COMMA AMPER ID RPAREN SEMI  */
#line 389 "parser.y"
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
          ASTNode *s = ast_node("Scanf");
          ast_add_child(s, ast_leaf("format:%s", (yyvsp[-5].sval)));
          ast_add_child(s, ast_leaf("target:&%s", (yyvsp[-2].sval)));
          ast_append_stmt(s);
          free((yyvsp[-2].sval)); free((yyvsp[-5].sval));
      }
#line 2145 "parser.tab.c"
    break;

  case 45: /* non_if_stmt: SEMI  */
#line 408 "parser.y"
      {
          /* empty statement — represented explicitly so it shows up
             in the tree and isn't silently swallowed. */
          ast_append_stmt(ast_node("EmptyStmt"));
      }
#line 2155 "parser.tab.c"
    break;

  case 46: /* non_if_stmt: decl_stmt error  */
#line 414 "parser.y"
      {
          log_syntax_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "expected ';' after declaration");
          yyerrok;
      }
#line 2165 "parser.tab.c"
    break;

  case 47: /* non_if_stmt: assign_stmt error  */
#line 420 "parser.y"
      {
          log_syntax_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "expected ';' after assignment");
          yyerrok;
      }
#line 2175 "parser.tab.c"
    break;

  case 48: /* non_if_stmt: inc_stmt error  */
#line 426 "parser.y"
      {
          log_syntax_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "expected ';' after expression");
          yyerrok;
      }
#line 2185 "parser.tab.c"
    break;

  case 49: /* non_if_stmt: error SEMI  */
#line 432 "parser.y"
      {
          if ((yylsp[-1]).first_line != last_syntax_error_line) {
              log_syntax_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "expected expression before ';' token");
              last_syntax_error_line = (yylsp[-1]).first_line;
          }
          yyerrok;
      }
#line 2198 "parser.tab.c"
    break;

  case 53: /* $@3: %empty  */
#line 460 "parser.y"
      {
          /* hold the switch expression in a stable temp so the
             dispatch can reference it repeatedly. copy into a temp
             in case the aexpr was a variable that gets modified
             inside the switch body. */
          char *tsw = new_temp();
          emit_text("%s = %s", tsw, (yyvsp[-1].a)->place);
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

          /* start a Switch AST node. push it on the switch-stack so
             case_entries can find it; its first child is the
             discriminant expression. */
          ASTNode *sw = ast_node("Switch");
          ast_add_child(sw, ast_node1("Discriminant", (yyvsp[-1].a)->node));
          ast_push_switch(sw);
          free_aattr((yyvsp[-1].a));
      }
#line 2231 "parser.tab.c"
    break;

  case 54: /* switch_stmt: SWITCH LPAREN aexpr RPAREN $@3 LBRACE case_list RBRACE  */
#line 489 "parser.y"
      {
          /* nothing left except patching break. the cases already
             emitted their bodies and dispatch entries inline.
             the dispatch "goto L_end" falls through to here. */
          int end_lbl = nextinstr();
          breaklist_pop(end_lbl);

          ASTNode *sw = ast_pop_switch();
          ast_append_stmt(sw);
      }
#line 2246 "parser.tab.c"
    break;

  case 58: /* $@4: %empty  */
#line 517 "parser.y"
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
          int skip_body = emit_goto(-1);
          patch_one(ifi, nextinstr());
          /* stash skip_body in a small stack so case_list can chain */
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp < 128) __sw_skip_stack[__sw_skip_sp++] = skip_body;

          /* start collecting this case's body into a fresh Block.
             remember the case label so the closing action can build
             a Case node with (value, body). */
          ast_push_case_label(ast_leaf("case:%s", (yyvsp[-1].sval)));
          ast_push_block(ast_node("Block"));
          free((yyvsp[-1].sval));
      }
#line 2275 "parser.tab.c"
    break;

  case 59: /* case_entry: CASE NUMBER COLON $@4 stmt_list  */
#line 542 "parser.y"
      {
          /* at end of this case's statements, patch the skip-body
             goto to point HERE (where the next dispatch starts). */
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp > 0) {
              int skip = __sw_skip_stack[--__sw_skip_sp];
              patch_one(skip, nextinstr());
          }

          ASTNode *body = ast_pop_block();
          /* unwrap: if the case body is just { ... } it'll be a Block
             containing a single Block — flatten to one. */
          if (body->nchildren == 1 &&
              strcmp(body->children[0]->label, "Block") == 0)
              body = body->children[0];
          ASTNode *lbl  = ast_pop_case_label();
          ASTNode *c    = ast_node("Case");
          ast_add_child(c, lbl);
          ast_add_child(c, body);
          /* attach to the enclosing Switch (top of switch stack) */
          ASTNode *sw = ast_top_switch();
          if (sw) ast_add_child(sw, c);
      }
#line 2304 "parser.tab.c"
    break;

  case 60: /* $@5: %empty  */
#line 567 "parser.y"
      {
          extern int __sw_has_default;
          extern int __sw_default_entry;
          __sw_has_default = 1;
          int skip_body = emit_goto(-1);
          __sw_default_entry = nextinstr();
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp < 128) __sw_skip_stack[__sw_skip_sp++] = skip_body;

          ast_push_case_label(ast_node("default"));
          ast_push_block(ast_node("Block"));
      }
#line 2322 "parser.tab.c"
    break;

  case 61: /* case_entry: DEFAULT COLON $@5 stmt_list  */
#line 581 "parser.y"
      {
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp > 0) {
              int skip = __sw_skip_stack[--__sw_skip_sp];
              patch_one(skip, nextinstr());
          }

          ASTNode *body = ast_pop_block();
          if (body->nchildren == 1 &&
              strcmp(body->children[0]->label, "Block") == 0)
              body = body->children[0];
          ASTNode *lbl  = ast_pop_case_label();
          ASTNode *c    = ast_node("Default");
          ast_add_child(c, lbl);
          ast_add_child(c, body);
          ASTNode *sw = ast_top_switch();
          if (sw) ast_add_child(sw, c);
      }
#line 2346 "parser.tab.c"
    break;

  case 65: /* decl_item: ID  */
#line 614 "parser.y"
      {
          if (sym_declare((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column)) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "redefinition of '%s'", (yyvsp[0].sval));
              SymEntry *prev = sym_find((yyvsp[0].sval));
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", (yyvsp[0].sval));
          }
          ASTNode *d = ast_node("VarDecl");
          ast_add_child(d, ast_leaf("type:int"));
          ast_add_child(d, ast_leaf("name:%s", (yyvsp[0].sval)));
          ast_append_stmt(d);
          free((yyvsp[0].sval));
      }
#line 2366 "parser.tab.c"
    break;

  case 66: /* decl_item: ID ASSIGN aexpr  */
#line 630 "parser.y"
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
          ASTNode *d = ast_node("VarDeclInit");
          ast_add_child(d, ast_leaf("type:int"));
          ast_add_child(d, ast_leaf("name:%s", (yyvsp[-2].sval)));
          ast_add_child(d, ast_node1("Init", (yyvsp[0].a)->node));
          ast_append_stmt(d);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2390 "parser.tab.c"
    break;

  case 67: /* decl_item: ID ASSIGN STRING  */
#line 650 "parser.y"
      {
          if (sym_declare((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column)) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "redefinition of '%s'", (yyvsp[-2].sval));
          }
          log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "incompatible types when initializing type 'int' using type 'char *'");
          free((yyvsp[-2].sval)); free((yyvsp[0].sval));
      }
#line 2404 "parser.tab.c"
    break;

  case 68: /* decl_item: ID ASSIGN LPAREN bexpr RPAREN  */
#line 660 "parser.y"
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
          ASTNode *d = ast_node("VarDeclInit");
          ast_add_child(d, ast_leaf("type:int"));
          ast_add_child(d, ast_leaf("name:%s", (yyvsp[-4].sval)));
          ast_add_child(d, ast_node1("Init", (yyvsp[-1].b)->node ? (yyvsp[-1].b)->node : ast_node("BoolExpr")));
          ast_append_stmt(d);
          free((yyvsp[-4].sval));
      }
#line 2428 "parser.tab.c"
    break;

  case 69: /* decl_item: ID ASSIGN NOT LPAREN bexpr RPAREN  */
#line 680 "parser.y"
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
          ASTNode *d = ast_node("VarDeclInit");
          ast_add_child(d, ast_leaf("type:int"));
          ast_add_child(d, ast_leaf("name:%s", (yyvsp[-5].sval)));
          ast_add_child(d, ast_node1("Init",
              ast_node1("Not", (yyvsp[-1].b)->node ? (yyvsp[-1].b)->node : ast_node("BoolExpr"))));
          ast_append_stmt(d);
          free((yyvsp[-5].sval));
      }
#line 2456 "parser.tab.c"
    break;

  case 70: /* assign_stmt: ID ASSIGN aexpr  */
#line 707 "parser.y"
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
          ASTNode *a = ast_node("Assign");
          ast_add_child(a, ast_leaf("id:%s", (yyvsp[-2].sval)));
          ast_add_child(a, (yyvsp[0].a)->node);
          ast_append_stmt(a);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2480 "parser.tab.c"
    break;

  case 71: /* assign_stmt: ID ASSIGN STRING  */
#line 727 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          }
          log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
              "incompatible types when assigning to type 'int' from type 'char *'");
          free((yyvsp[-2].sval)); free((yyvsp[0].sval));
      }
#line 2494 "parser.tab.c"
    break;

  case 72: /* assign_stmt: ID ASSIGN LPAREN bexpr RPAREN  */
#line 737 "parser.y"
      {
          if (!sym_lookup((yyvsp[-4].sval))) {
              log_sem_error((yylsp[-4]).first_line, (yylsp[-4]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-4].sval));
          } else {
              sym_mark_init((yyvsp[-4].sval));
          }
          emit_bool_assignment((yyvsp[-4].sval), (yyvsp[-1].b));
          ASTNode *a = ast_node("Assign");
          ast_add_child(a, ast_leaf("id:%s", (yyvsp[-4].sval)));
          ast_add_child(a, (yyvsp[-1].b)->node ? (yyvsp[-1].b)->node : ast_node("BoolExpr"));
          ast_append_stmt(a);
          free((yyvsp[-4].sval));
      }
#line 2513 "parser.tab.c"
    break;

  case 73: /* assign_stmt: ID ASSIGN NOT LPAREN bexpr RPAREN  */
#line 752 "parser.y"
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
          ASTNode *a = ast_node("Assign");
          ast_add_child(a, ast_leaf("id:%s", (yyvsp[-5].sval)));
          ast_add_child(a, ast_node1("Not", (yyvsp[-1].b)->node ? (yyvsp[-1].b)->node : ast_node("BoolExpr")));
          ast_append_stmt(a);
          free((yyvsp[-5].sval));
      }
#line 2535 "parser.tab.c"
    break;

  case 74: /* assign_stmt: ID ADD_ASSIGN aexpr  */
#line 770 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "+", (yyvsp[0].a));
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:+="));
          ast_add_child(a, ast_leaf("id:%s", (yyvsp[-2].sval)));
          ast_add_child(a, (yyvsp[0].a)->node);
          ast_append_stmt(a);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2556 "parser.tab.c"
    break;

  case 75: /* assign_stmt: ID SUB_ASSIGN aexpr  */
#line 787 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "-", (yyvsp[0].a));
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:-="));
          ast_add_child(a, ast_leaf("id:%s", (yyvsp[-2].sval)));
          ast_add_child(a, (yyvsp[0].a)->node);
          ast_append_stmt(a);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2577 "parser.tab.c"
    break;

  case 76: /* assign_stmt: ID MUL_ASSIGN aexpr  */
#line 804 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "*", (yyvsp[0].a));
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:*="));
          ast_add_child(a, ast_leaf("id:%s", (yyvsp[-2].sval)));
          ast_add_child(a, (yyvsp[0].a)->node);
          ast_append_stmt(a);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2598 "parser.tab.c"
    break;

  case 77: /* assign_stmt: ID DIV_ASSIGN aexpr  */
#line 821 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "/", (yyvsp[0].a));
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:/="));
          ast_add_child(a, ast_leaf("id:%s", (yyvsp[-2].sval)));
          ast_add_child(a, (yyvsp[0].a)->node);
          ast_append_stmt(a);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2619 "parser.tab.c"
    break;

  case 78: /* assign_stmt: ID MOD_ASSIGN aexpr  */
#line 838 "parser.y"
      {
          if (!sym_lookup((yyvsp[-2].sval))) {
              log_sem_error((yylsp[-2]).first_line, (yylsp[-2]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-2].sval));
          } else {
              sym_mark_used((yyvsp[-2].sval), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
              sym_mark_init((yyvsp[-2].sval));
          }
          emit_compound_assign((yyvsp[-2].sval), "%", (yyvsp[0].a));
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:%%="));
          ast_add_child(a, ast_leaf("id:%s", (yyvsp[-2].sval)));
          ast_add_child(a, (yyvsp[0].a)->node);
          ast_append_stmt(a);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2640 "parser.tab.c"
    break;

  case 79: /* inc_stmt: ID INC  */
#line 858 "parser.y"
      {
          if (!sym_lookup((yyvsp[-1].sval))) {
              log_sem_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-1].sval));
          } else {
              sym_mark_used((yyvsp[-1].sval), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
              sym_mark_init((yyvsp[-1].sval));
          }
          emit_incdec((yyvsp[-1].sval), 1);
          ASTNode *n = ast_node("PostInc");
          ast_add_child(n, ast_leaf("id:%s", (yyvsp[-1].sval)));
          ast_append_stmt(n);
          free((yyvsp[-1].sval));
      }
#line 2659 "parser.tab.c"
    break;

  case 80: /* inc_stmt: ID DEC  */
#line 873 "parser.y"
      {
          if (!sym_lookup((yyvsp[-1].sval))) {
              log_sem_error((yylsp[-1]).first_line, (yylsp[-1]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[-1].sval));
          } else {
              sym_mark_used((yyvsp[-1].sval), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
              sym_mark_init((yyvsp[-1].sval));
          }
          emit_incdec((yyvsp[-1].sval), 0);
          ASTNode *n = ast_node("PostDec");
          ast_add_child(n, ast_leaf("id:%s", (yyvsp[-1].sval)));
          ast_append_stmt(n);
          free((yyvsp[-1].sval));
      }
#line 2678 "parser.tab.c"
    break;

  case 81: /* inc_stmt: INC ID  */
#line 888 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
              sym_mark_init((yyvsp[0].sval));
          }
          emit_incdec((yyvsp[0].sval), 1);
          ASTNode *n = ast_node("PreInc");
          ast_add_child(n, ast_leaf("id:%s", (yyvsp[0].sval)));
          ast_append_stmt(n);
          free((yyvsp[0].sval));
      }
#line 2697 "parser.tab.c"
    break;

  case 82: /* inc_stmt: DEC ID  */
#line 903 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
              sym_mark_init((yyvsp[0].sval));
          }
          emit_incdec((yyvsp[0].sval), 0);
          ASTNode *n = ast_node("PreDec");
          ast_add_child(n, ast_leaf("id:%s", (yyvsp[0].sval)));
          ast_append_stmt(n);
          free((yyvsp[0].sval));
      }
#line 2716 "parser.tab.c"
    break;

  case 83: /* return_stmt: RETURN  */
#line 921 "parser.y"
      {
          emit_text("return");
          ast_append_stmt(ast_node("Return"));
      }
#line 2725 "parser.tab.c"
    break;

  case 84: /* return_stmt: RETURN aexpr  */
#line 926 "parser.y"
      {
          emit_text("return %s", (yyvsp[0].a)->place);
          ASTNode *r = ast_node("Return");
          ast_add_child(r, (yyvsp[0].a)->node);
          ast_append_stmt(r);
          free_aattr((yyvsp[0].a));
      }
#line 2737 "parser.tab.c"
    break;

  case 85: /* M: %empty  */
#line 935 "parser.y"
                { (yyval.ival) = nextinstr(); }
#line 2743 "parser.tab.c"
    break;

  case 86: /* N: %empty  */
#line 936 "parser.y"
                { (yyval.list) = makelist(emit_goto(-1)); }
#line 2749 "parser.tab.c"
    break;

  case 87: /* push_block: %empty  */
#line 942 "parser.y"
                  { ast_push_block(ast_node("Block")); }
#line 2755 "parser.tab.c"
    break;

  case 88: /* push_capture: %empty  */
#line 950 "parser.y"
                  { ast_push_block(ast_node("_capture")); }
#line 2761 "parser.tab.c"
    break;

  case 89: /* loop_enter: %empty  */
#line 958 "parser.y"
      {
          breaklist_push();
          continuelist_push();
          ast_push_block(ast_node("_capture"));
      }
#line 2771 "parser.tab.c"
    break;

  case 90: /* bexpr: bor_expr  */
#line 965 "parser.y"
                     { (yyval.b) = (yyvsp[0].b); }
#line 2777 "parser.tab.c"
    break;

  case 91: /* bor_expr: bor_expr OR M band_expr  */
#line 969 "parser.y"
      {
          backpatch((yyvsp[-3].b)->falselist, (yyvsp[-1].ival));
          (yyval.b) = make_battr(merge_list((yyvsp[-3].b)->truelist, (yyvsp[0].b)->truelist), (yyvsp[0].b)->falselist,
                          ast_node2("||", (yyvsp[-3].b)->node, (yyvsp[0].b)->node));
      }
#line 2787 "parser.tab.c"
    break;

  case 92: /* bor_expr: band_expr  */
#line 974 "parser.y"
                { (yyval.b) = (yyvsp[0].b); }
#line 2793 "parser.tab.c"
    break;

  case 93: /* band_expr: band_expr AND M bnot_expr  */
#line 979 "parser.y"
      {
          backpatch((yyvsp[-3].b)->truelist, (yyvsp[-1].ival));
          (yyval.b) = make_battr((yyvsp[0].b)->truelist, merge_list((yyvsp[-3].b)->falselist, (yyvsp[0].b)->falselist),
                          ast_node2("&&", (yyvsp[-3].b)->node, (yyvsp[0].b)->node));
      }
#line 2803 "parser.tab.c"
    break;

  case 94: /* band_expr: bnot_expr  */
#line 984 "parser.y"
                { (yyval.b) = (yyvsp[0].b); }
#line 2809 "parser.tab.c"
    break;

  case 95: /* bnot_expr: NOT bnot_expr  */
#line 988 "parser.y"
                    { (yyval.b) = make_battr((yyvsp[0].b)->falselist, (yyvsp[0].b)->truelist,
                                      ast_node1("!", (yyvsp[0].b)->node)); }
#line 2816 "parser.tab.c"
    break;

  case 96: /* bnot_expr: bprimary  */
#line 990 "parser.y"
                    { (yyval.b) = (yyvsp[0].b); }
#line 2822 "parser.tab.c"
    break;

  case 97: /* bprimary: LPAREN bexpr RPAREN  */
#line 1001 "parser.y"
      { (yyval.b) = (yyvsp[-1].b); }
#line 2828 "parser.tab.c"
    break;

  case 98: /* bprimary: rel_bool  */
#line 1003 "parser.y"
      { (yyval.b) = (yyvsp[0].b); }
#line 2834 "parser.tab.c"
    break;

  case 99: /* bprimary: ID ASSIGN aexpr  */
#line 1005 "parser.y"
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
          /* an `=` used as a truth value: build an Assign-as-expression */
          (yyval.b)->node = ast_node2("Assign", ast_leaf("id:%s", (yyvsp[-2].sval)), (yyvsp[0].a)->node);
          free((yyvsp[-2].sval)); free_aattr((yyvsp[0].a));
      }
#line 2858 "parser.tab.c"
    break;

  case 100: /* bprimary: aexpr  */
#line 1025 "parser.y"
      {
          /* arithmetic-as-truthy: treat nonzero as true */
          (yyval.b) = emit_truthy((yyvsp[0].a)->place);
          (yyval.b)->node = ast_node1("Truthy", (yyvsp[0].a)->node);
          free_aattr((yyvsp[0].a));
      }
#line 2869 "parser.tab.c"
    break;

  case 101: /* rel_bool: aexpr LT aexpr  */
#line 1035 "parser.y"
      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, "<",  (yyvsp[0].a)->place);
        (yyval.b)->node = ast_node2("<",  (yyvsp[-2].a)->node, (yyvsp[0].a)->node);
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2877 "parser.tab.c"
    break;

  case 102: /* rel_bool: aexpr GT aexpr  */
#line 1039 "parser.y"
      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, ">",  (yyvsp[0].a)->place);
        (yyval.b)->node = ast_node2(">",  (yyvsp[-2].a)->node, (yyvsp[0].a)->node);
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2885 "parser.tab.c"
    break;

  case 103: /* rel_bool: aexpr LE aexpr  */
#line 1043 "parser.y"
      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, "<=", (yyvsp[0].a)->place);
        (yyval.b)->node = ast_node2("<=", (yyvsp[-2].a)->node, (yyvsp[0].a)->node);
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2893 "parser.tab.c"
    break;

  case 104: /* rel_bool: aexpr GE aexpr  */
#line 1047 "parser.y"
      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, ">=", (yyvsp[0].a)->place);
        (yyval.b)->node = ast_node2(">=", (yyvsp[-2].a)->node, (yyvsp[0].a)->node);
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2901 "parser.tab.c"
    break;

  case 105: /* rel_bool: aexpr EQ aexpr  */
#line 1051 "parser.y"
      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, "==", (yyvsp[0].a)->place);
        (yyval.b)->node = ast_node2("==", (yyvsp[-2].a)->node, (yyvsp[0].a)->node);
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2909 "parser.tab.c"
    break;

  case 106: /* rel_bool: aexpr NE aexpr  */
#line 1055 "parser.y"
      { (yyval.b) = emit_relop((yyvsp[-2].a)->place, "!=", (yyvsp[0].a)->place);
        (yyval.b)->node = ast_node2("!=", (yyvsp[-2].a)->node, (yyvsp[0].a)->node);
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2917 "parser.tab.c"
    break;

  case 107: /* aexpr: aexpr PLUS term  */
#line 1062 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s + %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place);
        (yyval.a) = make_aattr(t, ast_node2("+", (yyvsp[-2].a)->node, (yyvsp[0].a)->node));
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2925 "parser.tab.c"
    break;

  case 108: /* aexpr: aexpr MINUS term  */
#line 1066 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s - %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place);
        (yyval.a) = make_aattr(t, ast_node2("-", (yyvsp[-2].a)->node, (yyvsp[0].a)->node));
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2933 "parser.tab.c"
    break;

  case 109: /* aexpr: term  */
#line 1070 "parser.y"
      { (yyval.a) = (yyvsp[0].a); }
#line 2939 "parser.tab.c"
    break;

  case 110: /* term: term MUL factor  */
#line 1075 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s * %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place);
        (yyval.a) = make_aattr(t, ast_node2("*", (yyvsp[-2].a)->node, (yyvsp[0].a)->node));
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2947 "parser.tab.c"
    break;

  case 111: /* term: term DIV factor  */
#line 1079 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s / %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place);
        (yyval.a) = make_aattr(t, ast_node2("/", (yyvsp[-2].a)->node, (yyvsp[0].a)->node));
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2955 "parser.tab.c"
    break;

  case 112: /* term: term MOD factor  */
#line 1083 "parser.y"
      { char *t = new_temp(); emit_text("%s = %s %% %s", t, (yyvsp[-2].a)->place, (yyvsp[0].a)->place);
        (yyval.a) = make_aattr(t, ast_node2("%", (yyvsp[-2].a)->node, (yyvsp[0].a)->node));
        free_aattr((yyvsp[-2].a)); free_aattr((yyvsp[0].a)); }
#line 2963 "parser.tab.c"
    break;

  case 113: /* term: factor  */
#line 1087 "parser.y"
      { (yyval.a) = (yyvsp[0].a); }
#line 2969 "parser.tab.c"
    break;

  case 114: /* factor: LPAREN aexpr RPAREN  */
#line 1091 "parser.y"
                          { (yyval.a) = (yyvsp[-1].a); }
#line 2975 "parser.tab.c"
    break;

  case 115: /* factor: INC ID  */
#line 1093 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
              sym_mark_init((yyvsp[0].sval));
          }
          /* pre-increment: bump first, then use the new value */
          emit_incdec((yyvsp[0].sval), 1);
          (yyval.a) = make_aattr(xstrdup((yyvsp[0].sval)),
                 ast_node1("PreInc", ast_leaf("id:%s", (yyvsp[0].sval))));
          free((yyvsp[0].sval));
      }
#line 2994 "parser.tab.c"
    break;

  case 116: /* factor: DEC ID  */
#line 1108 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
              sym_mark_init((yyvsp[0].sval));
          }
          /* pre-decrement: bump first, then use the new value */
          emit_incdec((yyvsp[0].sval), 0);
          (yyval.a) = make_aattr(xstrdup((yyvsp[0].sval)),
                 ast_node1("PreDec", ast_leaf("id:%s", (yyvsp[0].sval))));
          free((yyvsp[0].sval));
      }
#line 3013 "parser.tab.c"
    break;

  case 117: /* factor: ID INC  */
#line 1123 "parser.y"
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
          (yyval.a) = make_aattr(old,
                 ast_node1("PostInc", ast_leaf("id:%s", (yyvsp[-1].sval))));
          free((yyvsp[-1].sval));
      }
#line 3034 "parser.tab.c"
    break;

  case 118: /* factor: ID DEC  */
#line 1140 "parser.y"
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
          (yyval.a) = make_aattr(old,
                 ast_node1("PostDec", ast_leaf("id:%s", (yyvsp[-1].sval))));
          free((yyvsp[-1].sval));
      }
#line 3055 "parser.tab.c"
    break;

  case 119: /* factor: MINUS factor  */
#line 1157 "parser.y"
      {
          char *t = new_temp();
          emit_text("%s = 0 - %s", t, (yyvsp[0].a)->place);
          (yyval.a) = make_aattr(t, ast_node1("Neg", (yyvsp[0].a)->node));
          free_aattr((yyvsp[0].a));
      }
#line 3066 "parser.tab.c"
    break;

  case 120: /* factor: PLUS factor  */
#line 1164 "parser.y"
      {
          /* unary plus is a no-op, just pass through */
          (yyval.a) = (yyvsp[0].a);
      }
#line 3075 "parser.tab.c"
    break;

  case 121: /* factor: ID  */
#line 1169 "parser.y"
      {
          if (!sym_lookup((yyvsp[0].sval))) {
              log_sem_error((yylsp[0]).first_line, (yylsp[0]).first_column,
                  "use of undeclared identifier '%s'", (yyvsp[0].sval));
          } else {
              sym_mark_used((yyvsp[0].sval), (yylsp[0]).first_line, (yylsp[0]).first_column);
          }
          (yyval.a) = make_aattr((yyvsp[0].sval), ast_leaf("id:%s", (yyvsp[0].sval)));
      }
#line 3089 "parser.tab.c"
    break;

  case 122: /* factor: NUMBER  */
#line 1179 "parser.y"
      { (yyval.a) = make_aattr((yyvsp[0].sval), ast_leaf("num:%s", (yyvsp[0].sval))); }
#line 3095 "parser.tab.c"
    break;


#line 3099 "parser.tab.c"

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

#line 1182 "parser.y"


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
