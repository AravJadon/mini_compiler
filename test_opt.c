/* Test program to demonstrate all 4 optimizations */
int main() {
    int a;
    int b;
    int c;
    int d;
    int result;

    /* Constant Folding: 3 + 5 should become 8 */
    a = 3 + 5;

    /* Constant Propagation: b should use 10 directly */
    b = 10;
    c = b + 2;

    /* Common Sub-Expression: a + b computed twice */
    d = a + b;
    result = a + b;

    /* Dead Code: temp from unused expression */
    if (d > 0) {
        result = d + c;
    } else {
        result = c;
    }
}
