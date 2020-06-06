int one(int a):
    return a + 2;

int two(int b, int c):
    if(b > 3):
        return b;
    return c * b + 7;

int three(int a, int b):
    return a + 3 * b;

int main():
    int z = 5;

    // w = 1
    int w = four(z + 3);

    // y = 7 + 5
    int y = one(z) + two(z, w);

    // x = 5 + 3
    int x = z + one(w);

    // 5 + 1 + 12 - 8 = 10
    return z + w + y - x;

int four(int y):
    if(y < 1):
        return 1;
    return four(y - 1);

