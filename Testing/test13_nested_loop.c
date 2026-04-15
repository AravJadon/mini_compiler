int main() {
    int i;
    int j;
    int sum;
    sum = 0;
    i = 0;
    while (i < 3) {
        j = 0;
        while (j < 3) {
            sum = sum + 1;
            j = j + 1;
        }
        i = i + 1;
    }

    /* do-while inside while */
    i = 0;
    while (i < 2) {
        j = 3;
        do {
            sum = sum + j;
            j = j - 1;
        } while (j > 0);
        i = i + 1;
    }

    return sum;
}
