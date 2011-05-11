int kk = 1;
int jj = 2;

int main() {
    int aarr[10];
    int a = 1;

    int b;
    b = a + 1;
    aarr[b + 3] = 10;

    for (a = 0; a < 100; a++)
        b = a + 1;

    if (b > 100)
        a = b + a + 100;
    else
        a = aarr[3];

    a = 0;
    while (a < 100) {
        b = a + 1;
        a = b;
        a = -(a + 1);
        kk = b--;
        jj = ++b;
    }
}
