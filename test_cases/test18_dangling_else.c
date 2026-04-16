int main() {
    int a;
    int b;
    int c;
    a = 5;
    b = 10;
    c = 0;

    /* dangling else: the else should bind to inner if */
    if (a > 0)
        if (b > 0)
            c = 1;
        else
            c = 2;

    /* chained if-else-if */
    if (a > 10) {
        c = 100;
    } else {
        if (a > 5) {
            c = 50;
        } else {
            if (a > 0) {
                c = 10;
            } else {
                c = 0;
            }
        }
    }

    return c;
}
