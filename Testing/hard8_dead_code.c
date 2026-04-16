/* HARD TEST 8: Dead code elimination stress test */
int main() {
    int a, b, c, d, unused1, unused2, result;
    a = 10;
    b = 20;
    c = 30;

    /* These are dead: unused1 and unused2 never read */
    unused1 = a + b;
    unused2 = a * b * c;

    /* This is live */
    result = a + c;

    /* Dead assignment: d assigned but overwritten before use */
    d = 100;
    d = 200;
    d = a + b + c;

    /* Constant condition: one branch is dead */
    if (1 > 0) {
        result = result + d;
    } else {
        result = 999;
    }

    /* Another dead computation */
    unused1 = result * 2;
    unused2 = unused1 + 5;
}
