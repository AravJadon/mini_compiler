int main() {
    /* stress test: lots of variables to force register spilling */
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    int j;

    a = 1;
    b = 2;
    c = 3;
    d = 4;
    e = 5;
    f = 6;
    g = 7;
    h = 8;
    i = 9;
    j = 10;

    /* use all of them together to force interference */
    int sum;
    sum = a + b + c + d + e + f + g + h + i + j;

    /* modify all */
    a = a + 1;
    b = b + 1;
    c = c + 1;
    d = d + 1;
    e = e + 1;
    f = f + 1;
    g = g + 1;
    h = h + 1;
    i = i + 1;
    j = j + 1;

    sum = a + b + c + d + e + f + g + h + i + j;

    return sum;
}
