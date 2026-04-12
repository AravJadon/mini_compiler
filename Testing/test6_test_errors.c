int main() {
    int a, b;

    a = 10
    b = 20;          // ❌ syntax error (missing semicolon above)

    c = a + b;       // ❌ semantic error (undeclared variable 'c')

    int a;           // ❌ redeclaration of 'a'

    int d;
    int e;

    d = e + 5;       // ⚠️ warning: 'e' used uninitialized

    int x;
    x = 5 @ 3;       // ❌ lexical error (invalid token '@')

    if (a < ) {      // ❌ syntax error (incomplete condition)
        b = 1;
    }

    return 0;
}