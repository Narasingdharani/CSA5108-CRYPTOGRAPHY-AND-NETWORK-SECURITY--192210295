#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26


void generateCipherAlphabet(char keyword[], char cipher[]) {
    int used[26] = {0};
    int idx = 0;


    for (int i = 0; keyword[i] != '\0'; i++) {
        char ch = toupper(keyword[i]);
        if (ch >= 'A' && ch <= 'Z' && !used[ch - 'A']) {
            cipher[idx++] = ch;
            used[ch - 'A'] = 1;
        }
    }

    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (!used[ch - 'A']) {
            cipher[idx++] = ch;
        }
    }
    cipher[idx] = '\0';
}

// Encrypt function
void encrypt(char plaintext[], char cipher[]) {
    printf("Encrypted text: ");
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];
        if (isalpha(ch)) {
            int pos = tolower(ch) - 'a';
            char enc = cipher[pos];
            if (islower(ch))
                enc = tolower(enc);
            printf("%c", enc);
        } else {
            printf("%c", ch);
        }
    }
    printf("\n");
}

// Decrypt function
void decrypt(char ciphertext[], char cipher[]) {
    printf("Decrypted text: ");
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];
        if (isalpha(ch)) {
            char upper = toupper(ch);
            int pos = -1;
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                if (cipher[j] == upper) {
                    pos = j;
                    break;
                }
            }
            char dec = 'A' + pos;
            if (islower(ch))
                dec = tolower(dec);
            printf("%c", dec);
        } else {
            printf("%c", ch);
        }
    }
    printf("\n");
}

int main() {
    char keyword[] = "CIPHER"; // keyword
    char cipher[ALPHABET_SIZE + 1];
    char text[200];

    // Generate cipher alphabet
    generateCipherAlphabet(keyword, cipher);

    // Print mapping
    printf("Plain:  ");
    for (char ch = 'A'; ch <= 'Z'; ch++) printf("%c ", ch);
    printf("\nCipher: ");
    for (int i = 0; i < ALPHABET_SIZE; i++) printf("%c ", cipher[i]);
    printf("\n\n");

    // Input plaintext
    printf("Enter plaintext: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    encrypt(text, cipher);
    decrypt(text, cipher);

    return 0;
}
