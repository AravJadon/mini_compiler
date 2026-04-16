/* HARD TEST 9: Many variables, register pressure stress test */
int main() {
    int a, b, c, d, e, f, g, h;
    int i, j, k, m, n, p, q, r;
    int result;

    a = 1; b = 2; c = 3; d = 4;
    e = 5; f = 6; g = 7; h = 8;
    i = 9; j = 10; k = 11; m = 12;
    n = 13; p = 14; q = 15; r = 16;

    /* Use all variables simultaneously */
    result = a + b + c + d + e + f + g + h;
    result = result + i + j + k + m + n + p + q + r;

    /* Cross-variable dependencies */
    a = b + c;
    b = c + d;
    c = d + e;
    d = e + f;
    e = f + g;
    f = g + h;
    g = h + i;
    h = i + j;

    result = a + b + c + d + e + f + g + h;

    /* Conditional with many live variables */
    if (result > 100) {
        a = a + b;
        c = c + d;
        e = e + f;
        g = g + h;
    } else {
        b = a - b;
        d = c - d;
        f = e - f;
        h = g - h;
    }
}
