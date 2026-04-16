#include "common.h"
#include <ctype.h>

#define MAX_ASM 16384
static char asm_lines[MAX_ASM][128];
int asm_count = 0;

/* register allocation results */
RegAssign reg_assigns[MAX_VARS];
int reg_assign_count = 0;

static void emit(const char *fmt, ...) {
    if (asm_count >= MAX_ASM) return;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(asm_lines[asm_count], 128, fmt, ap);
    va_end(ap);
    asm_count++;
}

static int gen_is_number(const char *s) {
    if (!s || !*s) return 0;
    if (*s == '-') s++;
    if (!*s) return 0;
    while (*s) { if (!isdigit((unsigned char)*s)) return 0; s++; }
    return 1;
}

static const char *asm_op(const char *op) {
    if (strcmp(op, "+") == 0) return "ADD";
    if (strcmp(op, "-") == 0) return "SUB";
    if (strcmp(op, "*") == 0) return "MUL";
    if (strcmp(op, "/") == 0) return "DIV";
    if (strcmp(op, "%") == 0) return "MOD";
    return "???";
}

static const char *branch_op(const char *op) {
    if (strcmp(op, "<")  == 0) return "BLT";
    if (strcmp(op, ">")  == 0) return "BGT";
    if (strcmp(op, "<=") == 0) return "BLE";
    if (strcmp(op, ">=") == 0) return "BGE";
    if (strcmp(op, "==") == 0) return "BEQ";
    if (strcmp(op, "!=") == 0) return "BNE";
    return "JMP";
}

static int is_arith_op(const char *s) {
    return (strcmp(s,"+")==0 || strcmp(s,"-")==0 || strcmp(s,"*")==0 ||
            strcmp(s,"/")==0 || strcmp(s,"%")==0);
}

/* ---- variable collection ----
   gather all variable/temp names used in the optimized TAC */

static char all_vars[MAX_VARS][64];
static int var_count = 0;

static int find_var_index(const char *name) {
    int i;
    for (i = 0; i < var_count; i++)
        if (strcmp(all_vars[i], name) == 0) return i;
    return -1;
}

static int add_var(const char *name) {
    if (!name || !name[0]) return -1;
    if (gen_is_number(name)) return -1;
    int idx = find_var_index(name);
    if (idx >= 0) return idx;
    if (var_count >= MAX_VARS) return -1;
    strncpy(all_vars[var_count], name, 63);
    all_vars[var_count][63] = '\0';
    return var_count++;
}

static void collect_vars(void) {
    var_count = 0;
    int i;
    for (i = 0; i < opt_len; i++) {
        if (opt_buf[i].type == INS_TEXT) {
            char dest[64], rest[256], s1[64], op[8], s2[64];
            const char *text = opt_buf[i].text;
            if (strncmp(text, "param", 5) == 0 || strncmp(text, "call", 4) == 0 ||
                strncmp(text, "return", 6) == 0 || strncmp(text, "goto", 4) == 0) {
                /* extract vars from param/return if present */
                if (strncmp(text, "return ", 7) == 0) {
                    add_var(text + 7);
                } else if (strncmp(text, "param ", 6) == 0) {
                    add_var(text + 6);
                }
                continue;
            }
            if (sscanf(text, "%63s = %255[^\n]", dest, rest) == 2) {
                add_var(dest);
                if (sscanf(rest, "%63s %7s %63s", s1, op, s2) == 3 && is_arith_op(op)) {
                    add_var(s1);
                    add_var(s2);
                } else {
                    sscanf(rest, "%63s", s1);
                    add_var(s1);
                }
            }
        } else if (opt_buf[i].type == INS_IF) {
            char s1[64], op[8], s2[64];
            if (sscanf(opt_buf[i].text, "%63s %7s %63s", s1, op, s2) == 3) {
                add_var(s1);
                add_var(s2);
            }
        }
    }
}

/* ---- liveness analysis ----
   for each instruction, compute which vars are live after it.
   work backwards: var is live if it's used before being redefined. */

static int live_out[MAX_CODE][MAX_VARS]; /* live_out[i][v] = 1 if var v is live after instr i */

/* which var is defined/used by each instruction */
static int instr_def[MAX_CODE];  /* index of var defined, or -1 */
static int instr_use[MAX_CODE][2]; /* up to 2 vars used, -1 if none */

