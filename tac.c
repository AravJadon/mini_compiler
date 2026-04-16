#include "common.h"

Instruction code_buf[MAX_CODE];
int code_len = 0;
int temp_count = 1;

char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (!p) { fprintf(stderr, "out of memory\n"); exit(1); }
    memcpy(p, s, n);
    return p;
}

char *mkstr(const char *fmt, ...) {
    char buf[512];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    return xstrdup(buf);
}

AAttr *make_aattr(char *place) {
    AAttr *a = (AAttr *)malloc(sizeof(AAttr));
    a->place = place;
    return a;
}

BAttr *make_battr(IntList *t, IntList *f) {
    BAttr *b = (BAttr *)malloc(sizeof(BAttr));
    b->truelist = t;
    b->falselist = f;
    return b;
}

void free_aattr(AAttr *a) {
    if (!a) return;
    free(a->place);
    free(a);
}

int nextinstr(void) { return code_len; }
int line_of(int idx) { return 100 + idx; }

int emit_text(const char *fmt, ...) {
    char buf[256];
    va_list ap;
    if (code_len >= MAX_CODE) { fprintf(stderr, "too many TAC instructions\n"); exit(1); }
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    code_buf[code_len].type = INS_TEXT;
    strncpy(code_buf[code_len].text, buf, sizeof(code_buf[code_len].text) - 1);
    code_buf[code_len].text[sizeof(code_buf[code_len].text) - 1] = '\0';
    code_buf[code_len].target = -1;
    return code_len++;
}

int emit_if(const char *cond) {
    if (code_len >= MAX_CODE) { fprintf(stderr, "too many TAC instructions\n"); exit(1); }
    code_buf[code_len].type = INS_IF;
    strncpy(code_buf[code_len].text, cond, sizeof(code_buf[code_len].text) - 1);
    code_buf[code_len].text[sizeof(code_buf[code_len].text) - 1] = '\0';
    code_buf[code_len].target = -1;
    return code_len++;
}

int emit_goto(int target) {
    if (code_len >= MAX_CODE) { fprintf(stderr, "too many TAC instructions\n"); exit(1); }
    code_buf[code_len].type = INS_GOTO;
    code_buf[code_len].text[0] = '\0';
    code_buf[code_len].target = target;
    return code_len++;
}

IntList *makelist(int idx) {
    IntList *n = (IntList *)malloc(sizeof(IntList));
    n->index = idx;
    n->next = NULL;
    return n;
}

IntList *merge_list(IntList *a, IntList *b) {
    IntList *p;
    if (!a) return b;
    p = a;
    while (p->next) p = p->next;
    p->next = b;
    return a;
}

void patch_one(int idx, int target) {
    if (idx < 0 || idx >= code_len) return;
    code_buf[idx].target = target;
}

void backpatch(IntList *list, int target) {
    IntList *p = list;
    while (p) { patch_one(p->index, target); p = p->next; }
}

/* frees a backpatch list */
void free_list(IntList *list) {
    IntList *p;
    while (list) {
        p = list;
        list = list->next;
        free(p);
    }
}

char *new_temp(void) {
    char buf[64];
    snprintf(buf, sizeof(buf), "t%d", temp_count++);
    return xstrdup(buf);
}

BAttr *emit_truthy(const char *expr) {
    char *cond = mkstr("%s != 0", expr);
    int i = emit_if(cond);
    int j = emit_goto(-1);
    free(cond);
    return make_battr(makelist(i), makelist(j));
}

BAttr *emit_relop(const char *lhs, const char *op, const char *rhs) {
    char *cond = mkstr("%s %s %s", lhs, op, rhs);
    int i = emit_if(cond);
    int j = emit_goto(-1);
    free(cond);
    return make_battr(makelist(i), makelist(j));
}

void emit_compound_assign(const char *id, const char *op, AAttr *rhs) {
    char *t = new_temp();
    emit_text("%s = %s %s %s", t, id, op, rhs->place);
    emit_text("%s = %s", id, t);
    free(t);
}

void emit_incdec(const char *id, int is_inc) {
    char *t = new_temp();
    emit_text("%s = %s %c 1", t, id, is_inc ? '+' : '-');
    emit_text("%s = %s", id, t);
    free(t);
}

void emit_bool_assignment(const char *id, BAttr *b) {
    char *t = new_temp();
    int false_lbl = nextinstr();
    emit_text("%s = 0", t);
    int jump_end = emit_goto(-1);
    int true_lbl = nextinstr();
    emit_text("%s = 1", t);
    int end_lbl = nextinstr();
    emit_text("%s = %s", id, t);
    backpatch(b->truelist, true_lbl);
    backpatch(b->falselist, false_lbl);
    patch_one(jump_end, end_lbl);
    free(t);
}

/* ---- break/continue label stacks ----
   each loop and each switch pushes a frame on entry. break/continue
   record the index of their emit_goto(-1), and those get backpatched
   when the frame pops with a known end/header target.

   switch only needs the break stack (no continue target).
   loops need both: break jumps past the loop, continue jumps
   to the loop header. */

#define MAX_CTRL_DEPTH 64

typedef struct {
    IntList *pending;    /* list of emit_goto(-1) indices waiting for a target */
} CtrlFrame;

static CtrlFrame break_stack[MAX_CTRL_DEPTH];
static int break_sp = 0;
static CtrlFrame continue_stack[MAX_CTRL_DEPTH];
static int continue_sp = 0;

void breaklist_push(void) {
    if (break_sp >= MAX_CTRL_DEPTH) return;
    break_stack[break_sp].pending = NULL;
    break_sp++;
}

void breaklist_add(int goto_idx) {
    if (break_sp <= 0) {
        /* nothing to break out of — leave the goto unpatched, optimizer
           / codegen will flag it. happens only on bad source code. */
        return;
    }
    IntList *n = makelist(goto_idx);
    break_stack[break_sp - 1].pending =
        merge_list(break_stack[break_sp - 1].pending, n);
}

void breaklist_pop(int end_target) {
    if (break_sp <= 0) return;
    CtrlFrame *f = &break_stack[--break_sp];
    backpatch(f->pending, end_target);
    free_list(f->pending);
    f->pending = NULL;
}

void continuelist_push(void) {
    if (continue_sp >= MAX_CTRL_DEPTH) return;
    continue_stack[continue_sp].pending = NULL;
    continue_sp++;
}

void continuelist_add(int goto_idx) {
    if (continue_sp <= 0) return;
    IntList *n = makelist(goto_idx);
    continue_stack[continue_sp - 1].pending =
        merge_list(continue_stack[continue_sp - 1].pending, n);
}

void continuelist_pop(int loop_head_target) {
    if (continue_sp <= 0) return;
    CtrlFrame *f = &continue_stack[--continue_sp];
    backpatch(f->pending, loop_head_target);
    free_list(f->pending);
    f->pending = NULL;
}

void print_tac(void) {
    int i;
    for (i = 0; i < code_len; i++) {
        if (code_buf[i].type == INS_TEXT) {
            printf("  %d: %s\n", line_of(i), code_buf[i].text);
        } else if (code_buf[i].type == INS_IF) {
            if (code_buf[i].target < 0)
                printf("  %d: if %s goto ???  ; unpatched\n", line_of(i), code_buf[i].text);
            else
                printf("  %d: if %s goto %d\n", line_of(i), code_buf[i].text, line_of(code_buf[i].target));
        } else {
            if (code_buf[i].target < 0)
                printf("  %d: goto ???  ; unpatched\n", line_of(i));
            else
                printf("  %d: goto %d\n", line_of(i), line_of(code_buf[i].target));
        }
    }
}