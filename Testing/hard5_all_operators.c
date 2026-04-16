/* HARD TEST 5: Every required operator used together */
int main() {
    int a, b, c, d, e, f, g, h;
    a = 100;
    b = 37;

    /* All arithmetic operators */
    c = a + b;
    d = a - b;
    e = a * b;
    f = a / b;

    /* Chain all operators in one expression */
    g = a + b * c - d / e;

    /* All relational operators controlling flow */
    if (a < b) {
        h = 1;
    }
    if (a > b) {
        h = 2;
    }
    if (a == b) {
        h = 3;
    }
    if (a != b) {
        h = 4;
    }

    /* Relational inside loop condition */
    while (g > 0) {
        g = g - 1;
    }
}
