int main():
    int a = 5;
    int* b = &a;
    (*b) = 3;
    return a;
