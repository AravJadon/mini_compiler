/* ================================================================
 *  tac_ast.c  —  AST node construction and printing
 *
 *  Kept separate from tac.c so the TAC generator stays clean.
 *  The parser attaches an ASTNode to each AAttr/BAttr as it reduces,
 *  main.c walks the finished tree in phase 2, and everything is
 *  freed at the end.
 *
 *  The printer uses box-drawing characters (├── / └── / │) so the
 *  output is genuinely tree-shaped regardless of fan-out or depth.
 *  Flip AST_ASCII_ONLY to 1 if the terminal mangles UTF-8.
 * ================================================================ */
#include "common.h"
#include <stdarg.h>

#ifndef AST_ASCII_ONLY
#define AST_ASCII_ONLY 0
#endif

#if AST_ASCII_ONLY
  #define CH_BRANCH   "|-- "
  #define CH_LAST     "`-- "
  #define CH_VERT     "|   "
  #define CH_GAP      "    "
#else
  #define CH_BRANCH   "\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 "   /* ├──  */
  #define CH_LAST     "\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 "   /* └──  */
  #define CH_VERT     "\xe2\x94\x82   "                         /* │     */
  #define CH_GAP      "    "
#endif

/* the root lives here; parser.y's top production sets it */
ASTNode *ast_root = NULL;

/* plain node — label is a fixed string, children added later */
ASTNode *ast_node(const char *label) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (label) {
        strncpy(n->label, label, sizeof(n->label) - 1);
        n->label[sizeof(n->label) - 1] = '\0';
    }
    return n;
}

/* printf-style leaf — lets callers write ast_leaf("id:%s", name)
   or ast_leaf("num:%s", text) without sprintf'ing into a temp. */
ASTNode *ast_leaf(const char *fmt, ...) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode));
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(n->label, sizeof(n->label), fmt, ap);
    va_end(ap);
    return n;
}

/* convenience builders for the common arities. anything bigger
   should use ast_node() + ast_add_child() in a loop. */
ASTNode *ast_node1(const char *label, ASTNode *c1) {
    ASTNode *n = ast_node(label);
    if (c1) { n->children[0] = c1; n->nchildren = 1; }
    return n;
}

ASTNode *ast_node2(const char *label, ASTNode *c1, ASTNode *c2) {
    ASTNode *n = ast_node(label);
    if (c1) n->children[n->nchildren++] = c1;
    if (c2) n->children[n->nchildren++] = c2;
    return n;
}

ASTNode *ast_node3(const char *label, ASTNode *c1, ASTNode *c2, ASTNode *c3) {
    ASTNode *n = ast_node(label);
    if (c1) n->children[n->nchildren++] = c1;
    if (c2) n->children[n->nchildren++] = c2;
    if (c3) n->children[n->nchildren++] = c3;
    return n;
}

ASTNode *ast_node4(const char *label, ASTNode *c1, ASTNode *c2, ASTNode *c3, ASTNode *c4) {
    ASTNode *n = ast_node(label);
    if (c1) n->children[n->nchildren++] = c1;
    if (c2) n->children[n->nchildren++] = c2;
    if (c3) n->children[n->nchildren++] = c3;
    if (c4) n->children[n->nchildren++] = c4;
    return n;
}

/* silently drops the child if we've hit AST_MAX_CHILDREN. */
void ast_add_child(ASTNode *parent, ASTNode *child) {
    if (!parent || !child) return;
    if (parent->nchildren < AST_MAX_CHILDREN)
        parent->children[parent->nchildren++] = child;
}

/* ----------------------------------------------------------------
 *  tree-shaped printer
 *
 *  "prefix" is the string of │ and spaces drawn on every line to
 *  the left of this node. a last child extends prefix with spaces,
 *  a non-last child extends it with │ so the vertical bar carries
 *  through that column for every descendant below.
 * ---------------------------------------------------------------- */
static void ast_print_rec(ASTNode *node, const char *prefix, int is_last, int is_root) {
    if (!node) return;

    if (is_root) {
        printf("  %s\n", node->label);
    } else {
        printf("  %s%s%s\n", prefix, is_last ? CH_LAST : CH_BRANCH, node->label);
    }

    char child_prefix[2048];
    if (is_root) {
        child_prefix[0] = '\0';
    } else {
        snprintf(child_prefix, sizeof(child_prefix), "%s%s",
                 prefix, is_last ? CH_GAP : CH_VERT);
    }

    int i;
    for (i = 0; i < node->nchildren; i++) {
        int last = (i == node->nchildren - 1);
        ast_print_rec(node->children[i], child_prefix, last, 0);
    }
}

/* public entry. "depth" is kept for API compat but ignored — the
   printer computes its own prefixes. */
void ast_print(ASTNode *node, int depth) {
    (void)depth;
    if (!node) return;
    ast_print_rec(node, "", 1, 1);
}

void ast_free(ASTNode *node) {
    if (!node) return;
    int i;
    for (i = 0; i < node->nchildren; i++)
        ast_free(node->children[i]);
    free(node);
}