static void compute_def_use(void) {
    int i;
    for (i = 0; i < opt_len; i++) {
        instr_def[i] = -1;
        instr_use[i][0] = instr_use[i][1] = -1;

        if (opt_buf[i].type == INS_TEXT) {
            char dest[64], rest[256], s1[64], op[8], s2[64];
            const char *text = opt_buf[i].text;
            if (strncmp(text, "param", 5) == 0 || strncmp(text, "call", 4) == 0 ||
                strncmp(text, "goto", 4) == 0)
                continue;
            if (strncmp(text, "return ", 7) == 0) {
                instr_use[i][0] = find_var_index(text + 7);
                continue;
            }
            if (strncmp(text, "return", 6) == 0) continue;

            if (sscanf(text, "%63s = %255[^\n]", dest, rest) == 2) {
                instr_def[i] = find_var_index(dest);
                if (sscanf(rest, "%63s %7s %63s", s1, op, s2) == 3 && is_arith_op(op)) {
                    instr_use[i][0] = find_var_index(s1);
                    instr_use[i][1] = find_var_index(s2);
                } else {
                    sscanf(rest, "%63s", s1);
                    instr_use[i][0] = find_var_index(s1);
                }
            }
        } else if (opt_buf[i].type == INS_IF) {
            char s1[64], op[8], s2[64];
            if (sscanf(opt_buf[i].text, "%63s %7s %63s", s1, op, s2) == 3) {
                instr_use[i][0] = find_var_index(s1);
                instr_use[i][1] = find_var_index(s2);
            }
        }
        /* INS_GOTO has no def/use */
    }
}

static void liveness_analysis(void) {
    int i, v;

    /* clear everything */
    for (i = 0; i < opt_len; i++)
        for (v = 0; v < var_count; v++)
            live_out[i][v] = 0;

    /* iterate backward until stable */
    int changed = 1;
    while (changed) {
        changed = 0;
        for (i = opt_len - 1; i >= 0; i--) {
            /* live_in = (live_out - def) + use
               successor's live_in feeds into our live_out */

            /* figure out successors */
            int succs[2] = { -1, -1 };
            int nsuccs = 0;

            if (opt_buf[i].type == INS_GOTO) {
                if (opt_buf[i].target >= 0)
                    succs[nsuccs++] = opt_buf[i].target;
            } else if (opt_buf[i].type == INS_IF) {
                if (opt_buf[i].target >= 0)
                    succs[nsuccs++] = opt_buf[i].target;
                if (i + 1 < opt_len)
                    succs[nsuccs++] = i + 1;
            } else {
                if (i + 1 < opt_len)
                    succs[nsuccs++] = i + 1;
            }

            /* live_out[i] = union of live_in[succ] for each successor
               live_in[s] = (live_out[s] - def[s]) + use[s] */
            for (int si = 0; si < nsuccs; si++) {
                int s = succs[si];
                if (s < 0 || s >= opt_len) continue;
                for (v = 0; v < var_count; v++) {
                    /* compute live_in of successor s */
                    int live_in_s = live_out[s][v];
                    if (instr_def[s] == v) live_in_s = 0;
                    if (instr_use[s][0] == v || instr_use[s][1] == v) live_in_s = 1;

                    if (live_in_s && !live_out[i][v]) {
                        live_out[i][v] = 1;
                        changed = 1;
                    }
                }
            }
        }
    }
}

/* ---- interference graph ----
   two vars interfere if they're both live at the same point */

static int interferes[MAX_VARS][MAX_VARS];

static void build_interference_graph(void) {
    int i, u, v;
    memset(interferes, 0, sizeof(interferes));

    for (i = 0; i < opt_len; i++) {
        /* find all vars live at this point */
        for (u = 0; u < var_count; u++) {
            if (!live_out[i][u]) continue;
            for (v = u + 1; v < var_count; v++) {
                if (!live_out[i][v]) continue;
                interferes[u][v] = 1;
                interferes[v][u] = 1;
            }
        }
        /* also: the defined var interferes with everything live-out
           (except itself, which is the standard convention) */
        int d = instr_def[i];
        if (d >= 0) {
            for (v = 0; v < var_count; v++) {
                if (v == d) continue;
                if (live_out[i][v]) {
                    interferes[d][v] = 1;
                    interferes[v][d] = 1;
                }
            }
        }
    }
}

/* ---- graph coloring (simplified Chaitin's) ----
   R0 is reserved, so we have R1-R7 = 7 usable registers.
   vars that can't be colored get spilled. */

#define USABLE_REGS 7

static int var_color[MAX_VARS];   /* register number (1-7) or -1 for spill */

