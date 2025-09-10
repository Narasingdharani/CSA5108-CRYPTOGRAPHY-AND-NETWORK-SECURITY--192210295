#include <stdio.h>
#include <math.h>


void factor_from_phi(long long n, long long phi_n, long long *p, long long *q) {

    long long sum_of_roots = n - phi_n + 1;


    long long discriminant = sum_of_roots * sum_of_roots - 4 * n;
    long long sqrt_discriminant = (long long)sqrt(discriminant);

    *p = (sum_of_roots + sqrt_discriminant) / 2;
    *q = (sum_of_roots - sqrt_discriminant) / 2;
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
        m = a % m;
        a = t;
        t = y;
        y = x - q * y;
        x = t;
    }

    if (x < 0) {
        x += m0;
    }

    return x;
}

int main() {
    // Assume Bob's original key pair is known by the adversary
    // (This is the "leak" from the problem description)
    int n = 3599; // Same modulus as the previous problem
    int e_old = 31;
    int d_old = 727;

    printf("--- ADVERSARY'S VULNERABILITY DEMONSTRATION ---\n\n");
    printf("Assume the original keys e = %d, d = %d, and n = %d were leaked.\n\n", e_old, d_old, n);

    // Step 1: Adversary calculates a value that is a multiple of phi(n)
    long long multiple_of_phi_n = (long long)e_old * d_old - 1;

    // In this specific case, we can deduce phi(n) directly.
    long long phi_n = multiple_of_phi_n / 1; // It's phi_n since d is the direct inverse.
    printf("Step 1: The adversary calculates phi(n) from the leaked keys:\n");
    printf("phi(n) = (e * d - 1) = (%d * %d - 1) = %lld\n\n", e_old, d_old, phi_n);

    // Step 2: Adversary factors the modulus 'n' using the calculated phi(n)
    long long p, q;
    factor_from_phi(n, phi_n, &p, &q);

    printf("Step 2: The adversary factors n into its prime components p and q:\n");
    printf("From n = %d and phi(n) = %lld\n", n, phi_n);
    printf("The adversary finds p = %lld and q = %lld\n\n", p, q);

    // Step 3: Bob generates a new public and private key with the same n
    int e_new = 13; // Bob chooses a new public key
    printf("--- BOB'S MISTAKE ---\n\n");
    printf("Bob, unaware of the vulnerability, generates a new public key e = %d.\n", e_new);
    printf("He uses the same modulus n = %d.\n\n", n);

    // Step 4: Adversary calculates the new private key d_new
    int d_new = extended_euclidean_algorithm(e_new, phi_n);
    printf("--- ADVERSARY'S FINAL STEP ---\n\n");
    printf("Step 3: The adversary calculates Bob's new private key (d_new):\n");
    printf("d_new is the multiplicative inverse of e_new (%d) modulo phi(n) (%lld)\n", e_new, phi_n);
    printf("d_new = %d\n\n", d_new);

    printf("Result: The adversary has successfully calculated Bob's new private key.\n");
    printf("Bob's new keys are now compromised without the need for another leak.\n");

    return 0;
}
