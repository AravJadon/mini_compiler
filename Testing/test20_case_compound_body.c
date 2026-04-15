int main() {
    int x;
    int y;
    x = 2;
    y = 0;

    /* case body with multiple statements in braces */
    switch (x) {
        case 1: {
            y = 10;
            y = y + 1;
        }
        case 2: {
            y = 20;
            y = y + 2;
        }
        case 3: {
            y = 30;
        }
        default: {
            y = y + 100;
        }
    }

    return y;
}
