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
int stat_cond_fold = 0;
int stat_loop_unroll = 0;
int stat_loop_jam = 0;

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

/* ---- loop region map ----
   for each instruction index, record whether it's the head of a
   loop (target of a back-edge) and the range of the loop body.
   the optimizer uses this to invalidate const/copy maps when
   entering a loop, not globally — that way, code BEFORE the loop
   still benefits from propagation. */

static int loop_head[MAX_CODE];      /* 1 if this index is a loop header */
static int loop_end_of[MAX_CODE];    /* if loop_head[i], this is the back-edge idx */

static void scan_loops(void) {
    memset(loop_head, 0, sizeof(int) * opt_len);
    memset(loop_end_of, 0, sizeof(int) * opt_len);
    int i;
    for (i = 0; i < opt_len; i++) {
        int back_target = -1;
        if (opt_buf[i].type == INS_GOTO &&
            opt_buf[i].target >= 0 && opt_buf[i].target < i) {
            back_target = opt_buf[i].target;
        } else if (opt_buf[i].type == INS_IF &&
            opt_buf[i].target >= 0 && opt_buf[i].target < i) {
            back_target = opt_buf[i].target;
        }
        if (back_target < 0) continue;
        loop_head[back_target] = 1;
        loop_end_of[back_target] = i;
    }
}

/* does any instruction in [start..end] redefine `name`? */
static int var_defined_in_range(const char *name, int start, int end) {
    int i;
    for (i = start; i <= end && i < opt_len; i++) {
        if (opt_buf[i].type != INS_TEXT) continue;
        ParsedTAC p;
        parse_tac(opt_buf[i].text, &p);
        if (p.kind >= 1 && strcmp(p.dest, name) == 0) return 1;
    }
    return 0;
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
   x * 0 = 0, 0 * x = 0, 0 + x = x, x - x = 0, etc. */
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
                p.kind = 1; changed = 1;
            } else if (s1_is_num && v1 == 0) {
                strncpy(p.src1, p.src2, 63); p.kind = 1; changed = 1;
            }
        } else if (strcmp(p.op, "-") == 0) {
            if (s2_is_num && v2 == 0) {
                p.kind = 1; changed = 1;
            } else if (strcmp(p.src1, p.src2) == 0) {
                strncpy(p.src1, "0", 63); p.kind = 1; changed = 1;
            }
        } else if (strcmp(p.op, "*") == 0) {
            if ((s2_is_num && v2 == 0) || (s1_is_num && v1 == 0)) {
                strncpy(p.src1, "0", 63); p.kind = 1; changed = 1;
            } else if (s2_is_num && v2 == 1) {
                p.kind = 1; changed = 1;
            } else if (s1_is_num && v1 == 1) {
                strncpy(p.src1, p.src2, 63); p.kind = 1; changed = 1;
            }
        } else if (strcmp(p.op, "/") == 0) {
            if (s2_is_num && v2 == 1) {
                p.kind = 1; changed = 1;
            } else if (strcmp(p.src1, p.src2) == 0) {
                strncpy(p.src1, "1", 63); p.kind = 1; changed = 1;
            }
        } else if (strcmp(p.op, "%") == 0) {
            if (s2_is_num && v2 == 1) {
                strncpy(p.src1, "0", 63); p.kind = 1; changed = 1;
            } else if (strcmp(p.src1, p.src2) == 0) {
                strncpy(p.src1, "0", 63); p.kind = 1; changed = 1;
            }
        }

        if (changed) {
            rebuild(i, &p);
            stat_algebraic++;
        }
    }
}

/* ---- PASS: strength reduction ---- */
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
            strcpy(p.op, "+");
            strncpy(p.src2, p.src1, 63);
            rebuild(i, &p);
            stat_strength_red++;
        } else if (s1_num && atol(p.src1) == 2) {
            strncpy(p.src1, p.src2, 63);
            strcpy(p.op, "+");
            rebuild(i, &p);
            stat_strength_red++;
        }
    }
}

/* ---- prop / copy maps (shared infrastructure) ---- */
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

/* when entering a loop, any var that gets reassigned inside the
   loop body is no longer known to be a constant. drop those
   entries but keep ones that aren't modified in the loop — those
   are still propagatable through the loop. */
static void prop_invalidate_loop_writes(int loop_start, int loop_end) {
    int i = 0;
    while (i < prop_count) {
        if (var_defined_in_range(prop_map[i].name, loop_start, loop_end)) {
            prop_map[i] = prop_map[--prop_count];
        } else {
            i++;
        }
    }
}

