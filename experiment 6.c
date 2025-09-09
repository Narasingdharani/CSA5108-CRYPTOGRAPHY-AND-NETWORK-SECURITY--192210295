#include <stdio.h>
#include <string.h>

void decrypt(char ciphertext[], int a, int b) {
    int a_inv = 0;

    for (int i = 1; i < 26; i++) {
        if ((a * i) % 26 == 1) {
            a_inv = i;
            break;
        }
    }

    if (a_inv == 0) {
        printf("No modular inverse for a=%d. Decryption not possible.\n", a);
        return;
    }

    printf("Decrypted text: ");
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];
        if (ch >= 'A' && ch <= 'Z') {
            int C = ch - 'A';
            int P = (a_inv * (C - b + 26)) % 26;
            printf("%c", P + 'A');
        } else if (ch >= 'a' && ch <= 'z') {
            int C = ch - 'a';
            int P = (a_inv * (C - b + 26)) % 26;
            printf("%c", P + 'a');
        } else {
            printf("%c", ch);
        }
    }
    printf("\n");
}

int main() {
    char ciphertext[200];

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // remove newline

    int a = 3, b = 15; // Derived keys from frequency analysis

    printf("Derived keys: a = %d, b = %d\n", a, b);
    decrypt(ciphertext, a, b);

    return 0;
}
