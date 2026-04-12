%code requires {
#include "common.h"
}

%define parse.error verbose
%locations

%union {
    char *sval;
    AAttr *a;
    BAttr *b;
    IntList *list;
    int ival;
}

%token <sval> ID NUMBER STRING
%token TYPE IF ELSE RETURN
%token FOR WHILE DO SWITCH CASE DEFAULT BREAK CONTINUE
%token PRINTF SCANF AMPER
%token ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token PLUS MINUS MUL DIV MOD
%token LT GT LE GE EQ NE
%token AND OR NOT
%token INC DEC
%token LPAREN RPAREN LBRACE RBRACE SEMI COMMA COLON
%token INVALID

%type <a> aexpr term factor
%type <b> bexpr bor_expr band_expr bnot_expr bprimary rel_bool
%type <ival> M
%type <list> N

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left OR
%left AND
%right NOT
%left PLUS MINUS
%left MUL DIV MOD
%right UMINUS

%%

program
    : external_list
    ;

external_list
    : external_list external
    | /* empty */
    ;

external
    : function_def
    | function_proto
    | statement
    ;

function_def
    : TYPE ID LPAREN opt_param_list RPAREN compound_stmt
      { free($2); }
    ;

function_proto
    : TYPE ID LPAREN opt_param_list RPAREN SEMI
      { free($2); }
    ;

opt_param_list
    : /* empty */
    | param_list
    ;

param_list
    : param_decl
    | param_list COMMA param_decl
    ;

param_decl
    : TYPE ID
      {
          if (sym_declare($2, @2.first_line, @2.first_column)) {
              log_sem_error(@2.first_line, @2.first_column,
                  "redefinition of '%s'", $2);
              SymEntry *prev = sym_find($2);
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", $2);
          }
          free($2);
      }
    ;

compound_stmt
    : LBRACE stmt_list RBRACE
    | LBRACE stmt_list error RBRACE
      {
          log_syntax_error(@3.first_line, @3.first_column,
              "expected '}' at end of compound statement");
          yyerrok;
      }
    ;

stmt_list
    : stmt_list statement
    | /* empty */
    ;

statement
    : matched_stmt
    | unmatched_stmt
    ;

non_if_stmt
    : decl_stmt SEMI
    | assign_stmt SEMI
    | inc_stmt SEMI
    | return_stmt SEMI
    | BREAK SEMI
      { emit_text("goto END_OF_LOOP_OR_SWITCH /* patch manually if needed */"); }
    | CONTINUE SEMI
      { emit_text("goto START_OF_LOOP /* patch manually if needed */"); }
    | ID LPAREN arg_list RPAREN SEMI
      { free($1); }
    | PRINTF LPAREN STRING RPAREN SEMI
      {
          emit_text("param %s", $3);
          emit_text("call printf, 1");
          free($3);
      }
    | PRINTF LPAREN STRING COMMA aexpr RPAREN SEMI
      {
          emit_text("param %s", $5->place);
          emit_text("param %s", $3);
          emit_text("call printf, 2");
          free_aattr($5);
          free($3);
      }
    | SCANF LPAREN STRING COMMA AMPER ID RPAREN SEMI
      {
          if (!sym_lookup($6)) {
              log_sem_error(@6.first_line, @6.first_column,
                  "use of undeclared identifier '%s'", $6);
          } else {
              sym_mark_init($6);
          }
          emit_text("param &%s", $6);
          emit_text("param %s", $3);
          emit_text("call scanf, 2");
          free($6); free($3);
      }
    | SWITCH LPAREN aexpr RPAREN LBRACE case_list RBRACE
      { free_aattr($3); }
    | compound_stmt
    | SEMI
    | decl_stmt error
      {
          log_syntax_error(@2.first_line, @2.first_column,
              "expected ';' after declaration");
          yyerrok;
      }
    | assign_stmt error
      {
          log_syntax_error(@2.first_line, @2.first_column,
              "expected ';' after assignment");
          yyerrok;
      }
    | inc_stmt error
      {
          log_syntax_error(@2.first_line, @2.first_column,
              "expected ';' after expression");
          yyerrok;
      }
    | error SEMI
      {
          if (@1.first_line != last_syntax_error_line) {
              log_syntax_error(@1.first_line, @1.first_column,
                  "expected expression before ';' token");
              last_syntax_error_line = @1.first_line;
          }
          yyerrok;
      }
    | error RBRACE
      {
          if (@1.first_line != last_syntax_error_line) {
              log_syntax_error(@1.first_line, @1.first_column,
                  "expected expression before '}' token");
              last_syntax_error_line = @1.first_line;
          }
          yyerrok;
      }
    ;

