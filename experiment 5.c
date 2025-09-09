#include <stdio.h>
#include <string.h>


int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1; 
}


void encrypt(char plaintext[], int a, int b) {
    printf("Encrypted text: ");
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];
        if (ch >= 'A' && ch <= 'Z') { 
            int p = ch - 'A';
            int C = (a * p + b) % 26;
            printf("%c", C + 'A');
        } 
        else if (ch >= 'a' && ch <= 'z') { // lowercase letters
            int p = ch - 'a';
            int C = (a * p + b) % 26;
            printf("%c", C + 'a');
        } 
        else {
            printf("%c", ch); // Non-alphabetic characters remain unchanged
        }
    }
    printf("\n");
}

// Decrypt function
void decrypt(char ciphertext[], int a, int b) {
    int a_inv = modInverse(a, 26); // find inverse of a mod 26
    if (a_inv == -1) {
        printf("Decryption not possible: 'a' has no inverse modulo 26.\n");
        return;
    }

    printf("Decrypted text: ");
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];
        if (ch >= 'A' && ch <= 'Z') {
            int C = ch - 'A';
            int P = (a_inv * (C - b + 26)) % 26;
            printf("%c", P + 'A');
        } 
        else if (ch >= 'a' && ch <= 'z') {
            int C = ch - 'a';
            int P = (a_inv * (C - b + 26)) % 26;
            printf("%c", P + 'a');
        } 
        else {
            printf("%c", ch);
        }
    }
    printf("\n");
}

int main() {
    char text[100];
    int a, b;

    printf("Enter the plaintext: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter key 'a' (must be coprime with 26): ");
    scanf("%d", &a);

    printf("Enter key 'b': ");
    scanf("%d", &b);

    // Remove newline character if present
    text[strcspn(text, "\n")] = '\0';

    // Encrypt and Decrypt
    encrypt(text, a, b);
    decrypt(text, a, b);

    return 0;
}
