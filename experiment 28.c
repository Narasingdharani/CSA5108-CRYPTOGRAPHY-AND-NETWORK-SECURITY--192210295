#include <stdio.h>
#include <math.h>

long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = (res * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int main() {
   
    long long q = 23; 
    long long a = 5;  // A primitive root modulo q (generator)

    printf("--- Diffie-Hellman Key Exchange Protocol ---\n\n");
    printf("Public Parameters:\n");
    printf("  q = %lld (a large prime number)\n", q);
    printf("  a = %lld (a primitive root modulo q)\n\n", a);

    // --- Alice's Side ---
    long long xA = 6; // Alice's secret private key (a random integer)
    printf("Alice selects her private key: xA = %lld\n", xA);
    long long yA = power(a, xA, q);
    printf("Alice computes her public key: yA = a^xA mod q = %lld^%lld mod %lld = %lld\n\n", a, xA, q, yA);

    // --- Bob's Side ---
    long long xB = 15; // Bob's secret private key (a random integer)
    printf("Bob selects his private key: xB = %lld\n", xB);
    long long yB = power(a, xB, q);
    printf("Bob computes his public key: yB = a^xB mod q = %lld^%lld mod %lld = %lld\n\n", a, xB, q, yB);

    // --- Public Exchange ---
    printf("Public Exchange:\n");
    printf("Alice sends yA (%lld) to Bob.\n", yA);
    printf("Bob sends yB (%lld) to Alice.\n\n", yB);

    // --- Key Derivation ---
    printf("Shared Secret Key Derivation:\n");
    // Alice computes the shared secret key
    long long kA = power(yB, xA, q);
    printf("Alice computes her shared secret key: kA = yB^xA mod q = %lld^%lld mod %lld = %lld\n", yB, xA, q, kA);

    // Bob computes the shared secret key
    long long kB = power(yA, xB, q);
    printf("Bob computes his shared secret key: kB = yA^xB mod q = %lld^%lld mod %lld = %lld\n\n", yA, xB, q, kB);

    printf("Result: kA and kB are equal, so a shared secret key (%lld) is established.\n", kA);
    printf("Note: Even though Eve knows a, q, yA, and yB, she cannot efficiently find xA or xB.\n");

    return 0;
}
