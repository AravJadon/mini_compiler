/* ================================================================
 *  optimizer.c  —  PHASE 5: Basic Optimization
 *
 *  Optimizations implemented:
 *    1. Constant Folding       — evaluate constant expressions at compile time
 *    2. Constant Propagation   — substitute known constants into expressions
 *    3. Common Sub-Expression Elimination (CSE)
 *    4. Dead Code Elimination  — remove unused temporaries
 * ================================================================ */
#include "common.h"
#include <ctype.h>

/* ── Optimized TAC buffer ────────────────────────────────── */
Instruction opt_buf[MAX_CODE];
int opt_len = 0;

/* ── Optimization statistics ─────────────────────────────── */
int stat_const_fold = 0;
int stat_const_prop = 0;
int stat_cse = 0;
int stat_dead_elim = 0;

/* ── Parsed TAC instruction ──────────────────────────────── */
typedef struct {
    char dest[64];
    char src1[64];
    char op[8];
    char src2[64];
    int kind;  /* 0=other, 1=copy/const (dest=src1), 2=binary (dest=src1 op src2) */
} ParsedTAC;

static int is_operator(const char *s) {
    return (strcmp(s,"+")==0 || strcmp(s,"-")==0 || strcmp(s,"*")==0 ||
            strcmp(s,"/")==0 || strcmp(s,"%")==0);
}

static int is_number(const char *s) {
    if (!s || !*s) return 0;
    if (*s == '-') s++;
    if (!*s) return 0;
    while (*s) { if (!isdigit((unsigned char)*s)) return 0; s++; }
    return 1;
}

static int is_temp(const char *s) {
    return (s[0] == 't' && isdigit((unsigned char)s[1]));
}

/* Parse "dest = src1 op src2" or "dest = src1" or other */
static int parse_tac(const char *text, ParsedTAC *p) {
    char rest[256];
    memset(p, 0, sizeof(ParsedTAC));

    /* Skip non-assignment instructions */
    if (strncmp(text, "param", 5) == 0 || strncmp(text, "call", 4) == 0 ||
        strncmp(text, "return", 6) == 0 || strncmp(text, "goto", 4) == 0) {
        p->kind = 0;
        return 1;
    }

    /* Try: dest = rest */
    if (sscanf(text, "%63s = %255[^\n]", p->dest, rest) == 2) {
        /* Try: src1 op src2 */
        if (sscanf(rest, "%63s %7s %63s", p->src1, p->op, p->src2) == 3 && is_operator(p->op)) {
            p->kind = 2;
        } else {
            /* Simple copy: dest = src1 */
            sscanf(rest, "%63s", p->src1);
            p->op[0] = '\0';
            p->src2[0] = '\0';
            p->kind = 1;
        }
        return 1;
    }

    p->kind = 0;
    return 1;
}

/* Rebuild instruction text from parsed components */
static void rebuild(int idx, ParsedTAC *p) {
    if (p->kind == 2)
        snprintf(opt_buf[idx].text, sizeof(opt_buf[idx].text),
                 "%s = %s %s %s", p->dest, p->src1, p->op, p->src2);
    else if (p->kind == 1)
        snprintf(opt_buf[idx].text, sizeof(opt_buf[idx].text),
                 "%s = %s", p->dest, p->src1);
}

/* ── Check if a variable name appears as a word in text ──── */
static int contains_var(const char *text, const char *var) {
    const char *p = text;
    size_t vlen = strlen(var);
    while ((p = strstr(p, var)) != NULL) {
        int left_ok  = (p == text) || (!isalnum((unsigned char)*(p-1)) && *(p-1) != '_');
        int right_ok = !isalnum((unsigned char)*(p+vlen)) && *(p+vlen) != '_';
        if (left_ok && right_ok) return 1;
        p += vlen;
    }
    return 0;
}

