#include "common.h"

SymEntry *sym_table[SYM_SIZE];

static unsigned sym_hash(const char *s) {
    unsigned h = 5381;
    while (*s) h = h * 33 ^ (unsigned char)*s++;
    return h % SYM_SIZE;
}

/* try to insert; returns 1 if name already exists */
int sym_declare(const char *name, int line, int col) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    while (e) {
        if (strcmp(e->name, name) == 0) return 1;
        e = e->next;
    }
    e = (SymEntry *)malloc(sizeof(SymEntry));
    strncpy(e->name, name, sizeof(e->name) - 1);
    e->name[sizeof(e->name) - 1] = '\0';
    e->type = SYM_INT;
    e->decl_line = line;
    e->decl_col = col;
    e->is_initialized = 0;
    e->is_used = 0;
    e->next = sym_table[h];
    sym_table[h] = e;
    return 0;
}

int sym_lookup(const char *name) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    while (e) {
        if (strcmp(e->name, name) == 0) return 1;
        e = e->next;
    }
    return 0;
}

SymEntry *sym_find(const char *name) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    while (e) {
        if (strcmp(e->name, name) == 0) return e;
        e = e->next;
    }
    return NULL;
}

/* flags var as read; warns if never assigned */
SymEntry *sym_mark_used(const char *name, int line, int col) {
    SymEntry *e = sym_find(name);
    if (!e) return NULL;
    e->is_used = 1;
    if (!e->is_initialized) {
        emit_diagnostic("<stdin>", line, col, "warning",
            "'%s' is used uninitialized [-Wuninitialized]", name);
    }
    return e;
}

/* flags var as written to */
SymEntry *sym_mark_init(const char *name) {
    SymEntry *e = sym_find(name);
    if (!e) return NULL;
    e->is_initialized = 1;
    return e;
}

const char *sym_type_str(SymEntry *e) {
    if (!e) return "unknown";
    if (e->type == SYM_INT) return "int";
    return "unknown";
}