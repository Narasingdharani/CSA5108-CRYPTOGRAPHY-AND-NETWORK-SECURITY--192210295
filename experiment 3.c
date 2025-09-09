#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char matrix[SIZE][SIZE];
char alphabet[] = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; 


void get_position(char ch, int *row, int *col) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}


void prepare_plaintext(char *text, char *prepared) {
    int i, j = 0;
    // Remove non-alphabetic characters and convert to uppercase
    for (i = 0; i < strlen(text); i++) {
        if (isalpha(text[i])) {
            prepared[j++] = toupper(text[i] == 'j' ? 'i' : text[i]);
        }
    }
    prepared[j] = '\0';

    // Insert 'X' between repeating characters or at the end if length is odd
    for (i = 0; i < strlen(prepared); i += 2) {
        if (prepared[i] == prepared[i + 1]) {
            for (j = strlen(prepared); j > i + 1; j--) {
                prepared[j] = prepared[j - 1];
            }
            prepared[i + 1] = 'X';
        }
    }
    if (strlen(prepared) % 2 != 0) {
        prepared[strlen(prepared)] = 'X';
        prepared[strlen(prepared) + 1] = '\0';
    }
}

// Function to generate the 5x5 matrix from a keyword
void generate_matrix(const char *keyword) {
    int i, j, k;
    int char_count[26] = {0};
    int key_len = strlen(keyword);
    int row = 0, col = 0;

    // Place the keyword into the matrix
    for (i = 0; i < key_len; i++) {
        char ch = toupper(keyword[i] == 'j' ? 'i' : keyword[i]);
        if (isalpha(ch) && char_count[ch - 'A'] == 0) {
            matrix[row][col++] = ch;
            char_count[ch - 'A'] = 1;
            if (col == SIZE) {
                col = 0;
                row++;
            }
        }
    }

    // Fill the rest of the matrix with the remaining letters
    for (k = 0; k < 25; k++) {
        char ch = alphabet[k];
        if (char_count[ch - 'A'] == 0) {
            matrix[row][col++] = ch;
            if (col == SIZE) {
                col = 0;
                row++;
            }
        }
    }
}

// Function to encrypt the plaintext
void encrypt(char *prepared) {
    int r1, c1, r2, c2;
    int len = strlen(prepared);

    for (int i = 0; i < len; i += 2) {
        get_position(prepared[i], &r1, &c1);
        get_position(prepared[i+1], &r2, &c2);

        // Same row rule
        if (r1 == r2) {
            prepared[i] = matrix[r1][(c1 + 1) % SIZE];
            prepared[i+1] = matrix[r2][(c2 + 1) % SIZE];
        }
        // Same column rule
        else if (c1 == c2) {
            prepared[i] = matrix[(r1 + 1) % SIZE][c1];
            prepared[i+1] = matrix[(r2 + 1) % SIZE][c2];
        }
        // Rectangle rule
        else {
            prepared[i] = matrix[r1][c2];
            prepared[i+1] = matrix[r2][c1];
        }
    }
}

int main() {
    char keyword[256];
    char plaintext[256];
    char prepared_text[512]; // Increased size for prepared text

    printf("Enter the keyword: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    generate_matrix(keyword);
    prepare_plaintext(plaintext, prepared_text);
    encrypt(prepared_text);

    printf("Encrypted message: %s\n", prepared_text);

    return 0;
}