static void graph_coloring(void) {
    int i, v;
    int stack[MAX_VARS];
    int sp = 0;
    int removed[MAX_VARS];
    int degree[MAX_VARS];

    for (v = 0; v < var_count; v++) {
        var_color[v] = -1;
        removed[v] = 0;
        degree[v] = 0;
        for (i = 0; i < var_count; i++)
            if (interferes[v][i]) degree[v]++;
    }

    /* simplify: keep removing nodes with degree < USABLE_REGS */
    int progress = 1;
    while (progress) {
        progress = 0;
        for (v = 0; v < var_count; v++) {
            if (removed[v]) continue;
            if (degree[v] < USABLE_REGS) {
                /* push onto stack and "remove" from graph */
                stack[sp++] = v;
                removed[v] = 1;
                /* decrement neighbors' degree */
                for (i = 0; i < var_count; i++) {
                    if (interferes[v][i] && !removed[i])
                        degree[i]--;
                }
                progress = 1;
            }
        }
    }

    /* anything still not removed needs to be spilled */
    for (v = 0; v < var_count; v++) {
        if (!removed[v]) {
            /* spill: pick the one with highest degree */
            stack[sp++] = v;
            removed[v] = 1;
            var_color[v] = -1; /* will stay spilled */
            for (i = 0; i < var_count; i++) {
                if (interferes[v][i] && !removed[i])
                    degree[i]--;
            }
        }
    }

    /* select: pop from stack, assign lowest available color */
    while (sp > 0) {
        v = stack[--sp];
        int used_colors[NUM_REGS];
        memset(used_colors, 0, sizeof(used_colors));

        /* check which colors neighbors are using */
        for (i = 0; i < var_count; i++) {
            if (interferes[v][i] && var_color[i] >= 0)
                used_colors[var_color[i]] = 1;
        }

        /* pick the lowest free color from R1-R7 */
        int color = -1;
        for (i = 1; i < NUM_REGS; i++) {
            if (!used_colors[i]) { color = i; break; }
        }
        var_color[v] = color; /* -1 if all taken, meaning spilled */
    }

    /* build the output table */
    reg_assign_count = 0;
    for (v = 0; v < var_count; v++) {
        strncpy(reg_assigns[reg_assign_count].name, all_vars[v], 63);
        reg_assigns[reg_assign_count].reg = var_color[v];
        reg_assign_count++;
    }
}

/* look up what register a var got, -1 if spilled */
static int get_reg(const char *name) {
    int idx = find_var_index(name);
    if (idx < 0) return -1;
    return var_color[idx];
}

static int reg_is_live_out(int instr_idx, int reg) {
    int v;
    for (v = 0; v < var_count; v++) {
        if (var_color[v] == reg && live_out[instr_idx][v])
            return 1;
    }
    return 0;
}

static int reg_is_avoided(int reg, int avoid1, int avoid2, int avoid3) {
    return reg == avoid1 || reg == avoid2 || reg == avoid3;
}

/* ---- scratch register selection ----
   when we need to stage a value for a binary op, we can't just grab
   R1 and R2 unconditionally — they might be the home of a variable
   that is still live across this instruction. picking a live reg
   as a scratch corrupts that variable silently.

   pick_scratch finds a register in [1..NUM_REGS) that is not the
   home of anything currently live-out of instruction `instr_idx`
   and not in `avoid` (used when we need two different scratches). */
static int pick_scratch(int instr_idx, int avoid1, int avoid2, int avoid3) {
    int r;
    for (r = 1; r < NUM_REGS; r++) {
        if (reg_is_avoided(r, avoid1, avoid2, avoid3)) continue;
        if (!reg_is_live_out(instr_idx, r)) return r;
    }
    /* everything is live — fall back to R1/R2 and accept that
       this is already a very crowded instruction. in practice
       the graph coloring would have spilled some of these. */
    for (r = 1; r < NUM_REGS; r++) {
        if (!reg_is_avoided(r, avoid1, avoid2, avoid3)) return r;
    }
    return 1;
}

/* load a value into a scratch register, but if the var has an allocated
   register, just return that register number instead */
static int load_operand(const char *val, int scratch) {
    if (gen_is_number(val)) {
        emit("    LOADI R%d, %s", scratch, val);
        return scratch;
    }
    int r = get_reg(val);
    if (r >= 0) {
        /* it's already in a register, just use it */
        return r;
    }
    /* spilled: load from memory into scratch */
    emit("    LOAD  R%d, %s", scratch, val);
    return scratch;
}

/* store result: if dest has a register, it's already there.
   if spilled, store from reg to memory. */
