int main() {
    int i;
    int sum;
    int n;

    /* simple while loop: sum 1 to 10 */
    n = 10;
    sum = 0;
    i = 1;
    while (i < n + 1) {
        sum = sum + i;
        i = i + 1;
    }

    /* nested loops: multiplication table style */
    int j;
    int product;
    int count;
    count = 0;
    i = 1;
    while (i < 6) {
        j = 1;
        while (j < 6) {
            product = i * j;
            if (product == 12) {
                count = count + 1;
            } else {
                count = count;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    /* loop with invariant expression that should be moved out */
    int base;
    int offset;
    int val;
    base = 100;
    offset = 5;
    i = 0;
    while (i < 10) {
        val = base + offset;
        sum = sum + val;
        i = i + 1;
    }

    /* countdown loop */
    i = 20;
    while (i > 0) {
        sum = sum - 1;
        i = i - 1;
    }

    return sum;
}