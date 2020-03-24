int main():
    return fib(10);

int fib(int s):
    if s <= 1:
        return s;
    return fib(s-1) + fib(s-2);