static void store_result(const char *dest, int reg) {
    int r = get_reg(dest);
    if (r >= 0) {
        /* dest lives in a register */
        if (r != reg)
            emit("    ADD   R%d, R%d, R0    ; move R%d -> R%d", r, reg, reg, r);
    } else {
        /* spilled: write to memory */
        emit("    STORE R%d, %s", reg, dest);
    }
}

/* ---- label tracking ---- */
static int label_target[MAX_CODE];

static void find_labels(void) {
    int i;
    memset(label_target, 0, sizeof(int) * opt_len);
    for (i = 0; i < opt_len; i++) {
        if ((opt_buf[i].type == INS_IF || opt_buf[i].type == INS_GOTO) &&
            opt_buf[i].target >= 0 && opt_buf[i].target < opt_len) {
            label_target[opt_buf[i].target] = 1;
        }
    }
}

/* ---- main code generation ---- */
void generate_assembly(void) {
    int i;
    asm_count = 0;

    /* run the register allocation pipeline */
    collect_vars();
    compute_def_use();
    liveness_analysis();
    build_interference_graph();
    graph_coloring();
    find_labels();

    /* header */
    emit("; target: Simple RISC Assembly (8 registers R0-R7)");
    emit("; R0 = zero/reserved, R1-R7 assigned by graph coloring");
    emit("");

    /* data section — only spilled vars need memory */
    emit(".DATA");
    for (i = 0; i < var_count; i++) {
        if (var_color[i] < 0) {
            /* spilled to memory */
            emit("    %-20s .WORD 0", all_vars[i]);
        }
    }
    emit("");

    /* code section */
    emit(".CODE");
    emit("    JMP   L%d", line_of(0));
    emit("");

    for (i = 0; i < opt_len; i++) {

        if (label_target[i])
            emit("L%d:", line_of(i));

        if (opt_buf[i].type == INS_GOTO) {
            if (opt_buf[i].target >= 0)
                emit("    JMP   L%d", line_of(opt_buf[i].target));
            else
                emit("    JMP   ???           ; unpatched");
            emit("");

        } else if (opt_buf[i].type == INS_IF) {
            char s1[64], op[8], s2[64];
            if (sscanf(opt_buf[i].text, "%63s %7s %63s", s1, op, s2) == 3) {
                /* for conditions the two scratches need to be different,
                   and neither can clobber a live register */
                int sc1 = pick_scratch(i, -1, -1, -1);
                int r1 = load_operand(s1, sc1);
                int sc2 = pick_scratch(i, sc1, r1, -1);
                int r2 = load_operand(s2, sc2);
                emit("    CMP   R%d, R%d", r1, r2);
                emit("    %-5s L%d", branch_op(op), line_of(opt_buf[i].target));
            } else {
                emit("    ; error: can't parse condition: %s", opt_buf[i].text);
            }
            emit("");

        } else if (opt_buf[i].type == INS_TEXT) {
            char dest[64], rest[256], s1[64], op[8], s2[64];
            const char *text = opt_buf[i].text;

            /* return with value: load into R0 then halt */
            if (strncmp(text, "return ", 7) == 0) {
                const char *retval = text + 7;
                int sc = pick_scratch(i, -1, -1, -1);
                int r = load_operand(retval, sc);
                if (r != 0)
                    emit("    ADD   R0, R%d, R0    ; return value", r);
                emit("    HALT");
                emit("");
                continue;
            }
            if (strncmp(text, "return", 6) == 0) {
                emit("    HALT");
                emit("");
                continue;
            }
            if (strncmp(text, "param", 5) == 0) {
                emit("    ; %s", text);
                emit("");
                continue;
            }
            if (strncmp(text, "call", 4) == 0) {
                emit("    ; %s", text);
                emit("");
                continue;
            }
            if (strncmp(text, "goto", 4) == 0) {
                emit("    ; %s", text);
                emit("");
                continue;
            }

            if (sscanf(text, "%63s = %255[^\n]", dest, rest) != 2) {
                emit("    ; unhandled: %s", text);
                emit("");
                continue;
            }

            if (sscanf(rest, "%63s %7s %63s", s1, op, s2) == 3 && is_arith_op(op)) {
                /* dest = s1 op s2 */
                emit("    ; %s", text);

                /* pick scratches that don't step on anything live.
                   for the two operand loads we use different scratches
                   so the second load doesn't overwrite the first. */
                int sc1 = pick_scratch(i, -1, -1, -1);
                int r1 = load_operand(s1, sc1);
                int sc2 = pick_scratch(i, sc1, r1, -1);
                int r2 = load_operand(s2, sc2);
                int rd = get_reg(dest);

                /* target_reg = where the arithmetic result actually
                   gets written. if dest has a home register, we'd
                   like to write directly there — UNLESS that register
                   currently holds a source operand that's still live
                   past this instruction. writing to rd early would
                   destroy that value before we're done reading it.
                   when that collision happens, stage through a fresh
                   scratch and copy into rd afterward. */
                int target_reg;
                int need_move = 0;

                if (rd >= 0) {
                    int collides = 0;
                    /* does rd alias a live operand register? */
                    if (rd == r1 || rd == r2) {
                        int v;
                        for (v = 0; v < var_count; v++) {
                            if (var_color[v] != rd) continue;
                            if (!live_out[i][v]) continue;
                            if (instr_def[i] == v) continue;  /* rd's own var is fine */
                            collides = 1;
                            break;
                        }
                    }
                    if (collides) {
                        target_reg = pick_scratch(i, r1, r2, rd);
                        /* pick_scratch may return sc1 or sc2; make sure
                           we get something distinct so the write lands
                           somewhere safe */
                        if (target_reg == r1 || target_reg == r2) {
                            int r;
                            for (r = 1; r < NUM_REGS; r++) {
                                if (r != r1 && r != r2 && r != rd) {
                                    target_reg = r;
                                    break;
                                }
                            }
                        }
                        need_move = 1;
                    } else {
                        target_reg = rd;
                    }
                } else {
                    /* spilled dest — compute into a scratch then STORE */
                    target_reg = pick_scratch(i, r1, r2, -1);
                    if (target_reg == r1 || target_reg == r2) {
                        int r;
                        for (r = 1; r < NUM_REGS; r++) {
                            if (r != r1 && r != r2) { target_reg = r; break; }
                        }
                    }
                }

                emit("    %-5s R%d, R%d, R%d", asm_op(op), target_reg, r1, r2);

                if (need_move) {
                    /* copy the staged result into the var's home register */
                    emit("    ADD   R%d, R%d, R0    ; move R%d -> R%d",
                         rd, target_reg, target_reg, rd);
                }
                if (rd < 0)
                    emit("    STORE R%d, %s", target_reg, dest);
            } else {
                /* dest = src (copy or constant) */
                emit("    ; %s", text);
                sscanf(rest, "%63s", s1);
                int sc = pick_scratch(i, get_reg(dest), -1, -1);
                int rs = load_operand(s1, sc);
                int rd = get_reg(dest);
                if (rd >= 0) {
                    if (rs != rd)
                        emit("    ADD   R%d, R%d, R0    ; move", rd, rs);
                    /* else: already in the right register, nothing to do */
                } else {
                    emit("    STORE R%d, %s", rs, dest);
                }
            }
            emit("");
        }
    }

    /* final halt just in case */
    emit("    HALT");
    emit("");
    emit("; end of generated assembly");
}

