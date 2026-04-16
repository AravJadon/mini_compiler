/* ================================================================
 *  tac_ast.c  —  AST node construction and printing
 *
 *  Built separately so tac.c stays untouched. These functions
 *  let the parser build a tree during parsing, which main.c
 *  prints in Phase 2 output.
 * ================================================================ */
#include "common.h"

ASTNode *ast_root = NULL;

ASTNode *ast_node(const char *label) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode));
    strncpy(n->label, label, sizeof(n->label) - 1);
    return n;
}

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

void ast_add_child(ASTNode *parent, ASTNode *child) {
    if (!parent || !child) return;
    if (parent->nchildren < AST_MAX_CHILDREN)
        parent->children[parent->nchildren++] = child;
}

void ast_print(ASTNode *node, int depth) {
    if (!node) return;
    int i;
    for (i = 0; i < depth; i++) printf("  |");
    printf("-- %s\n", node->label);
    for (i = 0; i < node->nchildren; i++)
        ast_print(node->children[i], depth + 1);
}

void ast_free(ASTNode *node) {
    if (!node) return;
    int i;
    for (i = 0; i < node->nchildren; i++)
        ast_free(node->children[i]);
    free(node);
}