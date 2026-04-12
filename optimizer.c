#include "common.h"
#include <ctype.h>

Instruction opt_buf[MAX_CODE];
int opt_len = 0;

/* stats */
int stat_const_fold = 0;
int stat_const_prop = 0;
int stat_cse = 0;
int stat_dead_elim = 0;
int stat_algebraic = 0;
int stat_copy_prop = 0;
int stat_strength_red = 0;
int stat_global_const = 0;
int stat_loop_inv = 0;
int stat_unreachable = 0;

/* parsed form of a single TAC instruction */
typedef struct {
    char dest[64];
    char src1[64];
    char op[8];
    char src2[64];
    int kind;  /* 0=other, 1=copy/const, 2=binary */
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

static int parse_tac(const char *text, ParsedTAC *p) {
    char rest[256];
    memset(p, 0, sizeof(ParsedTAC));

    if (strncmp(text, "param", 5) == 0 || strncmp(text, "call", 4) == 0 ||
        strncmp(text, "return", 6) == 0 || strncmp(text, "goto", 4) == 0) {
        p->kind = 0;
        return 1;
    }

    if (sscanf(text, "%63s = %255[^\n]", p->dest, rest) == 2) {
        if (sscanf(rest, "%63s %7s %63s", p->src1, p->op, p->src2) == 3 && is_operator(p->op)) {
            p->kind = 2;
        } else {
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

static void rebuild(int idx, ParsedTAC *p) {
    if (p->kind == 2)
        snprintf(opt_buf[idx].text, sizeof(opt_buf[idx].text),
                 "%s = %s %s %s", p->dest, p->src1, p->op, p->src2);
    else if (p->kind == 1)
        snprintf(opt_buf[idx].text, sizeof(opt_buf[idx].text),
                 "%s = %s", p->dest, p->src1);
}

/* checks if 'var' appears as a whole word in 'text' */
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

/* replaces whole-word occurrences of 'var' with 'rep' in buf */
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

/* jump target tracking */
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

/* ---- PASS: constant folding ----
   if both operands are numbers, just compute it now */
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

/* ---- PASS: algebraic simplification ----
   x + 0 = x, x - 0 = x, x * 1 = x, x / 1 = x,
   x * 0 = 0, 0 * x = 0, 0 + x = x, 0 - x = neg handled elsewhere,
   x - x = 0 */
static void algebraic_simplification(void) {
    int i;
    for (i = 0; i < opt_len; i++) {
        ParsedTAC p;
        if (opt_buf[i].type != INS_TEXT) continue;
        parse_tac(opt_buf[i].text, &p);
        if (p.kind != 2) continue;

        int s1_is_num = is_number(p.src1);
        int s2_is_num = is_number(p.src2);
        long v1 = s1_is_num ? atol(p.src1) : 0;
        long v2 = s2_is_num ? atol(p.src2) : 0;
        int changed = 0;

        if (strcmp(p.op, "+") == 0) {
            if (s2_is_num && v2 == 0) {
                /* x + 0 = x */
                strncpy(p.src1, p.src1, 63); p.kind = 1; changed = 1;
            } else if (s1_is_num && v1 == 0) {
                /* 0 + x = x */
                strncpy(p.src1, p.src2, 63); p.kind = 1; changed = 1;
            }
        } else if (strcmp(p.op, "-") == 0) {
            if (s2_is_num && v2 == 0) {
                /* x - 0 = x */
                p.kind = 1; changed = 1;
            } else if (strcmp(p.src1, p.src2) == 0) {
                /* x - x = 0 */
                strncpy(p.src1, "0", 63); p.kind = 1; changed = 1;
            }
        } else if (strcmp(p.op, "*") == 0) {
            if ((s2_is_num && v2 == 0) || (s1_is_num && v1 == 0)) {
                /* x * 0 or 0 * x = 0 */
                strncpy(p.src1, "0", 63); p.kind = 1; changed = 1;
            } else if (s2_is_num && v2 == 1) {
                /* x * 1 = x */
                p.kind = 1; changed = 1;
            } else if (s1_is_num && v1 == 1) {
                /* 1 * x = x */
                strncpy(p.src1, p.src2, 63); p.kind = 1; changed = 1;
            }
        } else if (strcmp(p.op, "/") == 0) {
            if (s2_is_num && v2 == 1) {
                /* x / 1 = x */
                p.kind = 1; changed = 1;
            } else if (strcmp(p.src1, p.src2) == 0) {
                /* x / x = 1 (assuming x != 0, but compile-time we just do it) */
                strncpy(p.src1, "1", 63); p.kind = 1; changed = 1;
            }
        } else if (strcmp(p.op, "%") == 0) {
            if (s2_is_num && v2 == 1) {
                /* x % 1 = 0 */
                strncpy(p.src1, "0", 63); p.kind = 1; changed = 1;
            } else if (strcmp(p.src1, p.src2) == 0) {
                /* x % x = 0 */
                strncpy(p.src1, "0", 63); p.kind = 1; changed = 1;
            }
        }

        if (changed) {
            rebuild(i, &p);
            stat_algebraic++;
        }
    }
}

/* ---- PASS: strength reduction ----
   x * 2 -> x + x
   2 * x -> x + x */
static void strength_reduction(void) {
    int i;
    for (i = 0; i < opt_len; i++) {
        ParsedTAC p;
        if (opt_buf[i].type != INS_TEXT) continue;
        parse_tac(opt_buf[i].text, &p);
        if (p.kind != 2) continue;
        if (strcmp(p.op, "*") != 0) continue;

        int s1_num = is_number(p.src1);
        int s2_num = is_number(p.src2);

        if (s2_num && atol(p.src2) == 2) {
            /* x * 2 -> x + x */
            strcpy(p.op, "+");
            strncpy(p.src2, p.src1, 63);
            rebuild(i, &p);
            stat_strength_red++;
        } else if (s1_num && atol(p.src1) == 2) {
            /* 2 * x -> x + x */
            strncpy(p.src1, p.src2, 63);
            strcpy(p.op, "+");
            rebuild(i, &p);
            stat_strength_red++;
        }
    }
}

/* ---- PASS: constant propagation (local) ----
   track vars assigned constants, substitute into later uses */
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
        if (jump_target[i]) prop_clear();

        if (opt_buf[i].type == INS_IF) {
            char s1[64], op[8], s2[64];
            if (sscanf(opt_buf[i].text, "%63s %7s %63s", s1, op, s2) == 3) {
                const char *v1 = prop_get(s1);
                const char *v2 = prop_get(s2);
                int changed = 0;
                if (v1 && is_number(v1)) { strncpy(s1, v1, 63); changed = 1; stat_const_prop++; }
                if (v2 && is_number(v2)) { strncpy(s2, v2, 63); changed = 1; stat_const_prop++; }
                if (changed)
                    snprintf(opt_buf[i].text, sizeof(opt_buf[i].text), "%s %s %s", s1, op, s2);
            }
            continue;
        }

        if (opt_buf[i].type != INS_TEXT) continue;

        ParsedTAC p;
        parse_tac(opt_buf[i].text, &p);
        if (p.kind == 0) continue;

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

        parse_tac(opt_buf[i].text, &p);

        if (p.kind == 1 && is_number(p.src1)) {
            prop_set(p.dest, p.src1);
        } else {
            prop_invalidate(p.dest);
        }

        if (opt_buf[i].type == INS_GOTO) prop_clear();
    }
}

/* ---- PASS: copy propagation (local) ----
   if t1 = a, replace later uses of t1 with a (within same block) */
#define MAX_COPIES 512
typedef struct { char from[64]; char to[64]; } CopyEntry;
static CopyEntry copy_map[MAX_COPIES];
static int copy_count = 0;

static void copy_set(const char *from, const char *to) {
    int i;
    /* if from already has an entry, update it */
    for (i = 0; i < copy_count; i++) {
        if (strcmp(copy_map[i].from, from) == 0) {
            strncpy(copy_map[i].to, to, 63);
            return;
        }
    }
    if (copy_count < MAX_COPIES) {
        strncpy(copy_map[copy_count].from, from, 63);
        strncpy(copy_map[copy_count].to, to, 63);
        copy_count++;
    }
}

static const char *copy_get(const char *from) {
    int i;
    for (i = 0; i < copy_count; i++)
        if (strcmp(copy_map[i].from, from) == 0) return copy_map[i].to;
    return NULL;
}

/* if someone redefines a variable, any copies pointing to/from it are stale */
static void copy_invalidate(const char *name) {
    int i = 0;
    while (i < copy_count) {
        if (strcmp(copy_map[i].from, name) == 0 || strcmp(copy_map[i].to, name) == 0) {
            copy_map[i] = copy_map[--copy_count];
        } else {
            i++;
        }
    }
}

static void copy_clear(void) { copy_count = 0; }

static void copy_propagation(void) {
    int i;
    copy_clear();
    find_jump_targets();

    for (i = 0; i < opt_len; i++) {
        if (jump_target[i]) copy_clear();

        if (opt_buf[i].type == INS_IF) {
            /* substitute copies in condition */
            char s1[64], op[8], s2[64];
            if (sscanf(opt_buf[i].text, "%63s %7s %63s", s1, op, s2) == 3) {
                const char *r1 = copy_get(s1);
                const char *r2 = copy_get(s2);
                int changed = 0;
                if (r1) { strncpy(s1, r1, 63); changed = 1; stat_copy_prop++; }
                if (r2) { strncpy(s2, r2, 63); changed = 1; stat_copy_prop++; }
                if (changed)
                    snprintf(opt_buf[i].text, sizeof(opt_buf[i].text), "%s %s %s", s1, op, s2);
            }
            continue;
        }

        if (opt_buf[i].type != INS_TEXT) {
            if (opt_buf[i].type == INS_GOTO) copy_clear();
            continue;
        }

        ParsedTAC p;
        parse_tac(opt_buf[i].text, &p);
        if (p.kind == 0) continue;

        /* substitute in operands */
        int changed = 0;
        if (p.kind >= 1) {
            const char *r1 = copy_get(p.src1);
            if (r1 && !is_number(p.src1)) { strncpy(p.src1, r1, 63); changed = 1; stat_copy_prop++; }
        }
        if (p.kind == 2) {
            const char *r2 = copy_get(p.src2);
            if (r2 && !is_number(p.src2)) { strncpy(p.src2, r2, 63); changed = 1; stat_copy_prop++; }
        }
        if (changed) rebuild(i, &p);

        /* re-parse and record copies */
        parse_tac(opt_buf[i].text, &p);
        copy_invalidate(p.dest);

        /* if it's a simple copy (dest = src, no op), record it */
        if (p.kind == 1 && !is_number(p.src1)) {
            copy_set(p.dest, p.src1);
        }
    }
}

/* ---- PASS: common sub-expression elimination ---- */
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
            if (p.kind == 1) cse_invalidate(p.dest);
            continue;
        }

        char expr[128];
        snprintf(expr, sizeof(expr), "%s %s %s", p.src1, p.op, p.src2);

        const char *prev = cse_lookup(expr);
        if (prev) {
            snprintf(opt_buf[i].text, sizeof(opt_buf[i].text), "%s = %s", p.dest, prev);
            stat_cse++;
        } else {
            cse_add(expr, p.dest);
        }

        cse_invalidate(p.dest);
    }
}

