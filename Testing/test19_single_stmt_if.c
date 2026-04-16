int main() {
    int x;
    int y;
    x = 10;
    y = 0;

    /* if without braces - single statement */
    if (x > 5)
        y = 1;

    /* if-else without braces */
    if (x > 20)
        y = 100;
    else
        y = 200;

    /* while without braces */
    while (x > 0)
        x = x - 1;

    return y;
}
