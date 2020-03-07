struct st:
    union un:
        real a;
        struct st2:
            int a;
            int b;
        int c;

    int d;
    int f;
    int z;
    union un2:
        int a;
        real b;

int main():
    return 0;
