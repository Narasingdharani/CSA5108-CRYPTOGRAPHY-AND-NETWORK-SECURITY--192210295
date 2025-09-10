#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

long long rsa_encrypt(int plaintext_char, long long e, long long n) {
    return power(plaintext_char, e, n);
}


void perform_attack(long long e, long long n, char *ciphertext_message, char *decrypted_message, int len) {
    printf("--- ADVERSARY'S ATTACK DEMONSTRATION ---\n\n");
    printf("Step 1: The adversary pre-computes the ciphertext for every possible plaintext character (0-25).\n");

    // Create a simple lookup table (dictionary)
    long long lookup_table[26];
    for (int i = 0; i < 26; i++) {
        lookup_table[i] = rsa_encrypt(i, e, n);
        printf("Plaintext '%c' (%d) -> Ciphertext %lld\n", (char)('A' + i), i, lookup_table[i]);
    }

    printf("\nStep 2: The adversary intercepts an encrypted message.\n");
    printf("Ciphertext message: %s\n", ciphertext_message);

    // Now, decrypt the intercepted message using the lookup table
    printf("\nStep 3: The adversary decrypts the message by looking up each ciphertext block.\n");
    printf("Decrypted message: ");
    for (int i = 0; i < len; i++) {
        long long current_ciphertext = atoll(&ciphertext_message[i * 10]); // Assuming each ciphertext is 10 digits for simplicity
        
        // Find the matching plaintext in the lookup table
        for (int j = 0; j < 26; j++) {
            if (lookup_table[j] == current_ciphertext) {
                decrypted_message[i] = 'A' + j;
                break;
            }
        }
    }
    decrypted_message[len] = '\0';
    printf("%s\n\n", decrypted_message);
}

int main() {
    // Bob's RSA public key (large numbers for demonstration)
    long long e = 65537; // Common RSA public exponent
    long long n = 1279768651; // A large modulus for a demo (p=35719, q=35821)
    
    // Alice's plaintext message
    char plaintext_message[] = "HELLO";
    int plaintext_len = strlen(plaintext_message);
    
    printf("--- RSA CHARACTER-BY-CHARACTER ENCRYPTION ---\n\n");
    printf("Bob's public key: e = %lld, n = %lld\n", e, n);
    printf("Alice's plaintext message: %s\n\n", plaintext_message);

    // Alice encrypts the message character by character
    char ciphertext_message[100];
    for (int i = 0; i < plaintext_len; i++) {
        int plaintext_char = plaintext_message[i] - 'A';
        long long ciphertext = rsa_encrypt(plaintext_char, e, n);
        sprintf(&ciphertext_message[i * 10], "%lld", ciphertext);
    }
    
    printf("Encrypted message (concatenated ciphertexts): %s\n\n", ciphertext_message);

    // Adversary performs the attack
    char decrypted_message[100];
    perform_attack(e, n, ciphertext_message, decrypted_message, plaintext_len);

    return 0;
}
