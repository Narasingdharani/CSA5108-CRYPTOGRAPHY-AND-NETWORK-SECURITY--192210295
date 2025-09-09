#include <stdio.h>
#include <string.h>
#include <ctype.h>


void caesar_cipher(char *text, int key, int mode) {
    int i = 0;
    while (text[i] != '\0') {
        char ch = text[i];


        if (isalpha(ch)) {
            char base = islower(ch) ? 'a' : 'A';

            ch = base + ((ch - base + key * mode) % 26);
        }
        text[i] = ch;
        i++;
    }
}

int main() {
    char text[100];
    int key;


    printf("Enter a message to encrypt: ");
    fgets(text, sizeof(text), stdin);

    text[strcspn(text, "\n")] = '\0';

   
    printf("Enter the shift key (1-25): ");
    scanf("%d", &key);

    if (key < 1 || key > 25) {
        printf("Invalid key. Please enter a value between 1 and 25.\n");
        return 1;
    }

   
    caesar_cipher(text, key, 1);
    printf("Encrypted message: %s\n", text);

   
    caesar_cipher(text, key, -1);
    printf("Decrypted message: %s\n", text);

    return 0;
}