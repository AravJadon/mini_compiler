int main() {
    int a, b, c, d;

    a = 10;

    // WARNING 1: use of uninitialized variable
    c = b + 5;

    // WARNING 2: variable set but never used
    d = 20;

    // WARNING 3: variable declared but never used
    int x;

    // valid usage to avoid errors
    a = a + 1;

    return 0;
}