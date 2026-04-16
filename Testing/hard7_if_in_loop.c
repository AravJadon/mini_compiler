/* HARD TEST 7: If-else inside loops AND loops inside if-else */
int main() {
    int i, j, count, flag;
    i = 0;
    count = 0;
    flag = 1;

    /* If-else inside loop */
    while (i < 10) {
        if (i < 5) {
            count = count + 1;
        } else {
            count = count + 2;
        }

        /* Nested: loop inside if, inside loop */
        if (flag == 1) {
            j = 0;
            while (j < 3) {
                count = count + i * j;
                j = j + 1;
            }
        }
        i = i + 1;
    }

    /* Loop inside if-else */
    if (count > 100) {
        i = 0;
        while (i < count) {
            i = i + 10;
        }
    } else {
        i = count;
        while (i > 0) {
            i = i - 1;
        }
    }
}
