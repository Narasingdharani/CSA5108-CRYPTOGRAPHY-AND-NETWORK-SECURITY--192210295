#include <stdio.h>
#include <math.h>


double log2_factorial(int n) {
    double x = n;

    double result = x * log2(x) - x * log2(exp(1)) + 0.5 * log2(2 * M_PI * x);
    return result;
}

int main() {
    int n = 25;
    double log2_total = log2_factorial(n);          
    double log2_unique = log2_total - log2(8);      

    printf("Playfair Cipher Key Space Analysis\n");
    printf("---------------------------------\n");
    printf("Total possible keys (25!) ≈ 2^%.2f\n", log2_total);
    printf("Unique effective keys (25!/8) ≈ 2^%.2f\n", log2_unique);

    return 0;
}
