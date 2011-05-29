int a = 0;
int main() {
    int i;
    int j;
    int b = 2*(a + 1);
    for (i = 0; i < 10; ++i) {
        j = i + 1 + b;
    }

    for (i = 0; i < 10; ++i) {
        j = i + 1;
    }
    return j;
}
