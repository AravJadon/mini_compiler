int main() {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int result;
    int temp;

    a = 7;
    b = 3;
    c = 12;
    d = 4;
    e = 1;
    f = 0;
    g = 50;
    h = 25;
    result = 0;

    /* heavy expression: lots of temps generated */
    temp = a * b + c - d * e + f * g - h;
    result = temp + a * c - b * d + e;

    /* CSE: same sub-expression used twice */
    int x;
    int y;
    x = a + b;
    y = a + b;
    result = result + x - y;

    /* copy propagation chain */
    int p;
    int q;
    int r;
    p = a;
    q = p;
    r = q;
    result = result + r;

    /* if-else inside a while */
    int i;
    i = 0;
    while (i < 10) {
        if (i < 5) {
            result = result + i;
        } else {
            result = result - 1;
        }
        i = i + 1;
    }

    /* while inside an if */
    if (result > 0) {
        i = 0;
        while (i < 3) {
            result = result + 2;
            i = i + 1;
        }
    } else {
        result = 0;
    }

    /* deeply nested conditions */
    if (a > b) {
        if (c > d) {
            if (e > f) {
                if (g > h) {
                    result = result + 1000;
                } else {
                    result = result + 100;
                }
            } else {
                result = result + 10;
            }
        } else {
            result = result + 1;
        }
    } else {
        result = 0;
    }

    return result;
}