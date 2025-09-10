#include <stdio.h>
#include <ctype.h>


int encrypt_char(int a, int b, int p) {
    return (a * p + b) % 26;
}

int main() {
    int a, b;
    int p1, p2;

    printf("--- Affine Caesar Cipher One-to-One Demonstration ---\n\n");

    a = 7; 
    b = 3;
    printf("Case 1: a = %d, b = %d (a is valid)\n", a, b);
    printf("Expected: One-to-one mapping, decryption is possible.\n\n");

    p1 = 0; // 'A'
    p2 = 13; // 'N'

    int c1 = encrypt_char(a, b, p1);
    int c2 = encrypt_char(a, b, p2);

    printf("Plaintext char 'A' (p = %d) encrypts to: %d\n", p1, c1);
    printf("Plaintext char 'N' (p = %d) encrypts to: %d\n\n", p2, c2);

    if (c1 != c2) {
        printf("Result: The mapping is one-to-one. This is a secure key.\n");
    } else {
        printf("Result: The mapping is NOT one-to-one.\n");
    }
    printf("---------------------------------------------------\n\n");

    // Case 2: An invalid key where a is not coprime to 26
    a = 2; // Not coprime to 26 (gcd(2, 26) = 2)
    b = 3;
    printf("Case 2: a = %d, b = %d (a is invalid)\n", a, b);
    printf("Expected: NON-one-to-one mapping, decryption is impossible.\n\n");

    p1 = 0; // 'A'
    p2 = 13; // 'N'

    c1 = encrypt_char(a, b, p1);
    c2 = encrypt_char(a, b, p2);

    printf("Plaintext char 'A' (p = %d) encrypts to: %d\n", p1, c1);
    printf("Plaintext char 'N' (p = %d) encrypts to: %d\n\n", p2, c2);

    if (c1 != c2) {
        printf("Result: The mapping is one-to-one. This is a secure key.\n");
    } else {
        printf("Result: The mapping is NOT one-to-one. This is an insecure key.\n");
    }

    return 0;
}
