int main() {
    int i, j, k;

    for (i = 0; i < 10; ++i) {
        j = i;
        while (j < 10) {
            k = i + j;
            j++;
        }
    }

    for (i = 0; i < 100; ++i) {
        k += i;
    }
    return k;
}