/* Replace all occurrences of variable 'var' with 'rep' in a string */
static void replace_var_in_str(char *buf, int bufsize, const char *var, const char *rep) {
    char tmp[256];
    char *p = buf;
    size_t vlen = strlen(var);
    size_t rlen = strlen(rep);
    int out = 0;

    while (*p && out < bufsize - 1) {
        if (strncmp(p, var, vlen) == 0) {
            int left_ok  = (p == buf) || (!isalnum((unsigned char)*(p-1)) && *(p-1) != '_');
            int right_ok = !isalnum((unsigned char)*(p+vlen)) && *(p+vlen) != '_';
            if (left_ok && right_ok) {
                size_t i;
                for (i = 0; i < rlen && out < bufsize - 1; i++)
                    tmp[out++] = rep[i];
                p += vlen;
                continue;
            }
        }
        tmp[out++] = *p++;
    }
    tmp[out] = '\0';
    strncpy(buf, tmp, bufsize - 1);
    buf[bufsize - 1] = '\0';
}

/* ── Set of jump targets (instructions that can be branched to) ── */
static int jump_target[MAX_CODE];

static void find_jump_targets(void) {
    int i;
    memset(jump_target, 0, sizeof(int) * opt_len);
    for (i = 0; i < opt_len; i++) {
        if ((opt_buf[i].type == INS_IF || opt_buf[i].type == INS_GOTO) &&
            opt_buf[i].target >= 0 && opt_buf[i].target < opt_len) {
            jump_target[opt_buf[i].target] = 1;
        }
    }
}

/* ══════════════════════════════════════════════════════════════
 *  PASS 1: Constant Folding
 *  If both operands are numeric, compute the result.
 * ══════════════════════════════════════════════════════════════ */
static void constant_folding(void) {
    int i;
    for (i = 0; i < opt_len; i++) {
        ParsedTAC p;
        if (opt_buf[i].type != INS_TEXT) continue;
        parse_tac(opt_buf[i].text, &p);
        if (p.kind != 2) continue;
        if (!is_number(p.src1) || !is_number(p.src2)) continue;

        long a = atol(p.src1), b = atol(p.src2), result = 0;
        int valid = 1;

        if      (strcmp(p.op, "+") == 0) result = a + b;
        else if (strcmp(p.op, "-") == 0) result = a - b;
        else if (strcmp(p.op, "*") == 0) result = a * b;
        else if (strcmp(p.op, "/") == 0) { if (b != 0) result = a / b; else valid = 0; }
        else if (strcmp(p.op, "%") == 0) { if (b != 0) result = a % b; else valid = 0; }
        else valid = 0;

        if (valid) {
            snprintf(p.src1, sizeof(p.src1), "%ld", result);
            p.kind = 1;
            rebuild(i, &p);
            stat_const_fold++;
        }
    }
}

/* ══════════════════════════════════════════════════════════════
 *  PASS 2: Constant Propagation
 *  Track variables assigned constants; substitute in uses.
 * ══════════════════════════════════════════════════════════════ */
#define MAX_PROPS 512
typedef struct { char name[64]; char value[64]; } PropEntry;
static PropEntry prop_map[MAX_PROPS];
static int prop_count = 0;

static void prop_set(const char *name, const char *val) {
    int i;
    for (i = 0; i < prop_count; i++) {
        if (strcmp(prop_map[i].name, name) == 0) {
            strncpy(prop_map[i].value, val, 63);
            return;
        }
    }
    if (prop_count < MAX_PROPS) {
        strncpy(prop_map[prop_count].name, name, 63);
        strncpy(prop_map[prop_count].value, val, 63);
        prop_count++;
    }
}

static const char *prop_get(const char *name) {
    int i;
    for (i = 0; i < prop_count; i++)
        if (strcmp(prop_map[i].name, name) == 0) return prop_map[i].value;
    return NULL;
}

static void prop_invalidate(const char *name) {
    int i;
    for (i = 0; i < prop_count; i++) {
        if (strcmp(prop_map[i].name, name) == 0) {
            prop_map[i] = prop_map[--prop_count];
            return;
        }
    }
}

static void prop_clear(void) { prop_count = 0; }

