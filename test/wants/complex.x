int main():
    complex a = 2.5r + 8.7i;
    complex b = -1r + 5.3i;
    complex c = a + b * a;

    imaginary im = 1.6i; // valid, im is complex now - equivalent to
                        // im = 0r + 1.6i

    // Ideas:
    // - Call floats real
