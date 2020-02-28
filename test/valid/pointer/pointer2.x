int main():
    int a = 6;
    int* b = &a;
    (*b)--;
    return a;
