#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_MESSAGE_SIZE 100

void generate_key(int* key, int length) {
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        key[i] = rand() % 26;
    }
}


void encrypt(const char* plaintext, const int* key, char* ciphertext) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = (char)(((plaintext[i] - base + key[i]) % 26) + base);
        } else {
            ciphertext[i] = plaintext[i]; // Non-alphabetic characters are not encrypted
        }
    }
    ciphertext[len] = '\0';
}

/**
 * Decrypts a ciphertext message using the one-time pad key stream.
 * @param ciphertext The message to be decrypted.
 * @param key The one-time pad key stream.
 * @param plaintext The buffer to store the decrypted message.
 */
void decrypt(const char* ciphertext, const int* key, char* plaintext) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            // Add 26 to handle negative results from the modulo operation
            plaintext[i] = (char)(((ciphertext[i] - base - key[i] + 26) % 26) + base);
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters are not decrypted
        }
    }
    plaintext[len] = '\0';
}

int main() {
    char plaintext[MAX_MESSAGE_SIZE] = "Send help, the attack begins at dawn!";
    char ciphertext[MAX_MESSAGE_SIZE];
    char decrypted_plaintext[MAX_MESSAGE_SIZE];
    int key[MAX_MESSAGE_SIZE];
    int len = strlen(plaintext);

    printf("Original Plaintext: %s\n", plaintext);

    // 1. Generate a random key stream
    generate_key(key, len);
    printf("Generated Key Stream: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", key[i]);
    }
    printf("\n\n");

    // 2. Encrypt the plaintext
    encrypt(plaintext, key, ciphertext);
    printf("Encrypted Ciphertext: %s\n", ciphertext);

    // 3. Decrypt the ciphertext using the same key
    decrypt(ciphertext, key, decrypted_plaintext);
    printf("Decrypted Plaintext:  %s\n\n", decrypted_plaintext);

    // Verify that decryption was successful
    if (strcmp(plaintext, decrypted_plaintext) == 0) {
        printf("Verification: The decrypted message matches the original plaintext. The one-time pad is correct.\n");
    } else {
        printf("Verification: There was an error in decryption.\n");
    }

    return 0;
}
