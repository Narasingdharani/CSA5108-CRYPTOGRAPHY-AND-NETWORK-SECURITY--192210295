#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5


char keySquare[SIZE][SIZE] = {
    {'M','F','H','I','K'},
    {'U','N','O','P','Q'},
    {'Z','V','W','X','Y'},
    {'E','L','A','R','G'},
    {'D','S','T','B','C'}
};

void findPosition(char c, int *row, int *col) {
    if (c == 'J') c = 'I'; 
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keySquare[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}


void encryptDigraph(char a, char b) {
    int r1, c1, r2, c2;
    findPosition(a, &r1, &c1);
    findPosition(b, &r2, &c2);

    if (r1 == r2) { // same row
        printf("%c%c", keySquare[r1][(c1+1)%SIZE], keySquare[r2][(c2+1)%SIZE]);
    } else if (c1 == c2) { // same column
        printf("%c%c", keySquare[(r1+1)%SIZE][c1], keySquare[(r2+1)%SIZE][c2]);
    } else { // rectangle
        printf("%c%c", keySquare[r1][c2], keySquare[r2][c1]);
    }
}

// Encrypt message
void encryptMessage(char *plaintext) {
    int len = strlen(plaintext);
    char filtered[500], digraphs[500];
    int k = 0;

    // Remove non-letters, convert to uppercase
    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            char c = toupper(plaintext[i]);
            if (c == 'J') c = 'I';
            filtered[k++] = c;
        }
    }
    filtered[k] = '\0';

    // Create digraphs with X for duplicates
    int d = 0;
    for (int i = 0; i < k; i++) {
        digraphs[d++] = filtered[i];
        if (i+1 < k && filtered[i] == filtered[i+1]) {
            digraphs[d++] = 'X';
        }
    }
    if (d % 2 != 0) digraphs[d++] = 'X'; // pad odd length
    digraphs[d] = '\0';

    // Encrypt pairs
    for (int i = 0; i < d; i += 2) {
        encryptDigraph(digraphs[i], digraphs[i+1]);
    }
    printf("\n");
}

int main() {
    char plaintext[] = "Must see you over Cadogan West. Coming at once.";
    
    printf("Playfair Cipher Encryption\n");
    printf("Plaintext: %s\n", plaintext);

    printf("\nKey Square:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", keySquare[i][j]);
        }
        printf("\n");
    }

    printf("\nCiphertext:\n");
    encryptMessage(plaintext);

    return 0;
}
