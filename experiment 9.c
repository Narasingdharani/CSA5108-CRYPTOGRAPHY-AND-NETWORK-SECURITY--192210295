#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char keySquare[SIZE][SIZE];


void generateKeySquare(char *key) {
    int used[26] = {0};
    int i, j, k = 0;
    char ch;


    for (i = 0; key[i]; i++) {
        ch = toupper(key[i]);
        if (ch < 'A' || ch > 'Z') continue;
        if (ch == 'J') ch = 'I';
        if (!used[ch - 'A']) {
            keySquare[k / SIZE][k % SIZE] = ch;
            used[ch - 'A'] = 1;
            k++;
        }
    }

    // Fill remaining
    for (ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!used[ch - 'A']) {
            keySquare[k / SIZE][k % SIZE] = ch;
            used[ch - 'A'] = 1;
            k++;
        }
    }
}

// Function to find position in key square
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

// Decrypt digraph
void decryptDigraph(char a, char b) {
    int r1, c1, r2, c2;
    findPosition(a, &r1, &c1);
    findPosition(b, &r2, &c2);

    if (r1 == r2) { // same row
        printf("%c%c", keySquare[r1][(c1 + SIZE - 1) % SIZE], keySquare[r2][(c2 + SIZE - 1) % SIZE]);
    } else if (c1 == c2) { // same column
        printf("%c%c", keySquare[(r1 + SIZE - 1) % SIZE][c1], keySquare[(r2 + SIZE - 1) % SIZE][c2]);
    } else { // rectangle
        printf("%c%c", keySquare[r1][c2], keySquare[r2][c1]);
    }
}

// Decrypt full ciphertext
void decryptMessage(char *cipher) {
    int len = strlen(cipher);
    for (int i = 0; i < len; i++) {
        if (!isalpha(cipher[i])) continue;
        char a = toupper(cipher[i]);
        i++;
        while (i < len && !isalpha(cipher[i])) i++;
        if (i < len) {
            char b = toupper(cipher[i]);
            decryptDigraph(a, b);
        }
    }
    printf("\n");
}

int main() {
    char key[] = "ROYALNAVY";
    char ciphertext[] =
        "KXJEY UREBE ZWEHE WRYTU HEYFS "
        "KREHE GOYFI WTTTU OLKSY CAJPO "
        "BOTEI ZONTX BYBNT GONEY CUZWR "
        "GDSON SXBOU YWRHE BAAHY USEDQ";

    printf("Playfair Cipher Decryption\n");
    printf("Key: %s\n", key);

    generateKeySquare(key);

    printf("\nKey Square:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", keySquare[i][j]);
        }
        printf("\n");
    }

    printf("\nCiphertext:\n%s\n", ciphertext);
    printf("\nDecrypted Plaintext:\n");
    decryptMessage(ciphertext);

    return 0;
}
