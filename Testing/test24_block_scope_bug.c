int main() {
    /* variable declared inside nested block */
    int x;
    x = 5;
    if (x > 0) {
        int y;
        y = 10;
        x = x + y;
    } else {
        int y;
        y = 20;
        x = x - y;
    }
    return x;
}
