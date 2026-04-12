int main() {
    int a;
    int b;
    int c;
    int d;
    int e;
    int result;

    a = 10;
    b = 3;
    c = a + b;
    d = a - b;
    e = c * d;
    result = e / b;

    /* constant folding should catch this */
    int x;
    x = 4 + 8;

    /* algebraic simplification targets */
    int y;
    y = a * 1;
    y = a + 0;
    y = a - 0;

    /* strength reduction: multiply by 2 */
    int z;
    z = a * 2;

    /* all operators in one expression */
    int big;
    big = a + b * c - d / e;

    return 0;
}