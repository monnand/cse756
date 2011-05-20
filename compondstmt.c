int main() {
    int a = 10;
    int i,j,k;
    for (i = 0; i < 100; ++i) {
        if (i < a)
            --a;
        for (j = i; j < 100; ++j) {
            k = a + i + j;
        }
    }

    return k;
}
