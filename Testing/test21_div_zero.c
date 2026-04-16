int main() {
    int a;
    int b;
    a = 10;
    b = 0;

    /* division by zero - should still compile, just bad at runtime */
    int c;
    c = a / b;
    c = a % b;

    return 0;
}
