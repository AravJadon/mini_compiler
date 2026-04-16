int main() {
    int a;
    int b;
    int c;
    int d;
    int e;

    a = 2;
    b = 3;
    c = 4;
    d = 5;
    e = 6;

    /* deeply nested parenthesized expression */
    int result;
    result = ((a + b) * (c - d)) + ((e % a) - (b / c));

    /* chained compound assignments */
    result += 10;
    result -= 3;
    result *= 2;
    result /= 4;
    result %= 7;

    /* negative number arithmetic */
    int neg;
    neg = -a;
    neg = neg * -b;
    neg = neg + (-c);

    return result;
}
