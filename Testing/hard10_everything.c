/* HARD TEST 10: Ultimate combined test - everything at once */
int main() {
    int n, i, j, temp, sum, max, min, avg;
    int found, count;

    n = 50;
    sum = 0;
    max = 0;
    min = 999;
    count = 0;
    found = 0;

    /* Outer loop: simulate processing N items */
    i = 1;
    while (i < n) {
        /* Complex expression for temp */
        temp = i * i - 3 * i + 2;

        /* Accumulate */
        sum = sum + temp;
        count = count + 1;

        /* Track max */
        if (temp > max) {
            max = temp;
        }

        /* Track min */
        if (temp < min) {
            min = temp;
        }

        /* Search: find first value > 100 */
        if (found == 0) {
            if (temp > 100) {
                found = i;
            }
        }

        /* Inner loop: do something for each i */
        j = 0;
        while (j < i) {
            if (j * 2 > i) {
                sum = sum + 1;
            }
            j = j + 1;
        }

        /* Constant folding opportunity */
        temp = 2 + 3 + 5 + 7;

        /* Common sub-expression */
        max = max + i * i;
        min = min + i * i;

        i = i + 1;
    }

    /* Final computation */
    if (count != 0) {
        avg = sum / count;
    } else {
        avg = 0;
    }

    /* Dead code: never used after this */
    temp = avg + max + min + found;
}
