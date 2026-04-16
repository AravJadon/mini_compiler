%{
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

%token <sval> ID NUMBER STRING TYPE
%token IF ELSE RETURN
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
    : {
          /* push the top-level program block before any externals reduce.
             each external (function def/proto/top-level stmt) appends
             itself here. */
          ast_push_block(ast_node("Program"));
      }
      external_list
      {
          /* pop the program block and hand it to ast_root. */
          ast_root = ast_pop_block();
      }
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
<<<<<<< HEAD
    : function_header compound_stmt
      {
          /* compound_stmt already appended its Block to the FunctionDef.
             pop FunctionDef and hand it up to the enclosing scope. */
          ASTNode *fn = ast_pop_block();
          ast_append_stmt(fn);
=======
    : TYPE ID LPAREN opt_param_list RPAREN compound_stmt
      {
          if (strcmp($1, "int") != 0) {
              log_sem_error(@1.first_line, @1.first_column,
                  "Unsupported data type '%s'. Toy-C only supports 'int'", $1);
          }
          free($1); free($2);
>>>>>>> 82293e5207a16fc5a1fd12c60419dc7d7bc64b74
      }
    ;

function_proto
<<<<<<< HEAD
    : function_header SEMI
      {
          /* prototype: we pushed a FunctionDef-shaped node but since
             there's no body, re-label it and append. */
          ASTNode *fn = ast_pop_block();
          strncpy(fn->label, "FunctionProto", sizeof(fn->label) - 1);
          ast_append_stmt(fn);
      }
    ;

function_header
    : TYPE ID LPAREN
      {
          /* unified prefix for function_def and function_proto so bison
             doesn't hit reduce/reduce conflicts choosing between them.
             we push a FunctionDef block here; function_proto relabels
             it post-hoc if no body follows. */
          ASTNode *fn = ast_node("FunctionDef");
          ast_add_child(fn, ast_leaf("return_type:int"));
          ast_add_child(fn, ast_leaf("name:%s", $2));
          ast_push_block(fn);
          free($2);
      }
      opt_param_list RPAREN
=======
    : TYPE ID LPAREN opt_param_list RPAREN SEMI
      {
          if (strcmp($1, "int") != 0) {
              log_sem_error(@1.first_line, @1.first_column,
                  "Unsupported data type '%s'. Toy-C only supports 'int'", $1);
          }
          free($1); free($2);
      }
>>>>>>> 82293e5207a16fc5a1fd12c60419dc7d7bc64b74
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
<<<<<<< HEAD
          /* append a Param node to the enclosing FunctionDef/Proto block */
          ASTNode *p = ast_node("Param");
          ast_add_child(p, ast_leaf("type:int"));
          ast_add_child(p, ast_leaf("name:%s", $2));
          ast_append_stmt(p);
          free($2);
=======
          if (strcmp($1, "int") != 0) {
              log_sem_error(@1.first_line, @1.first_column,
                  "Unsupported data type '%s'. Toy-C only supports 'int'", $1);
          }
          free($1); free($2);
>>>>>>> 82293e5207a16fc5a1fd12c60419dc7d7bc64b74
      }
    ;

compound_stmt
    : LBRACE push_block stmt_list RBRACE
      {
          /* pop the completed Block and append to the enclosing
             accumulator. FunctionDef's closing action overrides this
             by using the returned node directly — but we do the
             append unconditionally here. FunctionDef's logic below
             has been adjusted to accommodate. */
          ASTNode *blk = ast_pop_block();
          ast_append_stmt(blk);
      }
    | LBRACE push_block stmt_list error RBRACE
      {
          log_syntax_error(@4.first_line, @4.first_column,
              "expected '}' at end of compound statement");
          ASTNode *blk = ast_pop_block();
          ast_append_stmt(blk);
          yyerrok;
      }
    ;

stmt_list
    : stmt_list statement
    | /* empty */
    ;

statement
    : non_if_stmt
    | IF LPAREN bexpr RPAREN M push_capture statement %prec LOWER_THAN_ELSE
      {
          backpatch($3->truelist, $5);
          backpatch($3->falselist, nextinstr());

          ASTNode *cap = ast_pop_block();
          ASTNode *then_body = (cap->nchildren == 1) ? cap->children[0] : cap;
          ASTNode *ifn = ast_node("If");
          ast_add_child(ifn, ast_node1("Cond", $3->node));
          ast_add_child(ifn, ast_node1("Then", then_body));
          ast_append_stmt(ifn);
      }
    | IF LPAREN bexpr RPAREN M push_capture statement ELSE N M push_capture statement
      {
          backpatch($3->truelist, $5);
          backpatch($3->falselist, $10);
          backpatch($9, nextinstr());

          ASTNode *cap_else = ast_pop_block();
          ASTNode *cap_then = ast_pop_block();
          ASTNode *then_body = (cap_then->nchildren == 1) ? cap_then->children[0] : cap_then;
          ASTNode *else_body = (cap_else->nchildren == 1) ? cap_else->children[0] : cap_else;
          ASTNode *ifn = ast_node("If");
          ast_add_child(ifn, ast_node1("Cond", $3->node));
          ast_add_child(ifn, ast_node1("Then", then_body));
          ast_add_child(ifn, ast_node1("Else", else_body));
          ast_append_stmt(ifn);
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
    | WHILE M LPAREN bexpr RPAREN loop_enter M statement %prec LOWER_THAN_ELSE
      {
          backpatch($4->truelist, $7);
          emit_goto($2);
          int end_lbl = nextinstr();
          backpatch($4->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop($2);

          ASTNode *cap = ast_pop_block();
          ASTNode *body = (cap->nchildren == 1) ? cap->children[0] : cap;
          ASTNode *wh = ast_node("While");
          ast_add_child(wh, ast_node1("Cond", $4->node));
          ast_add_child(wh, ast_node1("Body", body));
          ast_append_stmt(wh);
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
    | DO M loop_enter statement WHILE M LPAREN bexpr RPAREN SEMI
      {
          backpatch($8->truelist, $2);
          int end_lbl = nextinstr();
          backpatch($8->falselist, end_lbl);
          breaklist_pop(end_lbl);
          continuelist_pop($6);

          ASTNode *cap = ast_pop_block();
          ASTNode *body = (cap->nchildren == 1) ? cap->children[0] : cap;
          ASTNode *dw = ast_node("DoWhile");
          ast_add_child(dw, ast_node1("Body", body));
          ast_add_child(dw, ast_node1("Cond", $8->node));
          ast_append_stmt(dw);
      }
    | FOR LPAREN push_capture decl_stmt SEMI M bexpr SEMI M rel_bool N RPAREN M push_capture statement
      {
          /* for loop stub — control flow not fully wired, but we still
             build an AST node so the tree reflects the source. */
          ASTNode *cap_body = ast_pop_block();
          ASTNode *cap_init = ast_pop_block();
          ASTNode *init = (cap_init->nchildren == 1) ? cap_init->children[0] : cap_init;
          ASTNode *body = (cap_body->nchildren == 1) ? cap_body->children[0] : cap_body;
          ASTNode *fr = ast_node("For");
          ast_add_child(fr, ast_node1("Init", init));
          ast_add_child(fr, ast_node1("Cond", $7->node));
          ast_add_child(fr, ast_node1("Update", $10->node));
          ast_add_child(fr, ast_node1("Body", body));
          ast_append_stmt(fr);
      }
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
          ast_append_stmt(ast_node("Break"));
      }
    | CONTINUE SEMI
      {
          /* same deal, but target is the loop header (not the end) */
          int g = emit_goto(-1);
          continuelist_add(g);
          ast_append_stmt(ast_node("Continue"));
      }
    | ID LPAREN arg_list RPAREN SEMI
      {
          /* bare call as a statement. the arg_list rule (below) doesn't
             carry AST nodes up because bison %union has no slot for a
             list-of-AST, so we approximate with a FuncCall node tagged
             by name; arg details aren't shown. */
          ASTNode *c = ast_node("CallStmt");
          ast_add_child(c, ast_leaf("callee:%s", $1));
          ast_append_stmt(c);
          free($1);
      }
    | PRINTF LPAREN STRING RPAREN SEMI
      {
          emit_text("param %s", $3);
          emit_text("call printf, 1");
          ASTNode *p = ast_node("Printf");
          ast_add_child(p, ast_leaf("format:%s", $3));
          ast_append_stmt(p);
          free($3);
      }
    | PRINTF LPAREN STRING COMMA aexpr RPAREN SEMI
      {
          emit_text("param %s", $5->place);
          emit_text("param %s", $3);
          emit_text("call printf, 2");
          ASTNode *p = ast_node("Printf");
          ast_add_child(p, ast_leaf("format:%s", $3));
          ast_add_child(p, ast_node1("Arg", $5->node));
          ast_append_stmt(p);
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
          ASTNode *s = ast_node("Scanf");
          ast_add_child(s, ast_leaf("format:%s", $3));
          ast_add_child(s, ast_leaf("target:&%s", $6));
          ast_append_stmt(s);
          free($6); free($3);
      }
    | switch_stmt
    | compound_stmt
    | SEMI
      {
          /* empty statement — represented explicitly so it shows up
             in the tree and isn't silently swallowed. */
          ast_append_stmt(ast_node("EmptyStmt"));
      }
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
          ast_add_child(sw, ast_node1("Discriminant", $3->node));
          ast_push_switch(sw);
          free_aattr($3);
      }
      LBRACE case_list RBRACE
      {
          /* nothing left except patching break. the cases already
             emitted their bodies and dispatch entries inline.
             the dispatch "goto L_end" falls through to here. */
          int end_lbl = nextinstr();
          breaklist_pop(end_lbl);

          ASTNode *sw = ast_pop_switch();
          ast_append_stmt(sw);
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
          int skip_body = emit_goto(-1);
          patch_one(ifi, nextinstr());
          /* stash skip_body in a small stack so case_list can chain */
          extern int __sw_skip_stack[128];
          extern int __sw_skip_sp;
          if (__sw_skip_sp < 128) __sw_skip_stack[__sw_skip_sp++] = skip_body;

          /* start collecting this case's body into a fresh Block.
             remember the case label so the closing action can build
             a Case node with (value, body). */
          ast_push_case_label(ast_leaf("case:%s", $2));
          ast_push_block(ast_node("Block"));
          free($2);
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
    | DEFAULT COLON
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
      stmt_list
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
    ;


decl_stmt
    : TYPE decl_items
      {
          if (strcmp($1, "int") != 0) {
              log_sem_error(@1.first_line, @1.first_column,
                  "Unsupported data type '%s'. Toy-C only supports 'int'", $1);
          }
          free($1);
      }
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
          ASTNode *d = ast_node("VarDecl");
          ast_add_child(d, ast_leaf("type:int"));
          ast_add_child(d, ast_leaf("name:%s", $1));
          ast_append_stmt(d);
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
          ASTNode *d = ast_node("VarDeclInit");
          ast_add_child(d, ast_leaf("type:int"));
          ast_add_child(d, ast_leaf("name:%s", $1));
          ast_add_child(d, ast_node1("Init", $3->node));
          ast_append_stmt(d);
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
          ASTNode *d = ast_node("VarDeclInit");
          ast_add_child(d, ast_leaf("type:int"));
          ast_add_child(d, ast_leaf("name:%s", $1));
          ast_add_child(d, ast_node1("Init", $4->node ? $4->node : ast_node("BoolExpr")));
          ast_append_stmt(d);
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
          ASTNode *d = ast_node("VarDeclInit");
          ast_add_child(d, ast_leaf("type:int"));
          ast_add_child(d, ast_leaf("name:%s", $1));
          ast_add_child(d, ast_node1("Init",
              ast_node1("Not", $5->node ? $5->node : ast_node("BoolExpr"))));
          ast_append_stmt(d);
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
          ASTNode *a = ast_node("Assign");
          ast_add_child(a, ast_leaf("id:%s", $1));
          ast_add_child(a, $3->node);
          ast_append_stmt(a);
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
          ASTNode *a = ast_node("Assign");
          ast_add_child(a, ast_leaf("id:%s", $1));
          ast_add_child(a, $4->node ? $4->node : ast_node("BoolExpr"));
          ast_append_stmt(a);
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
          ASTNode *a = ast_node("Assign");
          ast_add_child(a, ast_leaf("id:%s", $1));
          ast_add_child(a, ast_node1("Not", $5->node ? $5->node : ast_node("BoolExpr")));
          ast_append_stmt(a);
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
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:+="));
          ast_add_child(a, ast_leaf("id:%s", $1));
          ast_add_child(a, $3->node);
          ast_append_stmt(a);
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
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:-="));
          ast_add_child(a, ast_leaf("id:%s", $1));
          ast_add_child(a, $3->node);
          ast_append_stmt(a);
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
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:*="));
          ast_add_child(a, ast_leaf("id:%s", $1));
          ast_add_child(a, $3->node);
          ast_append_stmt(a);
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
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:/="));
          ast_add_child(a, ast_leaf("id:%s", $1));
          ast_add_child(a, $3->node);
          ast_append_stmt(a);
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
          ASTNode *a = ast_node("CompoundAssign");
          ast_add_child(a, ast_leaf("op:%%="));
          ast_add_child(a, ast_leaf("id:%s", $1));
          ast_add_child(a, $3->node);
          ast_append_stmt(a);
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
          emit_incdec($1, 1);
          ASTNode *n = ast_node("PostInc");
          ast_add_child(n, ast_leaf("id:%s", $1));
          ast_append_stmt(n);
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
          emit_incdec($1, 0);
          ASTNode *n = ast_node("PostDec");
          ast_add_child(n, ast_leaf("id:%s", $1));
          ast_append_stmt(n);
          free($1);
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
          emit_incdec($2, 1);
          ASTNode *n = ast_node("PreInc");
          ast_add_child(n, ast_leaf("id:%s", $2));
          ast_append_stmt(n);
          free($2);
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
          emit_incdec($2, 0);
          ASTNode *n = ast_node("PreDec");
          ast_add_child(n, ast_leaf("id:%s", $2));
          ast_append_stmt(n);
          free($2);
      }
    ;

return_stmt
    : RETURN
      {
          emit_text("return");
          ast_append_stmt(ast_node("Return"));
      }
    | RETURN aexpr
      {
          emit_text("return %s", $2->place);
          ASTNode *r = ast_node("Return");
          ast_add_child(r, $2->node);
          ast_append_stmt(r);
          free_aattr($2);
      }
    ;

M : /* empty */ { $$ = nextinstr(); } ;
N : /* empty */ { $$ = makelist(emit_goto(-1)); } ;

/* push_block: shared empty non-terminal that pushes a proper Block
   node (as opposed to push_capture which pushes a throwaway). used
   by compound_stmt's two alternatives (success + error-recovery). */
push_block
    : /* empty */ { ast_push_block(ast_node("Block")); }
    ;

/* push_capture: shared empty non-terminal that pushes a throwaway
   Block onto the stmt stack. multiple control-flow rules use this
   so bison doesn't treat each anonymous mid-rule action as a
   distinct empty production (which causes reduce/reduce conflicts). */
push_capture
    : /* empty */ { ast_push_block(ast_node("_capture")); }
    ;

/* loop_enter: shared prefix for WHILE/DO. pushes break/continue
   frames AND an AST capture block. used to be an inline mid-rule
   action, but hoisting it here keeps bison happy (see push_capture). */
loop_enter
    : /* empty */
      {
          breaklist_push();
          continuelist_push();
          ast_push_block(ast_node("_capture"));
      }
    ;

bexpr     : bor_expr { $$ = $1; } ;

bor_expr
    : bor_expr OR M band_expr
      {
          backpatch($1->falselist, $3);
          $$ = make_battr(merge_list($1->truelist, $4->truelist), $4->falselist,
                          ast_node2("||", $1->node, $4->node));
      }
    | band_expr { $$ = $1; }
    ;

band_expr
    : band_expr AND M bnot_expr
      {
          backpatch($1->truelist, $3);
          $$ = make_battr($4->truelist, merge_list($1->falselist, $4->falselist),
                          ast_node2("&&", $1->node, $4->node));
      }
    | bnot_expr { $$ = $1; }
    ;

bnot_expr
    : NOT bnot_expr { $$ = make_battr($2->falselist, $2->truelist,
                                      ast_node1("!", $2->node)); }
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
          /* an `=` used as a truth value: build an Assign-as-expression */
          $$->node = ast_node2("Assign", ast_leaf("id:%s", $1), $3->node);
          free($1); free_aattr($3);
      }
    | aexpr
      {
          /* arithmetic-as-truthy: treat nonzero as true */
          $$ = emit_truthy($1->place);
          $$->node = ast_node1("Truthy", $1->node);
          free_aattr($1);
      }
    ;

rel_bool
    : aexpr LT aexpr
      { $$ = emit_relop($1->place, "<",  $3->place);
        $$->node = ast_node2("<",  $1->node, $3->node);
        free_aattr($1); free_aattr($3); }
    | aexpr GT aexpr
      { $$ = emit_relop($1->place, ">",  $3->place);
        $$->node = ast_node2(">",  $1->node, $3->node);
        free_aattr($1); free_aattr($3); }
    | aexpr LE aexpr
      { $$ = emit_relop($1->place, "<=", $3->place);
        $$->node = ast_node2("<=", $1->node, $3->node);
        free_aattr($1); free_aattr($3); }
    | aexpr GE aexpr
      { $$ = emit_relop($1->place, ">=", $3->place);
        $$->node = ast_node2(">=", $1->node, $3->node);
        free_aattr($1); free_aattr($3); }
    | aexpr EQ aexpr
      { $$ = emit_relop($1->place, "==", $3->place);
        $$->node = ast_node2("==", $1->node, $3->node);
        free_aattr($1); free_aattr($3); }
    | aexpr NE aexpr
      { $$ = emit_relop($1->place, "!=", $3->place);
        $$->node = ast_node2("!=", $1->node, $3->node);
        free_aattr($1); free_aattr($3); }
    ;

aexpr
    : aexpr PLUS term
      { char *t = new_temp(); emit_text("%s = %s + %s", t, $1->place, $3->place);
        $$ = make_aattr(t, ast_node2("+", $1->node, $3->node));
        free_aattr($1); free_aattr($3); }
    | aexpr MINUS term
      { char *t = new_temp(); emit_text("%s = %s - %s", t, $1->place, $3->place);
        $$ = make_aattr(t, ast_node2("-", $1->node, $3->node));
        free_aattr($1); free_aattr($3); }
    | term
      { $$ = $1; }
    ;

term
    : term MUL factor
      { char *t = new_temp(); emit_text("%s = %s * %s", t, $1->place, $3->place);
        $$ = make_aattr(t, ast_node2("*", $1->node, $3->node));
        free_aattr($1); free_aattr($3); }
    | term DIV factor
      { char *t = new_temp(); emit_text("%s = %s / %s", t, $1->place, $3->place);
        $$ = make_aattr(t, ast_node2("/", $1->node, $3->node));
        free_aattr($1); free_aattr($3); }
    | term MOD factor
      { char *t = new_temp(); emit_text("%s = %s %% %s", t, $1->place, $3->place);
        $$ = make_aattr(t, ast_node2("%", $1->node, $3->node));
        free_aattr($1); free_aattr($3); }
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
          emit_incdec($2, 1);
          $$ = make_aattr(xstrdup($2),
                 ast_node1("PreInc", ast_leaf("id:%s", $2)));
          free($2);
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
          emit_incdec($2, 0);
          $$ = make_aattr(xstrdup($2),
                 ast_node1("PreDec", ast_leaf("id:%s", $2)));
          free($2);
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
          $$ = make_aattr(old,
                 ast_node1("PostInc", ast_leaf("id:%s", $1)));
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
          $$ = make_aattr(old,
                 ast_node1("PostDec", ast_leaf("id:%s", $1)));
          free($1);
      }
    | MINUS factor %prec UMINUS
      {
          char *t = new_temp();
          emit_text("%s = 0 - %s", t, $2->place);
          $$ = make_aattr(t, ast_node1("Neg", $2->node));
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
          $$ = make_aattr($1, ast_leaf("id:%s", $1));
      }
    | NUMBER
      { $$ = make_aattr($1, ast_leaf("num:%s", $1)); }
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