arg_list
    : /* empty */
    | aexpr
    | arg_list COMMA aexpr
    ;

case_list
    : case_stmt
    | case_list case_stmt
    ;

case_stmt
    : CASE NUMBER COLON stmt_list
      { free($2); }
    | DEFAULT COLON stmt_list
    ;

matched_stmt
    : non_if_stmt
    | IF LPAREN bexpr RPAREN M matched_stmt N ELSE M matched_stmt
      {
          backpatch($3->truelist, $5);
          backpatch($3->falselist, $9);
          backpatch($7, nextinstr());
      }
    | IF LPAREN error RPAREN M matched_stmt N ELSE M matched_stmt
      {
          log_syntax_error(@3.first_line, @3.first_column,
              "expected expression in 'if' condition");
          yyerrok;
      }
    | IF error
      {
          log_syntax_error(@1.first_line, @1.first_column + 2,
              "expected '(' after 'if'");
          yyerrok;
      }
    | WHILE M LPAREN bexpr RPAREN M matched_stmt
      {
          backpatch($4->truelist, $6);
          emit_goto($2);
          backpatch($4->falselist, nextinstr());
      }
    | WHILE M LPAREN error RPAREN M matched_stmt
      {
          log_syntax_error(@4.first_line, @4.first_column,
              "expected expression in 'while' condition");
          yyerrok;
      }
    | WHILE error
      {
          log_syntax_error(@1.first_line, @1.first_column + 5,
              "expected '(' after 'while'");
          yyerrok;
      }
    | DO M statement WHILE M LPAREN bexpr RPAREN SEMI
      {
          backpatch($7->truelist, $2);
          backpatch($7->falselist, nextinstr());
      }
    | FOR LPAREN decl_stmt SEMI M bexpr SEMI M rel_bool N RPAREN M matched_stmt
      { /* for loop stub */ }
    ;

unmatched_stmt
    : IF LPAREN bexpr RPAREN M statement
      {
          backpatch($3->truelist, $5);
          backpatch($3->falselist, nextinstr());
      }
    | IF LPAREN bexpr RPAREN M matched_stmt N ELSE M unmatched_stmt
      {
          backpatch($3->truelist, $5);
          backpatch($3->falselist, $9);
          backpatch($7, nextinstr());
      }
    | WHILE M LPAREN bexpr RPAREN M unmatched_stmt
      {
          backpatch($4->truelist, $6);
          emit_goto($2);
          backpatch($4->falselist, nextinstr());
      }
    ;

decl_stmt
    : TYPE decl_items
    ;

decl_items
    : decl_item
    | decl_items COMMA decl_item
    ;

decl_item
    : ID
      {
          if (sym_declare($1, @1.first_line, @1.first_column)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "redefinition of '%s'", $1);
              SymEntry *prev = sym_find($1);
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", $1);
          }
          free($1);
      }
    | ID ASSIGN aexpr
      {
          if (sym_declare($1, @1.first_line, @1.first_column)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "redefinition of '%s'", $1);
              SymEntry *prev = sym_find($1);
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", $1);
          } else {
              sym_mark_init($1);
          }
          emit_text("%s = %s", $1, $3->place);
          free($1); free_aattr($3);
      }
    | ID ASSIGN STRING
      {
          if (sym_declare($1, @1.first_line, @1.first_column)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "redefinition of '%s'", $1);
          }
          log_sem_error(@3.first_line, @3.first_column,
              "incompatible types when initializing type 'int' using type 'char *'");
          free($1); free($3);
      }
    | ID ASSIGN LPAREN rel_bool RPAREN
      {
          if (sym_declare($1, @1.first_line, @1.first_column)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "redefinition of '%s'", $1);
              SymEntry *prev = sym_find($1);
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", $1);
          } else {
              sym_mark_init($1);
          }
          emit_bool_assignment($1, $4);
          free($1);
      }
    | ID ASSIGN NOT LPAREN rel_bool RPAREN
      {
          if (sym_declare($1, @1.first_line, @1.first_column)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "redefinition of '%s'", $1);
              SymEntry *prev = sym_find($1);
              if (prev)
                  emit_diagnostic("<stdin>", prev->decl_line, prev->decl_col,
                      "note", "previous definition of '%s' was here", $1);
          } else {
              sym_mark_init($1);
          }
          IntList *tmp = $5->truelist;
          $5->truelist = $5->falselist;
          $5->falselist = tmp;
          emit_bool_assignment($1, $5);
          free($1);
      }
    ;

