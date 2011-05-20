int main() {
    int i = 0;
    int j = 0;

    do {
        ++j;
    } while (j < 10);

    for (i = 0; i < 10; ++i) {
        ++j;
    }

    return j;
}
