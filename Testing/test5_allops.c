int main() {
    int a;
    int b;
    int c;
    int d;

    /* basic assignments */
    a = 10;
    b = 3;

    /* all arithmetic operators */
    c = a + b;
    c = a - b;
    c = a * b;
    c = a / b;
    c = a % b;

    /* unary minus */
    d = -a;
    d = -(a + b);

    /* increment and decrement */
    a++;
    ++b;
    a--;
    --b;

    /* compound assignments */
    c = 10;
    c += 5;
    c -= 2;
    c *= 3;
    c /= 4;
    c %= 7;

    /* all relational operators in conditions */
    int result;
    result = 0;

    if (a < b) {
        result = result + 1;
    } else {
        result = result + 0;
    }

    if (a > b) {
        result = result + 2;
    } else {
        result = result + 0;
    }

    if (a == b) {
        result = result + 4;
    } else {
        result = result + 0;
    }

    if (a != b) {
        result = result + 8;
    } else {
        result = result + 0;
    }

    if (a <= b) {
        result = result + 16;
    } else {
        result = result + 0;
    }

    if (a >= b) {
        result = result + 32;
    } else {
        result = result + 0;
    }

    /* boolean operators in conditions */
    if (a > 0 && b > 0) {
        result = result + 64;
    } else {
        result = result + 0;
    }

    if (a > 100 || b > 0) {
        result = result + 128;
    } else {
        result = result + 0;
    }

    if (!(a == 0)) {
        result = result + 256;
    } else {
        result = result + 0;
    }

    /* relational result assigned to variable */
    int flag;
    flag = (a > b);

    /* while loop with all operators */
    int i;
    int sum;
    sum = 0;
    i = 1;
    while (i < 11) {
        sum = sum + i * i;
        i = i + 1;
    }

    /* do-while loop */
    i = 5;
    while (i > 0) {
        sum = sum - i;
        i = i - 1;
    }

    /* big expression mixing everything */
    result = a + b * c - d / b + a % b - (-d);

    return result;
}