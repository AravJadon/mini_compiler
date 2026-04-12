int main() {
    int a;
    int b;
    int c;
    int result;

    a = 15;
    b = 20;
    c = 15;
    result = 0;

    /* basic if-else with < and > */
    if (a < b) {
        result = 1;
    } else {
        result = 0;
    }

    /* equality check */
    if (a == c) {
        result = result + 10;
    } else {
        result = result + 0;
    }

    /* not-equal */
    if (a != b) {
        result = result + 100;
    } else {
        result = result - 100;
    }

    /* compound condition with && */
    if (a < b && a == c) {
        result = result + 1000;
    } else {
        result = result - 1;
    }

    /* compound condition with || */
    if (a > b || a == c) {
        result = result + 5;
    } else {
        result = result - 5;
    }

    /* negation */
    if (!(a > b)) {
        result = result + 2;
    } else {
        result = result - 2;
    }

    /* nested if-else */
    if (a < b) {
        if (a == c) {
            if (b > c) {
                result = result + 7;
            } else {
                result = result - 7;
            }
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }

    return result;
}