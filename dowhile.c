int main() {
    int i = 0;
    int j;

    do {
        i++;
        j = j + i * 2;
    } while (j < 100);
    return j;
}
