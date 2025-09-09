#include <stdio.h>
#include <string.h>
#include <ctype.h>


void monoalphabetic_encrypt(char *plaintext, const char *key) {
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int i = 0;


    while (plaintext[i] != '\0') {
        char ch = plaintext[i];

        if (isalpha(ch)) {
            
            if (islower(ch)) {
                
                int index = ch - 'a';
                
                plaintext[i] = tolower(key[index]);
            } else if (isupper(ch)) {
                
                int index = ch - 'A';
                
                plaintext[i] = toupper(key[index]);
            }
        }
       
        i++;
    }
}

int main() {
    char plaintext[256];
    char key[27];

  
    printf("Enter the 26-letter substitution key (e.g., QWERTY...): ");
    scanf("%s", key);

    
    if (strlen(key) != 26) {
        printf("Invalid key length. The key must be exactly 26 letters.\n");
        return 1;
    }

    
    while (getchar() != '\n');

    // Prompt the user for the message to encrypt
    printf("Enter the message to encrypt: ");
    fgets(plaintext, sizeof(plaintext), stdin);

    // Remove the newline character added by fgets()
    plaintext[strcspn(plaintext, "\n")] = '\0';

    // Encrypt the message
    monoalphabetic_encrypt(plaintext, key);

    // Print the encrypted message
    printf("Encrypted message: %s\n", plaintext);

    return 0;
}