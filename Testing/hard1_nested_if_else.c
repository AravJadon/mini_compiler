/* HARD TEST 1: Deeply nested if-else chains with all relational operators */
int main() {
    int a, b, c, d, e, result;
    a = 100;
    b = 200;
    c = 50;
    d = 100;
    e = 0;

    if (a < b) {
        if (c > d) {
            result = a + b;
        } else {
            if (a == d) {
                if (b != c) {
                    result = a * c - d;
                } else {
                    result = b / c;
                }
            } else {
                result = c + d + e;
            }
        }
    } else {
        if (a > b) {
            result = a - b;
        } else {
            result = 0;
        }
    }
}
