int main() {
    int i, j, sum = 0;
    for (i = 0, j = 10; i < j; i++, j--) {
        sum = sum + i + j;
    }
    return sum;
}