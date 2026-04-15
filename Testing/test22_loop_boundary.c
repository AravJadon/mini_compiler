int main() {
    int a;
    a = 0;

    /* while loop that never executes */
    while (a > 100) {
        a = a + 1;
    }

    /* while loop with single iteration */
    a = 0;
    while (a < 1) {
        a = a + 1;
    }

    /* nested while with inner condition always false */
    int i;
    i = 0;
    while (i < 3) {
        int j;
        j = 100;
        while (j < 0) {
            j = j - 1;
        }
        i = i + 1;
    }

    return a;
}
