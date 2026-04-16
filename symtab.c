#include "common.h"

SymEntry *sym_table[SYM_SIZE];
static SymEntry *all_symbols_head = NULL;
static SymEntry *all_symbols_tail = NULL;

/* current scope depth — 0 at start (file/global), bumped on each { */
static int current_scope = 0;

static unsigned sym_hash(const char *s) {
    unsigned h = 5381;
    while (*s) h = h * 33 ^ (unsigned char)*s++;
    return h % SYM_SIZE;
}

int sym_current_scope(void) {
    return current_scope;
}

void sym_push_scope(void) {
    current_scope++;
}

/* on scope exit, walk every hash bucket and unlink anything
   declared at the scope we're leaving. the rest stays put. */
void sym_pop_scope(void) {
    if (current_scope <= 0) return;
    int i;
    for (i = 0; i < SYM_SIZE; i++) {
        SymEntry **pp = &sym_table[i];
        while (*pp) {
            SymEntry *e = *pp;
            if (e->scope_level == current_scope) {
                *pp = e->next;
                e->next = NULL;
                e->is_active = 0;
            } else {
                pp = &e->next;
            }
        }
    }
    current_scope--;
}

/* try to insert; returns 1 if a name already exists at the SAME scope.
   inner scopes shadowing outer ones is legal — e.g. test11 has
   `int a;` at global and `int a;` inside main, which is fine. */
int sym_declare(const char *name, int line, int col) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    while (e) {
        if (e->is_active &&
            strcmp(e->name, name) == 0 &&
            e->scope_level == current_scope)
            return 1;
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
    e->scope_level = current_scope;
    e->all_next = NULL;
    e->is_active = 1;
    e->next = sym_table[h];
    sym_table[h] = e;

    if (all_symbols_tail) {
        all_symbols_tail->all_next = e;
    } else {
        all_symbols_head = e;
    }
    all_symbols_tail = e;
    return 0;
}

/* walk the hash chain and return the innermost-scope match.
   we prefer deeper scopes because those shadow outer ones. */
int sym_lookup(const char *name) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    int best = -1;
    while (e) {
        if (e->is_active &&
            strcmp(e->name, name) == 0 &&
            e->scope_level > best)
            best = e->scope_level;
        e = e->next;
    }
    return (best >= 0);
}

SymEntry *sym_find(const char *name) {
    unsigned h = sym_hash(name);
    SymEntry *e = sym_table[h];
    SymEntry *best = NULL;
    while (e) {
        if (e->is_active && strcmp(e->name, name) == 0) {
            if (!best || e->scope_level > best->scope_level)
                best = e;
        }
        e = e->next;
    }
    return best;
}

SymEntry *sym_all_entries(void) {
    return all_symbols_head;
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
