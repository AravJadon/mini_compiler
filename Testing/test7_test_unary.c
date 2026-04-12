int main() {
    int i, j, k, a, b, c, d, e, f;

    // ─────────────────────────────────────────────
    // 1. Basic increment / decrement
    // ─────────────────────────────────────────────
    i = 5;
    a = i++;     // post-increment

    i = 5;
    b = ++i;     // pre-increment

    i = 5;
    c = i--;     // post-decrement

    i = 5;
    d = --i;     // pre-decrement


    // ─────────────────────────────────────────────
    // 2. Unary + and -
    // ─────────────────────────────────────────────
    j = 10;
    e = -j;      // unary minus
    f = +j;      // unary plus (should be no-op)


    // ─────────────────────────────────────────────
    // 3. Logical NOT
    // ─────────────────────────────────────────────
    k = 0;
    a = !k;      // should become 1

    k = 5;
    b = !k;      // should become 0


    // ─────────────────────────────────────────────
    // 4. Mixed unary with expressions
    // ─────────────────────────────────────────────
    i = 2;
    j = 3;
    c = -i + j;      // (-i) + j

    d = -(i + j);    // negate full expression


    // ─────────────────────────────────────────────
    // 5. Increment inside expressions
    // ─────────────────────────────────────────────
    i = 1;
    e = i++ + 10;    // use old i

    i = 1;
    f = ++i + 10;    // use new i


    // ─────────────────────────────────────────────
    // 6. Multiple unary ops (stress test)
    // ─────────────────────────────────────────────
    i = 1;
    a = i++ + i++;   // tricky: order matters

    i = 1;
    b = ++i + ++i;   // both pre


    // ─────────────────────────────────────────────
    // 7. Combination of unary operators
    // ─────────────────────────────────────────────
    i = 5;
    c = -(++i);      // increment then negate

    i = 5;
    d = !(i--);      // use old value then decrement

    i = 0;
    e = !(-i);       // nested unary


    return 0;
}