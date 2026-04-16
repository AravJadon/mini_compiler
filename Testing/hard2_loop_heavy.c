/* HARD TEST 2: Nested while loops with complex arithmetic */
int main() {
    int i, j, sum, product, temp,x,y;
    i = 0;
    sum = 0;
    product = 1;


    while (i < 5) {
        j = 0;
        while (j < 3) {
            temp = i * 3 + j;
            sum = sum + temp;
            x = y + 1;
            if (temp != 0) {
                product = product * 2;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    temp = sum + product;
}