static void constant_propagation(void) {
    int i;
    prop_clear();
    find_jump_targets();

    for (i = 0; i < opt_len; i++) {
        /* At jump targets, clear propagation state (conservative) */
        if (jump_target[i]) prop_clear();

        if (opt_buf[i].type == INS_IF) {
            /* Substitute constants in condition text */
            ParsedTAC p;
            char cond[256];
            strncpy(cond, opt_buf[i].text, 255);
            /* Try parsing condition: src1 op src2 */
            char s1[64], op[8], s2[64];
            if (sscanf(cond, "%63s %7s %63s", s1, op, s2) == 3) {
                const char *v1 = prop_get(s1);
                const char *v2 = prop_get(s2);
                int changed = 0;
                if (v1 && is_number(v1)) { strncpy(s1, v1, 63); changed = 1; stat_const_prop++; }
                if (v2 && is_number(v2)) { strncpy(s2, v2, 63); changed = 1; stat_const_prop++; }
                if (changed) {
                    snprintf(opt_buf[i].text, sizeof(opt_buf[i].text), "%s %s %s", s1, op, s2);
                }
            }
            continue;
        }

        if (opt_buf[i].type != INS_TEXT) continue;

        ParsedTAC p;
        parse_tac(opt_buf[i].text, &p);
        if (p.kind == 0) continue;

        /* Substitute constants in operands */
        if (p.kind == 2) {
            const char *v1 = prop_get(p.src1);
            const char *v2 = prop_get(p.src2);
            int changed = 0;
            if (v1 && is_number(v1)) { strncpy(p.src1, v1, 63); changed = 1; stat_const_prop++; }
            if (v2 && is_number(v2)) { strncpy(p.src2, v2, 63); changed = 1; stat_const_prop++; }
            if (changed) rebuild(i, &p);
        } else if (p.kind == 1 && !is_number(p.src1)) {
            const char *v = prop_get(p.src1);
            if (v && is_number(v)) {
                strncpy(p.src1, v, 63);
                rebuild(i, &p);
                stat_const_prop++;
            }
        }

        /* Re-parse after substitution for recording */
        parse_tac(opt_buf[i].text, &p);

        /* Record constant assignment: dest = number */
        if (p.kind == 1 && is_number(p.src1)) {
            prop_set(p.dest, p.src1);
        } else {
            /* dest is being assigned a non-constant; invalidate */
            prop_invalidate(p.dest);
        }

        /* After any jump, clear state */
        if (opt_buf[i].type == INS_GOTO) prop_clear();
    }
}

/* ══════════════════════════════════════════════════════════════
 *  PASS 3: Common Sub-Expression Elimination (CSE)
 *  Reuse results of identical expressions.
 * ══════════════════════════════════════════════════════════════ */
#define MAX_CSE 512
typedef struct { char expr[128]; char temp[64]; } CSEEntry;
static CSEEntry cse_map[MAX_CSE];
static int cse_count = 0;

static void cse_clear(void) { cse_count = 0; }

static const char *cse_lookup(const char *expr) {
    int i;
    for (i = 0; i < cse_count; i++)
        if (strcmp(cse_map[i].expr, expr) == 0) return cse_map[i].temp;
    return NULL;
}

static void cse_add(const char *expr, const char *temp) {
    if (cse_count < MAX_CSE) {
        strncpy(cse_map[cse_count].expr, expr, 127);
        strncpy(cse_map[cse_count].temp, temp, 63);
        cse_count++;
    }
}

/* Invalidate any CSE entry that involves variable 'name' */
static void cse_invalidate(const char *name) {
    int i = 0;
    while (i < cse_count) {
        if (contains_var(cse_map[i].expr, name)) {
            cse_map[i] = cse_map[--cse_count];
        } else {
            i++;
        }
    }
}

static void common_subexpr_elim(void) {
    int i;
    cse_clear();
    find_jump_targets();

    for (i = 0; i < opt_len; i++) {
        if (jump_target[i]) cse_clear();

        if (opt_buf[i].type != INS_TEXT) {
            if (opt_buf[i].type == INS_GOTO) cse_clear();
            continue;
        }

        ParsedTAC p;
        parse_tac(opt_buf[i].text, &p);
        if (p.kind != 2) {
            /* Any assignment invalidates CSE entries using dest */
            if (p.kind == 1) cse_invalidate(p.dest);
            continue;
        }

        /* Build expression key: "src1 op src2" */
        char expr[128];
        snprintf(expr, sizeof(expr), "%s %s %s", p.src1, p.op, p.src2);

        /* Check if this expression was computed before */
        const char *prev = cse_lookup(expr);
        if (prev) {
            /* Replace with copy from previous temp */
            snprintf(opt_buf[i].text, sizeof(opt_buf[i].text), "%s = %s", p.dest, prev);
            stat_cse++;
        } else {
            /* Record this expression */
            cse_add(expr, p.dest);
        }

        /* Invalidate any expressions using dest (since dest changed) */
        cse_invalidate(p.dest);
    }
}

