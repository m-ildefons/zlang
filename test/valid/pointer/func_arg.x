int main():
    int a = 5;
    inc(&a);
    return a;

int inc(int* i):
    (*i) += 1;
    return 0;