void print_assembly(void) {
    int i;
    for (i = 0; i < asm_count; i++)
        printf("  %s\n", asm_lines[i]);
}

/* prints the register allocation table */
void print_reg_allocation(void) {
    int i;
    printf("  Register Allocation (Graph Coloring):\n");
    printf("  %-20s %s\n", "Variable", "Register");
    printf("  %-20s %s\n", "--------", "--------");
    for (i = 0; i < reg_assign_count; i++) {
        if (reg_assigns[i].reg >= 0)
            printf("  %-20s R%d\n", reg_assigns[i].name, reg_assigns[i].reg);
        else
            printf("  %-20s [spilled]\n", reg_assigns[i].name);
    }
    printf("\n");
}

/* prints a summary of the liveness info */
void print_liveness_info(void) {
    int i, v, first;
    printf("  Liveness (live-out per instruction):\n");
    for (i = 0; i < opt_len; i++) {
        printf("    %d: { ", line_of(i));
        first = 1;
        for (v = 0; v < var_count; v++) {
            if (live_out[i][v]) {
                if (!first) printf(", ");
                printf("%s", all_vars[v]);
                first = 0;
            }
        }
        printf(" }\n");
    }
    printf("\n");

    printf("  Interference Graph:\n");
    for (i = 0; i < var_count; i++) {
        int has_edge = 0;
        for (v = 0; v < var_count; v++)
            if (interferes[i][v]) has_edge = 1;
        if (!has_edge) continue;

        printf("    %s -- ", all_vars[i]);
        first = 1;
        for (v = 0; v < var_count; v++) {
            if (interferes[i][v]) {
                if (!first) printf(", ");
                printf("%s", all_vars[v]);
                first = 0;
            }
        }
        printf("\n");
    }
    printf("\n");
}