assign_stmt
    : ID ASSIGN aexpr
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_init($1);
              if (strcmp($1, $3->place) == 0)
                  emit_diagnostic("<stdin>", @1.first_line, @1.first_column,
                      "warning",
                      "explicitly assigning value of variable '%s' to itself [-Wself-assign]",
                      $1);
          }
          emit_text("%s = %s", $1, $3->place);
          free($1); free_aattr($3);
      }
    | ID ASSIGN STRING
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          }
          log_sem_error(@3.first_line, @3.first_column,
              "incompatible types when assigning to type 'int' from type 'char *'");
          free($1); free($3);
      }
    | ID ASSIGN LPAREN rel_bool RPAREN
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_init($1);
          }
          emit_bool_assignment($1, $4);
          free($1);
      }
    | ID ASSIGN NOT LPAREN rel_bool RPAREN
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_init($1);
          }
          IntList *tmp = $5->truelist;
          $5->truelist = $5->falselist;
          $5->falselist = tmp;
          emit_bool_assignment($1, $5);
          free($1);
      }
    | ID ADD_ASSIGN aexpr
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          emit_compound_assign($1, "+", $3);
          free($1); free_aattr($3);
      }
    | ID SUB_ASSIGN aexpr
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          emit_compound_assign($1, "-", $3);
          free($1); free_aattr($3);
      }
    | ID MUL_ASSIGN aexpr
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          emit_compound_assign($1, "*", $3);
          free($1); free_aattr($3);
      }
    | ID DIV_ASSIGN aexpr
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          emit_compound_assign($1, "/", $3);
          free($1); free_aattr($3);
      }
    | ID MOD_ASSIGN aexpr
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          emit_compound_assign($1, "%", $3);
          free($1); free_aattr($3);
      }
    ;

inc_stmt
    : ID INC
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          emit_incdec($1, 1); free($1);
      }
    | ID DEC
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          emit_incdec($1, 0); free($1);
      }
    | INC ID
      {
          if (!sym_lookup($2)) {
              log_sem_error(@2.first_line, @2.first_column,
                  "use of undeclared identifier '%s'", $2);
          } else {
              sym_mark_used($2, @2.first_line, @2.first_column);
              sym_mark_init($2);
          }
          emit_incdec($2, 1); free($2);
      }
    | DEC ID
      {
          if (!sym_lookup($2)) {
              log_sem_error(@2.first_line, @2.first_column,
                  "use of undeclared identifier '%s'", $2);
          } else {
              sym_mark_used($2, @2.first_line, @2.first_column);
              sym_mark_init($2);
          }
          emit_incdec($2, 0); free($2);
      }
    ;

return_stmt
    : RETURN             { emit_text("return"); }
    | RETURN aexpr       { emit_text("return %s", $2->place); free_aattr($2); }
    ;

M : /* empty */ { $$ = nextinstr(); } ;
N : /* empty */ { $$ = makelist(emit_goto(-1)); } ;

bexpr     : bor_expr { $$ = $1; } ;

bor_expr
    : bor_expr OR M band_expr
      {
          backpatch($1->falselist, $3);
          $$ = make_battr(merge_list($1->truelist, $4->truelist), $4->falselist);
      }
    | band_expr { $$ = $1; }
    ;

band_expr
    : band_expr AND M bnot_expr
      {
          backpatch($1->truelist, $3);
          $$ = make_battr($4->truelist, merge_list($1->falselist, $4->falselist));
      }
    | bnot_expr { $$ = $1; }
    ;

bnot_expr
    : NOT bnot_expr { $$ = make_battr($2->falselist, $2->truelist); }
    | bprimary      { $$ = $1; }
    ;

bprimary
    : LPAREN bexpr RPAREN { $$ = $2; }
    | rel_bool             { $$ = $1; }
    | ID
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
          }
          $$ = emit_truthy($1); free($1);
      }
    | NUMBER
      { $$ = emit_truthy($1); free($1); }
    | ID ASSIGN aexpr
      {
          /* someone wrote = instead of == inside a condition */
          emit_diagnostic("<stdin>", @2.first_line, @2.first_column,
              "warning",
              "suggest parentheses around assignment used as truth value [-Wparentheses]");
          emit_diagnostic("<stdin>", @2.first_line, @2.first_column,
              "note", "use '==' to check for equality");
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_init($1);
          }
          emit_text("%s = %s", $1, $3->place);
          $$ = emit_truthy($1);
          free($1); free_aattr($3);
      }
    ;