/* ══════════════════════════════════════════════════════════════
 *  PASS 4: Dead Code Elimination
 *  Remove assignments to temporaries that are never used.
 * ══════════════════════════════════════════════════════════════ */
static int is_used_elsewhere(const char *name, int def_idx) {
    int i;
    for (i = 0; i < opt_len; i++) {
        if (i == def_idx) continue;
        if (opt_buf[i].type == INS_TEXT || opt_buf[i].type == INS_IF) {
            /* Check if name appears in the text (as a whole word) */
            ParsedTAC p;
            if (opt_buf[i].type == INS_TEXT) {
                parse_tac(opt_buf[i].text, &p);
                /* Check in operands, not in dest */
                if (p.kind >= 1 && contains_var(p.src1, name)) return 1;
                if (p.kind == 2 && contains_var(p.src2, name)) return 1;
                /* Also check if it appears in "other" type instructions */
                if (p.kind == 0 && contains_var(opt_buf[i].text, name)) return 1;
            } else {
                /* INS_IF: check condition */
                if (contains_var(opt_buf[i].text, name)) return 1;
            }
        }
    }
    return 0;
}

static void dead_code_elim(void) {
    int alive[MAX_CODE];
    int remap[MAX_CODE];
    int i, j;

    find_jump_targets();

    /* Mark all alive initially */
    for (i = 0; i < opt_len; i++) alive[i] = 1;

    /* Find dead temporaries (iterate until stable) */
    int changed = 1;
    while (changed) {
        changed = 0;
        for (i = 0; i < opt_len; i++) {
            if (!alive[i]) continue;
            if (opt_buf[i].type != INS_TEXT) continue;
            if (jump_target[i]) continue;

            ParsedTAC p;
            parse_tac(opt_buf[i].text, &p);
            if (p.kind == 0) continue;
            if (!is_temp(p.dest)) continue;

            if (!is_used_elsewhere(p.dest, i)) {
                alive[i] = 0;
                stat_dead_elim++;
                changed = 1;
            }
        }
    }

    /* Compact: build remap table */
    j = 0;
    for (i = 0; i < opt_len; i++) {
        if (alive[i]) {
            remap[i] = j;
            if (j != i) opt_buf[j] = opt_buf[i];
            j++;
        }
    }
    /* For dead entries, map to next alive instruction */
    for (i = opt_len - 1; i >= 0; i--) {
        if (!alive[i]) {
            int k = i + 1;
            while (k < opt_len && !alive[k]) k++;
            remap[i] = (k < opt_len) ? remap[k] : j;
        }
    }
    /* Remap jump targets */
    for (i = 0; i < j; i++) {
        if ((opt_buf[i].type == INS_IF || opt_buf[i].type == INS_GOTO) &&
            opt_buf[i].target >= 0 && opt_buf[i].target < opt_len) {
            opt_buf[i].target = remap[opt_buf[i].target];
        }
    }
    opt_len = j;
}

/* ══════════════════════════════════════════════════════════════
 *  Main optimization driver
 * ══════════════════════════════════════════════════════════════ */
void optimize(void) {
    /* Copy original TAC to optimization buffer */
    memcpy(opt_buf, code_buf, sizeof(Instruction) * code_len);
    opt_len = code_len;

    /* Reset statistics */
    stat_const_fold = 0;
    stat_const_prop = 0;
    stat_cse = 0;
    stat_dead_elim = 0;

    /* Apply optimization passes */
    constant_folding();
    constant_propagation();
    constant_folding();    /* re-run: propagation may create new folding opportunities */
    common_subexpr_elim();
    dead_code_elim();
}

/* Print optimized TAC */
void print_opt_tac(void) {
    int i;
    for (i = 0; i < opt_len; i++) {
        if (opt_buf[i].type == INS_TEXT)
            printf("  %d: %s\n", line_of(i), opt_buf[i].text);
        else if (opt_buf[i].type == INS_IF)
            printf("  %d: if %s goto %d\n", line_of(i), opt_buf[i].text, line_of(opt_buf[i].target));
        else
            printf("  %d: goto %d\n", line_of(i), line_of(opt_buf[i].target));
    }
}
