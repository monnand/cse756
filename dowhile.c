int main() {
    int i = 0;
    int j;

    do {
        i++;
        j = j + i * 2;
    } while (j < 100);

    i = 0;
    do {
        j++;
        do {
            i++;
        } while (i < 20);
    } while (i < 100);

    for (i = 0; i < 100; ++i) {
        j--;
    }
    return j;
}
