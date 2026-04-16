/* HARD TEST 4: Stress test for Constant Folding & Propagation */
int main() {
    int a, b, c, d, e, f;

    /* Direct constant folding */
    a = 3 + 5 * 2;
    b = 100 / 4 - 10;
    c = 7 * 8 + 3 * 2;

    /* Constant propagation chain */
    d = 10;
    e = d + 5;
    f = e * 2;

    /* Mixed: fold then propagate */
    a = 2 + 3;
    b = a + 10;
    c = b * a;

    /* Propagation across if-else */
    d = 42;
    if (d > 0) {
        e = d + 1;
    } else {
        e = d - 1;
    }
}
