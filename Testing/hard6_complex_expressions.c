/* HARD TEST 6: Very complex nested arithmetic expressions */
int main() {
    int a, b, c, d, result;
    a = 5;
    b = 10;
    c = 15;
    d = 20;

    /* Deeply nested parenthesized expressions */
    result = ((a + b) * (c - d)) / (a + 1);

    /* Long chain expression */
    result = a + b - c * d / a + b * c - d + a;

    /* Expression as loop bound with complex body */
    while (result > 0) {
        result = result - (a + b) * 2;
        a = a + 1;
        if (a > c) {
            result = 0;
        }
    }
}
