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
    int w = four(z + 3);
    int y = one(z) + two(z, w);
    int x = z + one(w);
    return z + w + y - x;

int four(int y):
    if(y < 1):
        return 1;
    return four(y - 1);

