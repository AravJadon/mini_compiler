/* ================================================================
 *  codegen.c  —  PHASE 6: Target Code Generation (Assembly)
 *
 *  Target ISA: Simple RISC (MIPS-like) with 8 registers (R0-R7)
 *
 *  Instruction Set:
 *    LOADI Rd, imm        — Load immediate into register
 *    LOAD  Rd, var         — Load variable from memory into register
 *    STORE Rs, var         — Store register into memory variable
 *    ADD   Rd, Rs1, Rs2    — Rd = Rs1 + Rs2
 *    SUB   Rd, Rs1, Rs2    — Rd = Rs1 - Rs2
 *    MUL   Rd, Rs1, Rs2    — Rd = Rs1 * Rs2
 *    DIV   Rd, Rs1, Rs2    — Rd = Rs1 / Rs2
 *    MOD   Rd, Rs1, Rs2    — Rd = Rs1 % Rs2
 *    CMP   Rs1, Rs2        — Compare Rs1 and Rs2, set flags
 *    BEQ   label           — Branch if equal
 *    BNE   label           — Branch if not equal
 *    BLT   label           — Branch if less than
 *    BGT   label           — Branch if greater than
 *    BLE   label           — Branch if less or equal
 *    BGE   label           — Branch if greater or equal
 *    JMP   label           — Unconditional jump
 *    HALT                  — Stop execution
 *
 *  Register Usage:
 *    R1, R2  — operand loading
 *    R1      — result of computation (stored back to memory)
 *    R0      — reserved for special uses
 * ================================================================ */
#include "common.h"
#include <ctype.h>

/* ── Assembly output buffer ──────────────────────────────── */
#define MAX_ASM 16384
static char asm_lines[MAX_ASM][128];
int asm_count = 0;

static void emit(const char *fmt, ...) {
    if (asm_count >= MAX_ASM) return;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(asm_lines[asm_count], 128, fmt, ap);
    va_end(ap);
    asm_count++;
}

/* ── Helpers ─────────────────────────────────────────────── */
static int gen_is_number(const char *s) {
    if (!s || !*s) return 0;
    if (*s == '-') s++;
    if (!*s) return 0;
    while (*s) { if (!isdigit((unsigned char)*s)) return 0; s++; }
    return 1;
}

/* Load a value (variable or immediate) into register Rn */
static void load_to_reg(int reg, const char *val) {
    if (gen_is_number(val))
        emit("    LOADI R%d, %s", reg, val);
    else
        emit("    LOAD  R%d, %s", reg, val);
}

/* Map TAC arithmetic operator to assembly mnemonic */
static const char *asm_op(const char *op) {
    if (strcmp(op, "+") == 0) return "ADD";
    if (strcmp(op, "-") == 0) return "SUB";
    if (strcmp(op, "*") == 0) return "MUL";
    if (strcmp(op, "/") == 0) return "DIV";
    if (strcmp(op, "%") == 0) return "MOD";
    return "???";
}

/* Map TAC relational operator to branch mnemonic */
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

/* ── Determine which TAC indices are jump targets ────────── */
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

/* ══════════════════════════════════════════════════════════════
 *  Generate assembly from optimized TAC
 * ══════════════════════════════════════════════════════════════ */
void generate_assembly(void) {
    int i;
    asm_count = 0;
    find_labels();

    /* ── Header ──────────────────────────────────────────── */
    emit("; ============================================================");
    emit(";  Target Code: Simple RISC Assembly");
    emit(";  Registers: R0-R7 (R1, R2 used for computation)");
    emit("; ============================================================");
    emit("");

    /* ── Data Section ────────────────────────────────────── */
    emit(".DATA");
    /* Emit all user variables from symbol table */
    for (i = 0; i < SYM_SIZE; i++) {
        SymEntry *e = sym_table[i];
        while (e) {
            emit("    %-20s .WORD 0", e->name);
            e = e->next;
        }
    }
    /* Emit all temporaries used */
    for (i = 1; i < temp_count; i++) {
        emit("    t%-19d .WORD 0", i);
    }
    emit("");

    /* ── Code Section ────────────────────────────────────── */
    emit(".CODE");
    emit("    JMP L%d              ; jump to program start", line_of(0));
    emit("");

    /* ── Generate code for each optimized TAC instruction ── */
    for (i = 0; i < opt_len; i++) {

        /* Emit label if this instruction is a jump target */
        if (label_target[i]) {
            emit("L%d:", line_of(i));
        }

        if (opt_buf[i].type == INS_GOTO) {
            /* ── Unconditional jump ──────────────────────── */
            emit("    JMP   L%d", line_of(opt_buf[i].target));
            emit("");

        } else if (opt_buf[i].type == INS_IF) {
            /* ── Conditional branch: if src1 op src2 goto L ── */
            char s1[64], op[8], s2[64];
            if (sscanf(opt_buf[i].text, "%63s %7s %63s", s1, op, s2) == 3) {
                load_to_reg(1, s1);
                load_to_reg(2, s2);
                emit("    CMP   R1, R2");
                emit("    %-5s L%d", branch_op(op), line_of(opt_buf[i].target));
            } else {
                emit("    ; ERROR: cannot parse condition: %s", opt_buf[i].text);
            }
            emit("");

        } else if (opt_buf[i].type == INS_TEXT) {
            /* ── Text instruction ────────────────────────── */
            char dest[64], rest[256];
            char s1[64], op[8], s2[64];
            const char *text = opt_buf[i].text;

            /* Handle special instructions */
            if (strncmp(text, "return", 6) == 0) {
                emit("    HALT                ; return");
                emit("");
                continue;
            }
            if (strncmp(text, "param", 5) == 0) {
                emit("    ; %s  (parameter passing)", text);
                emit("");
                continue;
            }
            if (strncmp(text, "call", 4) == 0) {
                emit("    ; %s  (function call)", text);
                emit("");
                continue;
            }
            if (strncmp(text, "goto", 4) == 0) {
                emit("    ; %s  (text goto)", text);
                emit("");
                continue;
            }

            /* Parse assignment: dest = rest */
            if (sscanf(text, "%63s = %255[^\n]", dest, rest) != 2) {
                emit("    ; UNHANDLED: %s", text);
                emit("");
                continue;
            }

            /* Try binary: src1 op src2 */
            if (sscanf(rest, "%63s %7s %63s", s1, op, s2) == 3 && is_arith_op(op)) {
                /* dest = src1 op src2 */
                emit("    ; %s", text);
                load_to_reg(1, s1);
                load_to_reg(2, s2);
                emit("    %-5s R1, R1, R2", asm_op(op));
                emit("    STORE R1, %s", dest);
            } else {
                /* dest = src (copy or constant) */
                emit("    ; %s", text);
                sscanf(rest, "%63s", s1);
                load_to_reg(1, s1);
                emit("    STORE R1, %s", dest);
            }
            emit("");
        }
    }

    /* Final HALT if not already emitted */
    emit("    HALT                ; end of program");
    emit("");
    emit("; ============================================================");
    emit(";  End of generated assembly");
    emit("; ============================================================");
}

/* Print generated assembly */
void print_assembly(void) {
    int i;
    for (i = 0; i < asm_count; i++) {
        printf("  %s\n", asm_lines[i]);
    }
}
