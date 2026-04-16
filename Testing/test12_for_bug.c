int main() {
    int count = 0;
    // The for-loop compiles syntactically (though the 3rd clause oddly requires a rel_bool)
    // but semantically the parser only has a /* for loop stub */
    // Thus it generates a jumping to an unpatched label and does not loop.
    for (int i = 0; i < 5; i > 0) {
        count++;
    }
    return 0;
}
