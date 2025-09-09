#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TEXT 1000
#define ALPHABET 26


char freqOrder[ALPHABET] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

// Structure for letter frequency
typedef struct {
    char letter;
    int count;
} Freq;

// Compare function for sorting frequencies
int cmp(const void *a, const void *b) {
    Freq *fa = (Freq *)a;
    Freq *fb = (Freq *)b;
    return fb->count - fa->count; // Descending order
}

// Function to build substitution mapping
void buildMapping(char *cipher, char map[26]) {
    int counts[26] = {0};
    int n = strlen(cipher);

    // Count frequency
    for (int i=0;i<n;i++) {
        if (isalpha(cipher[i]))
            counts[toupper(cipher[i]) - 'A']++;
    }

    // Build array for sorting
    Freq arr[26];
    for (int i=0;i<26;i++) {
        arr[i].letter = 'A' + i;
        arr[i].count = counts[i];
    }

    // Sort by frequency
    qsort(arr, 26, sizeof(Freq), cmp);

    // Map most frequent letters to freqOrder
    for (int i=0;i<26;i++) {
        map[arr[i].letter - 'A'] = freqOrder[i];
    }
}

// Decrypt using mapping
void decryptWithMap(char *cipher, char *plain, char map[26]) {
    int n = strlen(cipher);
    for (int i=0;i<n;i++) {
        if (isalpha(cipher[i])) {
            char c = toupper(cipher[i]);
            char sub = map[c - 'A'];
            plain[i] = islower(cipher[i]) ? tolower(sub) : sub;
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[n] = '\0';
}

int main() {
    char ciphertext[MAX_TEXT];
    int topN;

    printf("Enter ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    printf("How many top guesses to display? ");
    scanf("%d", &topN);

    char map[26];
    buildMapping(ciphertext, map);

    printf("\n--- Frequency Attack Results ---\n");

    // Generate guesses by shifting mapping
    for (int shift=0; shift<topN; shift++) {
        char testMap[26];
        for (int i=0;i<26;i++) {
            testMap[i] = freqOrder[(i+shift) % 26];
        }
        char plaintext[MAX_TEXT];
        decryptWithMap(ciphertext, plaintext, testMap);
        printf("\nGuess %d (mapping shift %d):\n%s\n", shift+1, shift, plaintext);
    }

    return 0;
}
