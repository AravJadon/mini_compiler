/* HARD TEST 3: Stress test for Common Sub-Expression Elimination */
int main() {
    int a, b, c, d, e, f, g;
    a = 10;
    b = 20;

    /* Same sub-expression a + b repeated many times */
    c = a + b;
    d = a + b;
    e = a + b + c;

    /* Same sub-expression a * b repeated */
    f = a * b;
    g = a * b;

    /* Nested common sub-expressions */
    c = a + b - a * b;
    d = a + b - a * b;

    /* After reassignment, CSE should NOT apply */
    a = 50;
    e = a + b;
}
