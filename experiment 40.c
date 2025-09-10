#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT 10000

const char *english_freq_order = "ETAOINSHRDLCUMWFGYPBVKJXQZ";


void count_frequencies(const char *text, int freq[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        freq[i] = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha((unsigned char)text[i])) {
            freq[toupper(text[i]) - 'A']++;
        }
    }
}

// Structure for letter frequency sorting
typedef struct {
    char letter;
    int count;
} LetterFreq;

// Comparison function for sorting
int cmp_freq(const void *a, const void *b) {
    LetterFreq *fa = (LetterFreq*)a;
    LetterFreq *fb = (LetterFreq*)b;
    return fb->count - fa->count; // descending order
}

// Generate substitution mapping
void generate_mapping(LetterFreq sorted[], char mapping[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        mapping[sorted[i].letter - 'A'] = english_freq_order[i];
    }
}

// Apply substitution mapping to ciphertext
void apply_mapping(const char *cipher, char *plain, char mapping[ALPHABET_SIZE]) {
    for (int i = 0; cipher[i]; i++) {
        if (isalpha((unsigned char)cipher[i])) {
            int idx = toupper(cipher[i]) - 'A';
            char subst = mapping[idx];
            plain[i] = isupper(cipher[i]) ? subst : tolower(subst);
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[strlen(cipher)] = '\0';
}

int main() {
    char ciphertext[MAX_TEXT];
    int freq[ALPHABET_SIZE];
    LetterFreq letters[ALPHABET_SIZE];
    char mapping[ALPHABET_SIZE];
    char plaintext[MAX_TEXT];
    int topN;

    printf("Enter ciphertext: ");
    fgets(ciphertext, MAX_TEXT, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    printf("Enter number of possible plaintexts to display (e.g., 10): ");
    scanf("%d", &topN);

    // Step 1: Count frequencies
    count_frequencies(ciphertext, freq);

    // Step 2: Store letters with their frequencies
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        letters[i].letter = 'A' + i;
        letters[i].count = freq[i];
    }

    // Step 3: Sort by frequency
    qsort(letters, ALPHABET_SIZE, sizeof(LetterFreq), cmp_freq);

    // Step 4: Generate candidate mappings and show results
    for (int t = 0; t < topN && t < ALPHABET_SIZE; t++) {
        generate_mapping(letters, mapping);

        // Slight variation: rotate english_freq_order by t to generate multiple guesses
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            mapping[letters[i].letter - 'A'] = english_freq_order[(i + t) % ALPHABET_SIZE];
        }

        apply_mapping(ciphertext, plaintext, mapping);

        printf("\nPossible Plaintext #%d:\n%s\n", t + 1, plaintext);
    }

    return 0;
}