static void constant_propagation(void) {
    int i;
    prop_clear();
    find_jump_targets();
    scan_loops();

    for (i = 0; i < opt_len; i++) {
        /* loop entry: invalidate only vars that get written in the loop */
        if (loop_head[i]) {
            prop_invalidate_loop_writes(i, loop_end_of[i]);
        } else if (jump_target[i]) {
            /* non-loop jump target: stay conservative, clear everything */
            prop_clear();
        }

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

/* ---- PASS: copy propagation (local, loop-aware) ---- */
#define MAX_COPIES 512
typedef struct { char from[64]; char to[64]; } CopyEntry;
static CopyEntry copy_map[MAX_COPIES];
static int copy_count = 0;

static void copy_set(const char *from, const char *to) {
    int i;
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

static void copy_invalidate_loop_writes(int loop_start, int loop_end) {
    int i = 0;
    while (i < copy_count) {
        if (var_defined_in_range(copy_map[i].from, loop_start, loop_end) ||
            var_defined_in_range(copy_map[i].to,   loop_start, loop_end)) {
            copy_map[i] = copy_map[--copy_count];
        } else {
            i++;
        }
    }
}

static void copy_propagation(void) {
    int i;
    copy_clear();
    find_jump_targets();
    scan_loops();

    for (i = 0; i < opt_len; i++) {
        if (loop_head[i]) {
            copy_invalidate_loop_writes(i, loop_end_of[i]);
        } else if (jump_target[i]) {
            copy_clear();
        }

        if (opt_buf[i].type == INS_IF) {
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

        parse_tac(opt_buf[i].text, &p);
        copy_invalidate(p.dest);

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

/* ---- PASS: dead code elimination ---- */
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

/* ---- PASS: global constant propagation ---- */
static void global_constant_propagation(void) {
    int changed = 1;
    while (changed) {
        changed = 0;
        prop_clear();
        scan_loops();

        int i;
        for (i = 0; i < opt_len; i++) {
            if (loop_head[i]) {
                prop_invalidate_loop_writes(i, loop_end_of[i]);
            }

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

            parse_tac(opt_buf[i].text, &p);
            if (p.kind == 1 && is_number(p.src1)) {
                prop_set(p.dest, p.src1);
            } else {
                prop_invalidate(p.dest);
            }
        }
    }
}

/* ---- PASS: constant-condition folding ----
   if both operands of INS_IF are numeric, evaluate the compare
   at compile time. true → unconditional goto; false → delete. */
static int eval_cond(long a, const char *op, long b) {
    if (strcmp(op, "<")  == 0) return a <  b;
    if (strcmp(op, ">")  == 0) return a >  b;
    if (strcmp(op, "<=") == 0) return a <= b;
    if (strcmp(op, ">=") == 0) return a >= b;
    if (strcmp(op, "==") == 0) return a == b;
    if (strcmp(op, "!=") == 0) return a != b;
    return -1;
}

static void fold_constant_conditions(void) {
    int alive[MAX_CODE];
    int remap[MAX_CODE];
    int i, j;

    for (i = 0; i < opt_len; i++) alive[i] = 1;

    for (i = 0; i < opt_len; i++) {
        if (opt_buf[i].type != INS_IF) continue;

        char s1[64], op[8], s2[64];
        if (sscanf(opt_buf[i].text, "%63s %7s %63s", s1, op, s2) != 3) continue;
        if (!is_number(s1) || !is_number(s2)) continue;

        int r = eval_cond(atol(s1), op, atol(s2));
        if (r < 0) continue;

        if (r == 1) {
            /* always true: turn the conditional into an unconditional jump */
            opt_buf[i].type = INS_GOTO;
            opt_buf[i].text[0] = '\0';
            stat_cond_fold++;
        } else {
            /* always false: drop it entirely, control falls through */
            alive[i] = 0;
            stat_cond_fold++;
        }
    }

    int removed = 0;
    for (i = 0; i < opt_len; i++) if (!alive[i]) removed++;
    if (removed == 0) return;

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

/* ---- PASS: loop-invariant code motion (recursive) ---- */
static int loop_invariant_motion(void) {
    find_jump_targets();

    int i;
    for (i = 0; i < opt_len; i++) {
        if (opt_buf[i].type != INS_GOTO) continue;
        int loop_start = opt_buf[i].target;
        if (loop_start < 0 || loop_start >= i) continue;
        int loop_end = i;

        char defined_in_loop[256][64];
        int def_count = 0;
        int j;
        for (j = loop_start; j <= loop_end && def_count < 256; j++) {
            if (opt_buf[j].type != INS_TEXT) continue;
            ParsedTAC p;
            parse_tac(opt_buf[j].text, &p);
            if (p.kind >= 1 && p.dest[0] != '\0') {
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

        for (j = loop_start; j <= loop_end; j++) {
            if (opt_buf[j].type != INS_TEXT) continue;
            if (jump_target[j]) continue;

            ParsedTAC p;
            parse_tac(opt_buf[j].text, &p);
            if (p.kind == 0) continue;
            if (!is_temp(p.dest)) continue;

            int invariant = 1;
            int k;
            for (k = 0; k < def_count; k++) {
                if (p.kind >= 1 && strcmp(p.src1, defined_in_loop[k]) == 0) { invariant = 0; break; }
                if (p.kind == 2 && strcmp(p.src2, defined_in_loop[k]) == 0) { invariant = 0; break; }
            }
            if (!invariant) continue;

            int def_times = 0;
            for (k = loop_start; k <= loop_end; k++) {
                if (opt_buf[k].type != INS_TEXT) continue;
                ParsedTAC q;
                parse_tac(opt_buf[k].text, &q);
                if (q.kind >= 1 && strcmp(q.dest, p.dest) == 0) def_times++;
            }
            if (def_times != 1) continue;

            Instruction saved = opt_buf[j];
            for (k = j; k > loop_start; k--)
                opt_buf[k] = opt_buf[k - 1];
            opt_buf[loop_start] = saved;

            for (k = 0; k < opt_len; k++) {
                if (opt_buf[k].type == INS_IF || opt_buf[k].type == INS_GOTO) {
                    int t = opt_buf[k].target;
                    if (t >= loop_start && t <= j) {
                        opt_buf[k].target = t + 1;
                    }
                }
            }

            stat_loop_inv++;
            return 1;
        }
    }
    return 0;
}

/* ---- PASS: unreachable code elimination ---- */
static void unreachable_code_elim(void) {
    int reachable[MAX_CODE];
    int remap[MAX_CODE];
    int i, j;

    memset(reachable, 0, sizeof(int) * opt_len);

    int stack[MAX_CODE];
    int sp = 0;
    stack[sp++] = 0;

    while (sp > 0) {
        int idx = stack[--sp];
        if (idx < 0 || idx >= opt_len) continue;
        if (reachable[idx]) continue;
        reachable[idx] = 1;

        if (opt_buf[idx].type == INS_GOTO) {
            if (opt_buf[idx].target >= 0)
                stack[sp++] = opt_buf[idx].target;
        } else if (opt_buf[idx].type == INS_IF) {
            if (opt_buf[idx].target >= 0)
                stack[sp++] = opt_buf[idx].target;
            stack[sp++] = idx + 1;
        } else {
            stack[sp++] = idx + 1;
        }
    }

    int removed = 0;
    for (i = 0; i < opt_len; i++)
        if (!reachable[i]) removed++;

    if (removed == 0) return;

    j = 0;
    for (i = 0; i < opt_len; i++) {
        if (reachable[i]) {
            remap[i] = j;
            if (j != i) opt_buf[j] = opt_buf[i];
            j++;
        }
    }
    for (i = opt_len - 1; i >= 0; i--) {
        if (!reachable[i]) {
            int k = i + 1;
            while (k < opt_len && !reachable[k]) k++;
            remap[i] = (k < opt_len) ? remap[k] : j;
        }
    }
    for (i = 0; i < j; i++) {
        if ((opt_buf[i].type == INS_IF || opt_buf[i].type == INS_GOTO) &&
            opt_buf[i].target >= 0 && opt_buf[i].target < opt_len) {
            opt_buf[i].target = remap[opt_buf[i].target];
        }
    }

    stat_unreachable += removed;
    opt_len = j;
}

/* ---- total stats, used to detect when the fixed-point loop is done ---- */
static int total_stats(void) {
    return stat_const_fold + stat_const_prop + stat_cse + stat_dead_elim +
           stat_algebraic + stat_copy_prop + stat_strength_red +
           stat_global_const + stat_cond_fold + stat_unreachable;
}


/* ---- PASS: loop unrolling ----
   Detect small loops with constant bounds and duplicate their body. */


/* ---- PASS: loop unrolling ---- */
static int loop_unroll(void) {
    int changed = 0;
    scan_loops();
    int i;
    for (i = 0; i < opt_len; i++) {
        if (!loop_head[i]) continue;
        int head = i;
        int end = loop_end_of[i];
        if (opt_buf[head].type != INS_IF || opt_buf[head+1].type != INS_GOTO) continue;
        
        int break_target = opt_buf[head+1].target;
        char var[64], op[8], limit_str[64];
        if (sscanf(opt_buf[head].text, "%63s %7s %63s", var, op, limit_str) != 3) continue;
        if (!is_number(limit_str)) continue;
        long limit = atol(limit_str);
        
        long start_val = 0;
        int init_idx = -1;
        int j;
        for (j = head - 1; j >= 0; j--) {
            if (opt_buf[j].type == INS_IF || opt_buf[j].type == INS_GOTO) break;
            if (opt_buf[j].type == INS_TEXT) {
                char d[64], s[64];
                if (sscanf(opt_buf[j].text, "%63s = %63s", d, s) == 2 && strcmp(d, var) == 0 && is_number(s)) {
                    start_val = atol(s);
                    init_idx = j;
                    break;
                }
            }
        }
        if (init_idx == -1) continue;
        
        long step = 0;
        int inc_idx = -1;
        for (j = head+2; j < end; j++) {
            char t1[64], s1[64], opp[8], s2[64], dest[64];
            if (sscanf(opt_buf[j].text, "%63s = %63s %7s %63s", t1, s1, opp, s2) == 4 && strcmp(opp, "+") == 0 && strcmp(s1, var) == 0 && is_number(s2)) {
                step = atol(s2);
                if (j + 1 < end && sscanf(opt_buf[j+1].text, "%63s = %63s", dest, s1) == 2 && strcmp(s1, t1) == 0 && strcmp(dest, var) == 0) {
                    inc_idx = j;
                    break;
                }
            }
        }
        if (inc_idx == -1 || step <= 0) continue;
        
        long iters = 0, cur = start_val;
        if (strcmp(op, "<") == 0) while (cur < limit) { cur += step; iters++; }
        else if (strcmp(op, "<=") == 0) while (cur <= limit) { cur += step; iters++; }
        else continue;
        
        if (iters == 0 || iters > 15) continue;
        
        Instruction new_body[MAX_CODE];
        int nlen = 0;
        long iter_val = start_val;
        int k;
        for (k = 0; k < iters; k++) {
            snprintf(new_body[nlen].text, 256, "%s = %ld", var, iter_val);
            new_body[nlen].type = INS_TEXT; new_body[nlen].target = -1; nlen++;
            for (j = head+2; j < end; j++) {
                if (j == inc_idx || j == inc_idx+1) continue;
                new_body[nlen] = opt_buf[j];
                if (new_body[nlen].target >= head+2 && new_body[nlen].target < end) {
                    new_body[nlen].target += nlen - (j - head - 2);
                }
                nlen++;
            }
            iter_val += step;
        }
        snprintf(new_body[nlen].text, 256, "%s = %ld", var, iter_val);
        new_body[nlen].type = INS_TEXT; new_body[nlen].target = -1; nlen++;
        
        int diff = nlen - (end - head + 1);
        if (opt_len + diff >= MAX_CODE) continue;
        
        if (diff != 0) {
            memmove(&opt_buf[end + 1 + diff], &opt_buf[end + 1], sizeof(Instruction) * (opt_len - end - 1));
            for (j = 0; j < opt_len + diff; j++) {
                if (j >= head && j < head + nlen) continue;
                if (opt_buf[j].target > end) opt_buf[j].target += diff;
                else if (opt_buf[j].target >= head && opt_buf[j].target <= end) opt_buf[j].target = break_target > end ? break_target + diff : break_target;
            }
        }
        
        memcpy(&opt_buf[head], new_body, sizeof(Instruction) * nlen);
        opt_len += diff;
        stat_loop_unroll++;
        changed = 1;
        break; // break and rescan
    }
    return changed;
}

/* ---- PASS: loop jamming ---- */
static int loop_jam(void) {
    int changed = 0;
    scan_loops();
    int i;
    for (i = 0; i < opt_len; i++) {
        if (!loop_head[i]) continue;
        int e1 = loop_end_of[i];
        if (opt_buf[i].type != INS_IF || opt_buf[i+1].type != INS_GOTO) continue;
        int jump1 = opt_buf[i+1].target;
        if (jump1 != e1 + 1) continue;
        
        // Find next loop
        int j = jump1;
        int init2 = -1;
        if (j < opt_len && opt_buf[j].type == INS_TEXT) { init2 = j; j++; }
        if (j >= opt_len || !loop_head[j]) continue;
        
        int e2 = loop_end_of[j];
        if (opt_buf[j].type != INS_IF || opt_buf[j+1].type != INS_GOTO) continue;
        int jump2 = opt_buf[j+1].target;
        if (jump2 != e2 + 1) continue;
        
        // Check identical conditions
        if (strcmp(opt_buf[i].text, opt_buf[j].text) != 0) {
            // Might have different loop index vars but same bounds.
            char v1[64], op1[8], l1[64], v2[64], op2[8], l2[64];
            if (sscanf(opt_buf[i].text, "%63s %7s %63s", v1, op1, l1) == 3 &&
                sscanf(opt_buf[j].text, "%63s %7s %63s", v2, op2, l2) == 3) {
                if (strcmp(op1, op2) != 0 || strcmp(l1, l2) != 0) continue;
            } else continue;
        }
        
        // Perform Jamming/Fusion
        Instruction fbody[MAX_CODE];
        int flen = 0;
        int k;
        
        if (init2 != -1) { fbody[flen++] = opt_buf[init2]; } // init for loop 2 BEFORE loop 1 header
        fbody[flen++] = opt_buf[i];   // if L1
        fbody[flen++] = opt_buf[i+1]; // goto B1
        
        // loop 1 body (except inc)
        for (k = i+2; k < e1; k++) {
            // Find L1 inc: typically last 2 instructions
            if (k >= e1 - 2) break;
            fbody[flen++] = opt_buf[k];
        }
        int l1_inc = k;
        
        // loop 2 body (except inc)
        for (k = j+2; k < e2; k++) {
            if (k >= e2 - 2) break;
            fbody[flen++] = opt_buf[k];
        }
        int l2_inc = k;
        
        for (k = l1_inc; k < e1; k++) fbody[flen++] = opt_buf[k]; // L1 inc
        for (k = l2_inc; k < e2; k++) fbody[flen++] = opt_buf[k]; // L2 inc
        fbody[flen++] = opt_buf[e1]; // goto L1
        
        int total_old = e2 - i + 1;
        int diff = flen - total_old;
        if (diff != 0) {
            memmove(&opt_buf[e2 + 1 + diff], &opt_buf[e2 + 1], sizeof(Instruction) * (opt_len - e2 - 1));
            for (k = 0; k < opt_len + diff; k++) {
                if (k >= i && k < i + flen) continue;
                if (opt_buf[k].target > e2) opt_buf[k].target += diff;
            }
        }
        
        memcpy(&opt_buf[i], fbody, sizeof(Instruction) * flen);
        // Fix up branch targets
        opt_buf[init2 != -1 ? i+1 : i].target = i + flen; // break target
        opt_buf[i + flen - 1].target = init2 != -1 ? i+1 : i; // goto header
        
        opt_len += diff;
        stat_loop_jam++;
        changed = 1;
        break; // restart
    }
    return changed;
}

/* main optimization driver ----
   the core insight: each pass either SUBSTITUTES or EVALUATES.
   substitutions (const prop, copy prop) expose new evaluation
   opportunities (fold, simplify, cond-fold). evaluation in turn
   may dead-code whole branches, exposing more substitution
   opportunities. so we loop the whole pipeline until nothing
   moves — this catches chains like:
     x = 10; y = x;  ->  y = 10  (const prop)
                      ->  nothing new to do for y
     if (x > 5) ...  ->  if (10 > 5)  (const prop)
                     ->  goto L      (cond fold)
                     ->  dead branch dropped  (unreachable)
   running once only catches the first link. */
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
    stat_cond_fold = 0;
    stat_loop_unroll = 0;
    stat_loop_jam = 0;

    int iteration = 0;
    int last_stats = -1;
    int last_len = -1;

    while (iteration < 20) {
        int snap_stats = total_stats();
        int snap_len = opt_len;

        common_subexpr_elim();
        constant_folding();
        algebraic_simplification();
        strength_reduction();
        constant_propagation();
        copy_propagation();
        constant_folding();           /* what prop exposed */
        algebraic_simplification();   /* what folding exposed */
        global_constant_propagation();
        constant_folding();           /* what global prop exposed */
        fold_constant_conditions();   /* decide branches with numeric operands */
        unreachable_code_elim();      /* drop branches that cond-fold made dead */
        dead_code_elim();

        int new_stats = total_stats();
        if (new_stats == snap_stats && opt_len == snap_len) break;

        last_stats = new_stats;
        last_len = opt_len;
        iteration++;
    }

    /* loop-invariant motion runs repeatedly until no changes made */
    while (loop_invariant_motion());
    while (loop_jam());
    while (loop_unroll());
    unreachable_code_elim();
    dead_code_elim();
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