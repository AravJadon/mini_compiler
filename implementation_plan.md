# Mini Compiler for Toy-C — Implementation Plan

## Assignment Requirements (Deadline: 14 April 2026)

| # | Phase | Marks | Status |
|---|-------|-------|--------|
| 1 | Lexical Analysis | 10 | ✅ Done |
| 2 | Syntax Analysis | 10 | ✅ Done |
| 3 | Semantic Analysis | 20 | ✅ Done |
| 4 | Intermediate Code (TAC) | 20 | ✅ Done |
| 5 | Basic Optimization | 20 | ❌ Not started |
| 6 | Target Code Generation (Assembly) | 30 | ❌ Not started |
| — | 5 Test Programs | Required | ⚠️ Partially (need clean set) |

---

## What We Have Right Now

### Phase 1 — Lexical Analysis ✅
[lexer.l](file:///c:/Users/Abhas/OneDrive/Desktop/compiler_lab/compiler_projecct/lexer.l) is complete. It handles:
- Keywords: `int`, `if`, `else`, `while`, `for`, `do`, `return`, `break`, `continue`, `switch`, `case`, `default`, `printf`, `scanf`
- Operators: `+`, `-`, `*`, `/`, `%`, `<`, `>`, `<=`, `>=`, `==`, `!=`, `&&`, `||`, `!`, `++`, `--`, `+=`, `-=`, `*=`, `/=`, `%=`
- Literals: integers, floats (as strings), strings
- Identifiers, punctuation, comments (single-line `//`, block `/* */`), preprocessor lines
- Error reporting via `report_lex_error()` for invalid characters

### Phase 2 — Syntax Analysis ✅
[parser.y](file:///c:/Users/Abhas/OneDrive/Desktop/compiler_lab/compiler_projecct/parser.y) has a full Bison grammar covering:
- Function definitions and prototypes
- Declarations with optional initializers
- Assignments (simple, compound `+=` etc.)
- Expressions (arithmetic with proper precedence, boolean with short-circuit)
- Control flow: `if-else` (matched/unmatched with dangling-else resolution), `while`, `do-while`, `for`, `switch-case`
- `break`, `continue`, `return`
- `printf` / `scanf`
- `error` recovery via `error SEMI`

### Phase 3 — Semantic Analysis ✅
Built into the parser actions:
- **Symbol table** (hash table, 1024 buckets): `sym_declare()` / `sym_lookup()`
- **Undeclared variable detection**: checked on every use (assignments, expressions, conditions)
- **Duplicate declaration detection**: checked on every `int x;`
- Reports errors to stderr with line numbers

### Phase 4 — Intermediate Code Generation (TAC) ✅
- Three-address code emitted into `code_buf[]` array during parsing
- Proper backpatching for `if-else`, `while`, `do-while`, boolean short-circuit (`&&`, `||`)
- Temporaries: `t1`, `t2`, ... generated via `new_temp()`
- Output format: `100: x = y + z`, `101: if x < y goto 105`, `102: goto 110`

### Phase 5 — Basic Optimization ❌ NOT IMPLEMENTED
The assignment requires **20 marks** worth of optimizations:
- Common sub-expression elimination (CSE)
- Constant propagation
- Constant folding
- Dead code elimination

### Phase 6 — Target Code Generation (Assembly) ❌ NOT IMPLEMENTED  
The assignment requires **30 marks** — the biggest chunk. Must target a RISC ISA (MIPS subset recommended). Sample from assignment:
```
LOAD R1, b
LOAD R2, c
ADD R1, R1, R2
STORE R1, a
```

---

## Proposed Changes

### Phase 5: Optimization Pass

> [!IMPORTANT]
> This is a **post-TAC pass** — we walk over the `code_buf[]` array after parsing is complete, before printing. No changes to the grammar or lexer needed.

#### [NEW] Optimization functions added to `parser.y` (or a separate `optimizer.c`)

The optimizer will make multiple passes over `code_buf[]`:

1. **Constant Folding** — If a TAC instruction is `t1 = 3 + 5`, replace with `t1 = 8`. Evaluate at compile time when both operands are numeric literals.

2. **Constant Propagation** — If `x = 5` and later `y = x + 3`, substitute to `y = 5 + 3` (which then constant-folds to `y = 8`).

3. **Common Sub-Expression Elimination (CSE)** — If `t1 = a + b` appears, and later `t3 = a + b` with neither `a` nor `b` changed, replace `t3 = a + b` with `t3 = t1`.

4. **Dead Code Elimination** — If a temporary `t5` is assigned but never used anywhere else, remove that instruction.

The optimizer will print a **"Before vs After"** comparison showing what was optimized.

---

### Phase 6: Assembly Code Generation

#### [NEW] `codegen.c` — RISC Assembly Generator

Target ISA: **Simple RISC (MIPS-like subset)** with 8 general-purpose registers (`R0`–`R7`).

**ISA Definition:**
| Instruction | Meaning |
|---|---|
| `LOAD Rd, var` | Load variable from memory into register |
| `STORE Rs, var` | Store register value into memory |
| `LOADI Rd, imm` | Load immediate value |
| `ADD Rd, Rs1, Rs2` | Rd = Rs1 + Rs2 |
| `SUB Rd, Rs1, Rs2` | Rd = Rs1 - Rs2 |
| `MUL Rd, Rs1, Rs2` | Rd = Rs1 * Rs2 |
| `DIV Rd, Rs1, Rs2` | Rd = Rs1 / Rs2 |
| `MOD Rd, Rs1, Rs2` | Rd = Rs1 % Rs2 |
| `CMP Rs1, Rs2` | Compare and set flags |
| `BEQ label` | Branch if equal |
| `BNE label` | Branch if not equal |
| `BLT label` | Branch if less than |
| `BGT label` | Branch if greater than |
| `BLE label` | Branch if ≤ |
| `BGE label` | Branch if ≥ |
| `JMP label` | Unconditional jump |
| `HALT` | Stop execution |

**Register Allocation:** Simple next-available strategy with spill/reload. Track which registers hold which variables; spill least-recently-used if all 8 are occupied.

**Approach:** Walk over the (optimized) TAC array. For each instruction type:
- `x = y op z` → `LOAD R1, y` / `LOAD R2, z` / `OP R0, R1, R2` / `STORE R0, x`
- `if x < y goto L` → `LOAD` both, `CMP`, conditional branch
- `goto L` → `JMP L`

---

### Test Programs

#### [NEW] 5 clean test files

| File | Description |
|---|---|
| `test1.c` | Basic arithmetic: declarations, `+`, `-`, `*`, `/` expressions |
| `test2.c` | If-else with relational ops (`<`, `>`, `==`, `!=`) |
| `test3.c` | While loop — compute power (base^exp) |
| `test4.c` | Nested if-else — triangle classifier |
| `test5.c` | Complex — GCD using subtraction with while loop |

Each test program must show output from **all 6 phases**: tokens → parse tree confirmation → semantic check → TAC → optimized TAC → assembly.

---

## Open Questions

> [!IMPORTANT]
> **Architecture choice:** The plan uses a MIPS-like RISC subset with 8 registers. Are you okay with this, or do you prefer standard MIPS with `$t0`–`$t9`, `$s0`–`$s7` register naming?

> [!IMPORTANT]
> **Output format:** The assignment says "show all intermediate steps and output after every phase." Should we print phase-by-phase in a single run (e.g., `=== PHASE 1: TOKENS ===`, `=== PHASE 2: SYNTAX OK ===`, etc.), or is the current approach (errors printed inline, TAC at the end) sufficient?

> [!IMPORTANT]
> **Code organization:** Should we keep everything in `parser.y` (simpler to compile), or split into separate files (`optimizer.c`, `codegen.c`) and use a Makefile?

---

## Verification Plan

### Automated Tests
```bash
# Build and run all 5 test files
lex lexer.l && bison -d parser.y && gcc lex.yy.c parser.tab.c -w -o compiler
./compiler < test1.c
./compiler < test2.c
./compiler < test3.c
./compiler < test4.c
./compiler < test5.c
```

### Manual Verification
- Verify TAC correctness by hand-tracing each test program
- Verify optimized TAC produces logically equivalent results
- Verify assembly output matches the RISC ISA specification
- Cross-check assembly with the sample in the assignment (`LOAD R1, b` / `LOAD R2, c` / `ADD R1, R1, R2` / `STORE R1, a`)
