int main():
    return func(2, 3, 4, 3, 4, 3, 4, 8);

int func(int a, int b, int c, int d, int e, int f, int g, int h):
    int s = a * b + c;
    int t = d * e + f;
    int u = f * g + h;
    return s * t + u;
