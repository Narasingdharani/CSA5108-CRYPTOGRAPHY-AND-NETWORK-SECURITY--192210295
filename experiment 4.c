#include <stdio.h>
#include <string.h>
#include <ctype.h>


void vigenere_encrypt(char *plaintext, const char *key) {
    int plaintext_len = strlen(plaintext);
    int key_len = strlen(key);
    int i, j = 0;

    for (i = 0; i < plaintext_len; i++) {
        char ch = plaintext[i];

    
        if (isalpha(ch)) {
        
            char key_char = key[j % key_len];


            int shift = toupper(key_char) - 'A';

           
            char base = islower(ch) ? 'a' : 'A';

            // Apply the shift and wrap around the alphabet
            ch = base + ((ch - base + shift) % 26);
            plaintext[i] = ch;

            // Move to the next key character
            j++;
        }
    }
}

int main() {
    char plaintext[256];
    char key[256];

    // Prompt for the key
    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    // Remove the newline character
    key[strcspn(key, "\n")] = '\0';

    // Prompt for the plaintext
    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    // Remove the newline character
    plaintext[strcspn(plaintext, "\n")] = '\0';

    // Encrypt the message
    vigenere_encrypt(plaintext, key);

    // Print the encrypted message
    printf("Encrypted message: %s\n", plaintext);

    return 0;
}