/* ---- PASS: dead code elimination ----
   remove assignments to temps that nobody reads */
static int is_used_elsewhere(const char *name, int def_idx) {
    int i;
    for (i = 0; i < opt_len; i++) {
        if (i == def_idx) continue;
        if (opt_buf[i].type == INS_TEXT || opt_buf[i].type == INS_IF) {
            ParsedTAC p;
            if (opt_buf[i].type == INS_TEXT) {
                parse_tac(opt_buf[i].text, &p);
                if (p.kind >= 1 && contains_var(p.src1, name)) return 1;
                if (p.kind == 2 && contains_var(p.src2, name)) return 1;
                if (p.kind == 0 && contains_var(opt_buf[i].text, name)) return 1;
            } else {
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

    for (i = 0; i < opt_len; i++) alive[i] = 1;

    /* keep removing dead temps until nothing changes */
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

    /* compact the buffer and remap jump targets */
    j = 0;
    for (i = 0; i < opt_len; i++) {
        if (alive[i]) {
            remap[i] = j;
            if (j != i) opt_buf[j] = opt_buf[i];
            j++;
        }
    }
    for (i = opt_len - 1; i >= 0; i--) {
        if (!alive[i]) {
            int k = i + 1;
            while (k < opt_len && !alive[k]) k++;
            remap[i] = (k < opt_len) ? remap[k] : j;
        }
    }
    for (i = 0; i < j; i++) {
        if ((opt_buf[i].type == INS_IF || opt_buf[i].type == INS_GOTO) &&
            opt_buf[i].target >= 0 && opt_buf[i].target < opt_len) {
            opt_buf[i].target = remap[opt_buf[i].target];
        }
    }
    opt_len = j;
}

/* ---- PASS: global constant propagation ----
   multiple iterations across the whole TAC until nothing changes.
   unlike local const prop which resets at every jump target,
   this one runs repeated passes over the entire buffer. */
static void global_constant_propagation(void) {
    int changed = 1;
    while (changed) {
        changed = 0;
        prop_clear();

        int i;
        for (i = 0; i < opt_len; i++) {
            if (opt_buf[i].type == INS_IF) {
                char s1[64], op[8], s2[64];
                if (sscanf(opt_buf[i].text, "%63s %7s %63s", s1, op, s2) == 3) {
                    const char *v1 = prop_get(s1);
                    const char *v2 = prop_get(s2);
                    int did = 0;
                    if (v1 && is_number(v1)) { strncpy(s1, v1, 63); did = 1; }
                    if (v2 && is_number(v2)) { strncpy(s2, v2, 63); did = 1; }
                    if (did) {
                        snprintf(opt_buf[i].text, sizeof(opt_buf[i].text), "%s %s %s", s1, op, s2);
                        stat_global_const++;
                        changed = 1;
                    }
                }
                continue;
            }

            if (opt_buf[i].type != INS_TEXT) continue;

            ParsedTAC p;
            parse_tac(opt_buf[i].text, &p);
            if (p.kind == 0) continue;

            if (p.kind == 2) {
                const char *v1 = prop_get(p.src1);
                const char *v2 = prop_get(p.src2);
                int did = 0;
                if (v1 && is_number(v1)) { strncpy(p.src1, v1, 63); did = 1; }
                if (v2 && is_number(v2)) { strncpy(p.src2, v2, 63); did = 1; }
                if (did) { rebuild(i, &p); stat_global_const++; changed = 1; }
            } else if (p.kind == 1 && !is_number(p.src1)) {
                const char *v = prop_get(p.src1);
                if (v && is_number(v)) {
                    strncpy(p.src1, v, 63);
                    rebuild(i, &p);
                    stat_global_const++;
                    changed = 1;
                }
            }

            /* re-parse to record */
            parse_tac(opt_buf[i].text, &p);
            if (p.kind == 1 && is_number(p.src1)) {
                prop_set(p.dest, p.src1);
            } else {
                prop_invalidate(p.dest);
            }
        }
    }
}

/* ---- PASS: loop-invariant code motion ----
   find loops (back edges: goto that jumps to an earlier instruction),
   identify assignments inside the loop whose operands don't change
   within the loop body, and move them before the loop header */
static void loop_invariant_motion(void) {
    find_jump_targets();

    int i;
    for (i = 0; i < opt_len; i++) {
        /* look for back edges: a goto that jumps backward */
        if (opt_buf[i].type != INS_GOTO) continue;
        int loop_start = opt_buf[i].target;
        if (loop_start < 0 || loop_start >= i) continue;
        int loop_end = i;

        /* collect all vars that get defined inside the loop */
        char defined_in_loop[256][64];
        int def_count = 0;
        int j;
        for (j = loop_start; j <= loop_end && def_count < 256; j++) {
            if (opt_buf[j].type != INS_TEXT) continue;
            ParsedTAC p;
            parse_tac(opt_buf[j].text, &p);
            if (p.kind >= 1 && p.dest[0] != '\0') {
                /* check we haven't added this one already */
                int dup = 0, k;
                for (k = 0; k < def_count; k++)
                    if (strcmp(defined_in_loop[k], p.dest) == 0) { dup = 1; break; }
                if (!dup) {
                    strncpy(defined_in_loop[def_count], p.dest, 63);
                    defined_in_loop[def_count][63] = '\0';
                    def_count++;
                }
            }
        }

        /* now check each instruction: is it invariant?
           invariant = binary/copy where neither operand is defined inside the loop */
        for (j = loop_start; j <= loop_end; j++) {
            if (opt_buf[j].type != INS_TEXT) continue;
            if (jump_target[j]) continue; /* don't move branch targets */

            ParsedTAC p;
            parse_tac(opt_buf[j].text, &p);
            if (p.kind == 0) continue;
            if (!is_temp(p.dest)) continue; /* only move temporaries */

            /* check if operands are loop-invariant */
            int invariant = 1;
            int k;
            for (k = 0; k < def_count; k++) {
                if (p.kind >= 1 && strcmp(p.src1, defined_in_loop[k]) == 0) { invariant = 0; break; }
                if (p.kind == 2 && strcmp(p.src2, defined_in_loop[k]) == 0) { invariant = 0; break; }
            }
            /* also the dest itself shouldn't be used before this point in the loop */
            if (!invariant) continue;

            /* also check dest isn't defined more than once in the loop */
            int def_times = 0;
            for (k = loop_start; k <= loop_end; k++) {
                if (opt_buf[k].type != INS_TEXT) continue;
                ParsedTAC q;
                parse_tac(opt_buf[k].text, &q);
                if (q.kind >= 1 && strcmp(q.dest, p.dest) == 0) def_times++;
            }
            if (def_times != 1) continue;

            /* move it: shift everything from loop_start to j-1 down by one,
               put the invariant instruction at loop_start */
            Instruction saved = opt_buf[j];
            for (k = j; k > loop_start; k--)
                opt_buf[k] = opt_buf[k - 1];
            opt_buf[loop_start] = saved;

            /* adjust jump targets that were in the shifted range */
            for (k = 0; k < opt_len; k++) {
                if (opt_buf[k].type == INS_IF || opt_buf[k].type == INS_GOTO) {
                    int t = opt_buf[k].target;
                    if (t >= loop_start && t <= j) {
                        opt_buf[k].target = t + 1;
                    }
                }
            }

            stat_loop_inv++;
            /* only move one per loop per pass to keep it simple */
            break;
        }
    }
}

/* ---- PASS: unreachable code elimination ----
   mark all reachable instructions starting from 0 via fall-through
   and jumps; anything unmarked is dead */
static void unreachable_code_elim(void) {
    int reachable[MAX_CODE];
    int remap[MAX_CODE];
    int i, j;

    memset(reachable, 0, sizeof(int) * opt_len);

    /* BFS/DFS from instruction 0 */
    int stack[MAX_CODE];
    int sp = 0;
    stack[sp++] = 0;

    while (sp > 0) {
        int idx = stack[--sp];
        if (idx < 0 || idx >= opt_len) continue;
        if (reachable[idx]) continue;
        reachable[idx] = 1;

        if (opt_buf[idx].type == INS_GOTO) {
            /* unconditional jump: only the target is reachable, not fall-through */
            if (opt_buf[idx].target >= 0)
                stack[sp++] = opt_buf[idx].target;
        } else if (opt_buf[idx].type == INS_IF) {
            /* conditional: both target and fall-through are reachable */
            if (opt_buf[idx].target >= 0)
                stack[sp++] = opt_buf[idx].target;
            stack[sp++] = idx + 1;
        } else {
            /* normal instruction: fall through */
            stack[sp++] = idx + 1;
        }
    }

    /* count how many we're removing */
    int removed = 0;
    for (i = 0; i < opt_len; i++)
        if (!reachable[i]) removed++;

    if (removed == 0) return;

    /* compact */
    j = 0;
    for (i = 0; i < opt_len; i++) {
        if (reachable[i]) {
            remap[i] = j;
            if (j != i) opt_buf[j] = opt_buf[i];
            j++;
        }
    }
    /* dead entries map to next reachable */
    for (i = opt_len - 1; i >= 0; i--) {
        if (!reachable[i]) {
            int k = i + 1;
            while (k < opt_len && !reachable[k]) k++;
            remap[i] = (k < opt_len) ? remap[k] : j;
        }
    }
    /* fix jump targets */
    for (i = 0; i < j; i++) {
        if ((opt_buf[i].type == INS_IF || opt_buf[i].type == INS_GOTO) &&
            opt_buf[i].target >= 0 && opt_buf[i].target < opt_len) {
            opt_buf[i].target = remap[opt_buf[i].target];
        }
    }

    stat_unreachable += removed;
    opt_len = j;
}

/* main optimization driver */
void optimize(void) {
    memcpy(opt_buf, code_buf, sizeof(Instruction) * code_len);
    opt_len = code_len;

    stat_const_fold = 0;
    stat_const_prop = 0;
    stat_cse = 0;
    stat_dead_elim = 0;
    stat_algebraic = 0;
    stat_copy_prop = 0;
    stat_strength_red = 0;
    stat_global_const = 0;
    stat_loop_inv = 0;
    stat_unreachable = 0;

    /* --- local passes --- */
    constant_folding();
    algebraic_simplification();
    strength_reduction();
    constant_propagation();
    copy_propagation();
    constant_folding();   /* propagation might've created new folding chances */
    algebraic_simplification();
    common_subexpr_elim();
    dead_code_elim();

    /* --- global passes --- */
    global_constant_propagation();
    constant_folding();   /* global prop might expose more folding */
    loop_invariant_motion();
    unreachable_code_elim();
    dead_code_elim();     /* final cleanup */
}

void print_opt_tac(void) {
    int i;
    for (i = 0; i < opt_len; i++) {
        if (opt_buf[i].type == INS_TEXT) {
            printf("  %d: %s\n", line_of(i), opt_buf[i].text);
        } else if (opt_buf[i].type == INS_IF) {
            if (opt_buf[i].target < 0)
                printf("  %d: if %s goto ???  ; unpatched\n", line_of(i), opt_buf[i].text);
            else
                printf("  %d: if %s goto %d\n", line_of(i), opt_buf[i].text, line_of(opt_buf[i].target));
        } else {
            if (opt_buf[i].target < 0)
                printf("  %d: goto ???  ; unpatched\n", line_of(i));
            else
                printf("  %d: goto %d\n", line_of(i), line_of(opt_buf[i].target));
        }
    }
}