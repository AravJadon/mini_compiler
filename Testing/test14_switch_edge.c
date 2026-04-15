int main() {
    int x;
    x = 3;

    /* switch where no case matches, should hit default */
    switch(x) {
        case 1: {
            x = 10;
        }
        case 2: {
            x = 20;
        }
        default: {
            x = 99;
        }
    }

    /* switch with expression */
    int y;
    y = x + 1;
    switch(y) {
        case 100: {
            y = 0;
        }
        default: {
            y = 1;
        }
    }

    return x;
}
