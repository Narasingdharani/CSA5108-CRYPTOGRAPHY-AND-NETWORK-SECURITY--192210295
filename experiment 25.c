#include <stdio.h>

void find_prime_factors(int n, int *p, int *q) {
    *p = 0;
    *q = 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            *p = i;
            *q = n / i;
            break;
        }
    }
}

int extended_euclidean_algorithm(int a, int m) {
    int m0 = m;
    int y = 0, x = 1;

    if (m == 1) {
        return 0;
    }

    while (a > 1) {

        int q = a / m;
        int t = m;

        // m becomes remainder now, process same as Euclidean algorithm
        m = a % m;
        a = t;
        t = y;

        // Update x and y
        y = x - q * y;
        x = t;
    }

    // Make x positive
    if (x < 0) {
        x += m0;
    }

    return x;
}

int main() {
    int e = 31;
    int n = 3599;

    printf("Given public key: e = %d, n = %d\n\n", e, n);

    // Step 1: Find p and q by trial-and-error (using a simple function)
    int p, q;
    find_prime_factors(n, &p, &q);

    if (p == 0 || q == 0) {
        printf("Error: Could not find prime factors for n = %d\n", n);
        return 1;
    }

    printf("Step 1: Determine p and q\n");
    printf("p = %d, q = %d\n\n", p, q);

    // Step 2: Calculate phi(n)
    int phi_n = (p - 1) * (q - 1);
    printf("Step 2: Calculate Euler's totient function, phi(n)\n");
    printf("phi(n) = (p - 1) * (q - 1) = (%d - 1) * (%d - 1) = %d\n\n", p, q, phi_n);

    // Step 3: Find the private key d, the multiplicative inverse of e mod phi(n)
    int d = extended_euclidean_algorithm(e, phi_n);
    printf("Step 3: Find the private key d using the Extended Euclidean Algorithm\n");
    printf("d is the multiplicative inverse of %d modulo %d\n", e, phi_n);
    printf("d = %d\n\n", d);

    printf("The private key for the given user is d = %d\n", d);

    return 0;
}
