#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

long long gcd(long long a, long long b) {
    while (b) {
        long long temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

// --- RSA Signature Functions ---
// RSA signature generation (simplified for demonstration)
// Signature = (message_hash)^d mod n
long long rsa_sign(long long message_hash, long long d, long long n) {
    return power(message_hash, d, n);
}

// RSA verification (simplified for demonstration)
// The recipient computes (signature)^e mod n and checks if it equals the message hash
long long rsa_verify(long long signature, long long e, long long n) {
    return power(signature, e, n);
}

// --- DSA Signature Functions ---
// DSA signature generation (simplified for demonstration)
// The 'k' value is a one-time random number.
void dsa_sign(long long *r_out, long long *s_out, long long message_hash, long long p, long long q, long long g, long long x, long long k) {
    // 1. Compute r = (g^k mod p) mod q
    *r_out = power(g, k, p) % q;

    // 2. Compute s = (k^-1 * (message_hash + x * r)) mod q
    long long k_inverse = 0;
    long long temp_k = k;
    // Find modular inverse of k
    while (k_inverse < q) {
        if ((temp_k * k_inverse) % q == 1) {
            break;
        }
        k_inverse++;
    }
    *s_out = (k_inverse * (message_hash + x * (*r_out))) % q;
}

// DSA verification (simplified for demonstration)
void dsa_verify(long long r, long long s, long long message_hash, long long p, long long q, long long g, long long y) {
    // 1. Compute w = s^-1 mod q
    long long w = 0;
    while (w < q) {
        if ((s * w) % q == 1) {
            break;
        }
        w++;
    }

    // 2. Compute u1 = (message_hash * w) mod q
    long long u1 = (message_hash * w) % q;

    // 3. Compute u2 = (r * w) mod q
    long long u2 = (r * w) % q;

    // 4. Compute v = ((g^u1 * y^u2) mod p) mod q
    long long v = ((power(g, u1, p) * power(y, u2, p)) % p) % q;

    // 5. Verify that v == r
    if (v == r) {
        printf("DSA Signature is VALID.\n");
    } else {
        printf("DSA Signature is INVALID.\n");
    }
}

int main() {
    srand(time(NULL));

    long long message_hash = 1234567; // A mock hash value for a message
    printf("Message hash value: %lld\n\n", message_hash);

    // --- RSA Demonstration ---
    printf("--- RSA Signature (Deterministic) ---\n");
    // Simplified RSA parameters
    long long rsa_p = 101, rsa_q = 103;
    long long rsa_n = rsa_p * rsa_q;
    long long rsa_phi = (rsa_p - 1) * (rsa_q - 1);
    long long rsa_e = 65537;
    // Find a private key d (simplified)
    long long rsa_d = 0;
    while (rsa_d < rsa_phi) {
        if ((rsa_e * rsa_d) % rsa_phi == 1) {
            break;
        }
        rsa_d++;
    }

    long long rsa_sig1 = rsa_sign(message_hash, rsa_d, rsa_n);
    long long rsa_sig2 = rsa_sign(message_hash, rsa_d, rsa_n);

    printf("First RSA signature: %lld\n", rsa_sig1);
    printf("Second RSA signature: %lld\n", rsa_sig2);

    if (rsa_sig1 == rsa_sig2) {
        printf("Result: The two RSA signatures are IDENTICAL.\n\n");
    } else {
        printf("Result: The two RSA signatures are DIFFERENT.\n\n");
    }

    // --- DSA Demonstration ---
    printf("--- DSA Signature (Probabilistic) ---\n");
    // Simplified DSA parameters
    long long dsa_p = 107, dsa_q = 53, dsa_g = 2;
    long long dsa_x = 13; // Bob's private key
    long long dsa_y = power(dsa_g, dsa_x, dsa_p); // Bob's public key

    // First signature
    long long dsa_k1 = rand() % (dsa_q - 1) + 1; // Random 'k'
    long long dsa_r1, dsa_s1;
    dsa_sign(&dsa_r1, &dsa_s1, message_hash, dsa_p, dsa_q, dsa_g, dsa_x, dsa_k1);
    printf("Using k = %lld\n", dsa_k1);
    printf("First DSA signature (r, s): (%lld, %lld)\n", dsa_r1, dsa_s1);

    // Second signature
    long long dsa_k2 = rand() % (dsa_q - 1) + 1; // A different random 'k'
    long long dsa_r2, dsa_s2;
    dsa_sign(&dsa_r2, &dsa_s2, message_hash, dsa_p, dsa_q, dsa_g, dsa_x, dsa_k2);
    printf("Using k = %lld\n", dsa_k2);
    printf("Second DSA signature (r, s): (%lld, %lld)\n", dsa_r2, dsa_s2);

    if (dsa_r1 == dsa_r2 && dsa_s1 == dsa_s2) {
        printf("Result: The two DSA signatures are IDENTICAL.\n");
    } else {
        printf("Result: The two DSA signatures are DIFFERENT.\n");
    }

    // A note on verification to ensure the signatures are valid despite being different
    printf("\nVerifying first DSA signature...\n");
    dsa_verify(dsa_r1, dsa_s1, message_hash, dsa_p, dsa_q, dsa_g, dsa_y);
    printf("Verifying second DSA signature...\n");
    dsa_verify(dsa_r2, dsa_s2, message_hash, dsa_p, dsa_q, dsa_g, dsa_y);

    return 0;
}
