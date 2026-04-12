/* ================================================================
 *  symtab.c  —  PHASE 3: Symbol Table (Semantic Analysis)
 *
 *  Enhanced with:
 *    - Declaration line/col tracking
 *    - Initialization and usage flags
 *    - sym_find() for retrieving entries for diagnostics
 * ================================================================ */
#include "common.h"

SymEntry *sym_table[SYM_SIZE];

static unsigned sym_hash(const char *s) {
    unsigned h = 5381;
    while (*s) h = h * 33 ^ (unsigned char)*s++;
    return h % SYM_SIZE;
}

/* Returns 1 if already declared (duplicate), 0 if newly inserted */
int sym_declare(const char *name, int line, int col) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    while (e) {
        if (strcmp(e->name, name) == 0) return 1;  /* duplicate */
        e = e->next;
    }
    e = (SymEntry *)malloc(sizeof(SymEntry));
    strncpy(e->name, name, sizeof(e->name) - 1);
    e->name[sizeof(e->name) - 1] = '\0';
    e->decl_line = line;
    e->decl_col = col;
    e->is_initialized = 0;
    e->is_used = 0;
    e->next = sym_table[h];
    sym_table[h] = e;
    return 0;
}

/* Returns 1 if found, 0 if undeclared */
int sym_lookup(const char *name) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    while (e) {
        if (strcmp(e->name, name) == 0) return 1;
        e = e->next;
    }
    return 0;
}

/* Returns pointer to entry, or NULL if not found */
SymEntry *sym_find(const char *name) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    while (e) {
        if (strcmp(e->name, name) == 0) return e;
        e = e->next;
    }
    return NULL;
}