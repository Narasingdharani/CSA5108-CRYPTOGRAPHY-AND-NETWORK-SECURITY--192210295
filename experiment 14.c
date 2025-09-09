#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26


int charToNum(char c) { return toupper(c) - 'A'; }
char numToChar(int n) { return (n % MOD) + 'A'; }


void encrypt(char *plaintext, int key[], int keylen, char *cipher) {
    int n = strlen(plaintext), j=0;
    for (int i=0;i<n;i++) {
        if (isalpha(plaintext[i])) {
            int p = charToNum(plaintext[i]);
            int c = (p + key[j % keylen]) % MOD;
            cipher[i] = numToChar(c);
            j++;
        } else {
            cipher[i] = plaintext[i]; 
        }
    }
    cipher[n] = '\0';
}

// Decrypt with key stream
void decrypt(char *cipher, int key[], int keylen, char *plain) {
    int n = strlen(cipher), j=0;
    for (int i=0;i<n;i++) {
        if (isalpha(cipher[i])) {
            int c = charToNum(cipher[i]);
            int p = (c - key[j % keylen] + MOD) % MOD;
            plain[i] = numToChar(p);
            j++;
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[n] = '\0';
}

// Find key given plaintext and ciphertext
void findKey(char *cipher, char *plaintext, int key[], int n) {
    int j=0;
    for (int i=0;i<n;i++) {
        if (isalpha(cipher[i]) && isalpha(plaintext[i])) {
            int c = charToNum(cipher[i]);
            int p = charToNum(plaintext[i]);
            key[j++] = (c - p + MOD) % MOD;
        }
    }
}

int main() {
    char plaintext[] = "send more money";
    int key[] = {9,0,1,7,23,15,21,14,11,11,2,8,9};
    int keylen = sizeof(key)/sizeof(key[0]);
    char ciphertext[100], decrypted[100];

    printf("Plaintext: %s\n", plaintext);

    // Part (a) Encrypt
    encrypt(plaintext, key, keylen, ciphertext);
    printf("Ciphertext (part a): %s\n", ciphertext);

    // Part (a) Decrypt check
    decrypt(ciphertext, key, keylen, decrypted);
    printf("Decrypted back: %s\n", decrypted);

    // Part (b) - New plaintext
    char newPlain[] = "cash not needed";
    int newKey[100];
    findKey(ciphertext, newPlain, newKey, strlen(ciphertext));

    printf("\nNew Plaintext (part b): %s\n", newPlain);
    printf("Required key stream: ");
    for (int i=0;i<13;i++) printf("%d ", newKey[i]);
    printf("\n");

    return 0;
}