rel_bool
    : aexpr LT aexpr  { $$ = emit_relop($1->place, "<",  $3->place); free_aattr($1); free_aattr($3); }
    | aexpr GT aexpr  { $$ = emit_relop($1->place, ">",  $3->place); free_aattr($1); free_aattr($3); }
    | aexpr LE aexpr  { $$ = emit_relop($1->place, "<=", $3->place); free_aattr($1); free_aattr($3); }
    | aexpr GE aexpr  { $$ = emit_relop($1->place, ">=", $3->place); free_aattr($1); free_aattr($3); }
    | aexpr EQ aexpr  { $$ = emit_relop($1->place, "==", $3->place); free_aattr($1); free_aattr($3); }
    | aexpr NE aexpr  { $$ = emit_relop($1->place, "!=", $3->place); free_aattr($1); free_aattr($3); }
    ;

aexpr
    : aexpr PLUS term
      { char *t = new_temp(); emit_text("%s = %s + %s", t, $1->place, $3->place); $$ = make_aattr(t); free_aattr($1); free_aattr($3); }
    | aexpr MINUS term
      { char *t = new_temp(); emit_text("%s = %s - %s", t, $1->place, $3->place); $$ = make_aattr(t); free_aattr($1); free_aattr($3); }
    | term
      { $$ = $1; }
    ;

term
    : term MUL factor
      { char *t = new_temp(); emit_text("%s = %s * %s", t, $1->place, $3->place); $$ = make_aattr(t); free_aattr($1); free_aattr($3); }
    | term DIV factor
      { char *t = new_temp(); emit_text("%s = %s / %s", t, $1->place, $3->place); $$ = make_aattr(t); free_aattr($1); free_aattr($3); }
    | term MOD factor
      { char *t = new_temp(); emit_text("%s = %s %% %s", t, $1->place, $3->place); $$ = make_aattr(t); free_aattr($1); free_aattr($3); }
    | factor
      { $$ = $1; }
    ;

factor
    : LPAREN aexpr RPAREN { $$ = $2; }
    | INC ID
      {
          if (!sym_lookup($2)) {
              log_sem_error(@2.first_line, @2.first_column,
                  "use of undeclared identifier '%s'", $2);
          } else {
              sym_mark_used($2, @2.first_line, @2.first_column);
              sym_mark_init($2);
          }
          /* pre-increment: bump first, then use the new value */
          emit_incdec($2, 1); $$ = make_aattr($2);
      }
    | DEC ID
      {
          if (!sym_lookup($2)) {
              log_sem_error(@2.first_line, @2.first_column,
                  "use of undeclared identifier '%s'", $2);
          } else {
              sym_mark_used($2, @2.first_line, @2.first_column);
              sym_mark_init($2);
          }
          /* pre-decrement: bump first, then use the new value */
          emit_incdec($2, 0); $$ = make_aattr($2);
      }
    | ID INC
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          /* post-increment: use the old value, then bump */
          char *old = new_temp();
          emit_text("%s = %s", old, $1);
          emit_incdec($1, 1);
          $$ = make_aattr(old);
          free($1);
      }
    | ID DEC
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
              sym_mark_init($1);
          }
          /* post-decrement: use the old value, then bump */
          char *old = new_temp();
          emit_text("%s = %s", old, $1);
          emit_incdec($1, 0);
          $$ = make_aattr(old);
          free($1);
      }
    | MINUS factor %prec UMINUS
      {
          char *t = new_temp();
          emit_text("%s = 0 - %s", t, $2->place);
          $$ = make_aattr(t);
          free_aattr($2);
      }
    | PLUS factor %prec UMINUS
      {
          /* unary plus is a no-op, just pass through */
          $$ = $2;
      }
    | NOT factor
      {
          /* logical NOT in arithmetic context: !x becomes (x == 0 ? 1 : 0) */
          char *t = new_temp();
          int false_lbl = nextinstr();
          emit_text("%s = 1", t);
          char *cond = mkstr("%s != 0", $2->place);
          int i = emit_if(cond);
          free(cond);
          int skip = emit_goto(-1);
          int true_lbl = nextinstr();
          emit_text("%s = 0", t);
          int end_lbl = nextinstr();
          patch_one(i, true_lbl);
          patch_one(skip, end_lbl);
          $$ = make_aattr(t);
          free_aattr($2);
      }
    | ID
      {
          if (!sym_lookup($1)) {
              log_sem_error(@1.first_line, @1.first_column,
                  "use of undeclared identifier '%s'", $1);
          } else {
              sym_mark_used($1, @1.first_line, @1.first_column);
          }
          $$ = make_aattr($1);
      }
    | NUMBER
      { $$ = make_aattr($1); }
    ;
%%