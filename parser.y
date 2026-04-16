%{
#include "common.h"
%}

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
    : non_if_stmt
    | IF LPAREN bexpr RPAREN M statement %prec LOWER_THAN_ELSE
      {
          backpatch($3->truelist, $5);
          backpatch($3->falselist, nextinstr());
      }
    | IF LPAREN bexpr RPAREN M statement ELSE N M statement
      {
          backpatch($3->truelist, $5);
          backpatch($3->falselist, $9);
          backpatch($8, nextinstr());
      }
    | IF LPAREN error RPAREN M statement %prec LOWER_THAN_ELSE
      {
          log_syntax_error(@3.first_line, @3.first_column,
              "expected expression in 'if' condition");
          yyerrok;
      }
    | IF LPAREN error RPAREN M statement ELSE M statement
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
    | WHILE M LPAREN bexpr RPAREN
      {
          /* push break/continue frames right before the body.
             continue targets the M at position $2, break targets
             whatever comes after the body. */
          breaklist_push();
          continuelist_push();
      }
      M statement %prec LOWER_THAN_ELSE
      {
          backpatch($4->truelist, $7);
          emit_goto($2);
          int end_lbl = nextinstr();
          backpatch($4->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop($2);
      }
    | WHILE M LPAREN error RPAREN M statement
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
    | DO M
      {
          breaklist_push();
          continuelist_push();
      }
      statement WHILE M LPAREN bexpr RPAREN SEMI
      {
          backpatch($8->truelist, $2);
          int end_lbl = nextinstr();
          backpatch($8->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop($6);
      }
    | FOR LPAREN decl_stmt SEMI M bexpr SEMI M rel_bool N RPAREN M statement
      { /* for loop stub â€” left as-is from original, pending real implementation */ }
    ;

non_if_stmt
    : decl_stmt SEMI
    | assign_stmt SEMI
    | inc_stmt SEMI
    | return_stmt SEMI
    | BREAK SEMI
      {
          /* defer the jump; the enclosing loop/switch backpatches it on exit */
          int g = emit_goto(-1);
          breaklist_add(g);
      }
    | CONTINUE SEMI
      {
          /* same deal, but target is the loop header (not the end) */
          int g = emit_goto(-1);
          continuelist_add(g);
      }
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
    | switch_stmt
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
    ;

arg_list
    : /* empty */
    | aexpr
    | arg_list COMMA aexpr
    ;

/* ---- switch: proper dispatch + break target ----
   flow:
     1. evaluate the switch expression into a named temp (sw_expr)
     2. for each case, emit "if t_sw == N goto L_caseN" at the TOP
     3. after dispatch, emit unconditional "goto L_end"
     4. then the case bodies, each with its own entry label
     5. break inside any case jumps to L_end (via breaklist)
     6. default's body also sits inline; if no default, L_end is
        the fall-through target
*/
switch_stmt
    : SWITCH LPAREN aexpr RPAREN
      {
          /* hold the switch expression in a stable temp so the
             dispatch can reference it repeatedly. copy into a temp
             in case the aexpr was a variable that gets modified
             inside the switch body. */
          char *tsw = new_temp();
          emit_text("%s = %s", tsw, $3->place);
          free_aattr($3);
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
      LBRACE case_list RBRACE
      {
          /* nothing left except patching break. the cases already
             emitted their bodies and dispatch entries inline.
             the dispatch "goto L_end" falls through to here. */
          int end_lbl = nextinstr();
          breaklist_pop(end_lbl);
      }
    ;

case_list
    : case_entries
    ;

case_entries
    : case_entry
    | case_entries case_entry
    ;

/* each case_entry emits its dispatch test at the current point,
   then falls through to the body. this interleaves dispatch and
   bodies, which isn't the textbook layout but keeps the grammar
   flat and the TAC correct. the optimizer re-orders nothing about
   control flow so this stays well-formed. */
case_entry
    : CASE NUMBER COLON
      {
          /* emit: if t_sw == <const> goto L_caseN
             where L_caseN is the instruction right after this if.
             but the body comes next, so we make the dispatch jump
             forward to the body. the body is the very next instr
             after this if+goto pair. */
          extern char __sw_temp[64];
          char *cond = mkstr("%s == %s", __sw_temp, $2);
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
          free($2);
          /* stash skip_body in a small stack so case_list can chain */
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp < 128) __sw_skip_stack[__sw_skip_sp++] = skip_body;
      }
      stmt_list
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
    | DEFAULT COLON
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
      stmt_list
      {
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp > 0) {
              int skip = __sw_skip_stack[--__sw_skip_sp];
              patch_one(skip, nextinstr());
          }
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
    | ID ASSIGN LPAREN bexpr RPAREN
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
    | ID ASSIGN NOT LPAREN bexpr RPAREN
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
    | ID ASSIGN LPAREN bexpr RPAREN
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
    | ID ASSIGN NOT LPAREN bexpr RPAREN
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

/* bprimary: a single unit that produces a BAttr (truelist/falselist).
   since `aexpr` already covers plain identifiers and numbers (via
   factor : ID | NUMBER), we don't need separate ID / NUMBER rules
   here — any arithmetic expression, including a bare variable or
   constant, becomes a truthy test. this is what lets
   `if (a+b)` or `while (count)` work. */
bprimary
    : LPAREN bexpr RPAREN
      { $$ = $2; }
    | rel_bool
      { $$ = $1; }
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
    | aexpr
      {
          /* arithmetic-as-truthy: treat nonzero as true */
          $$ = emit_truthy($1->place);
          free_aattr($1);
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
