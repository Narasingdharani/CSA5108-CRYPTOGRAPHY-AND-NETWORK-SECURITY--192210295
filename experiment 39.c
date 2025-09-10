#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

// Standard English letter frequencies (in percentage)
const double ENGLISH_FREQ[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015,
    6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749,
    7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758,
    0.978, 2.360, 0.150, 1.974, 0.074
};

// Struct to store a possible plaintext and its score
typedef struct {
    double score;
    char plaintext[256];
} PossiblePlaintext;

// Comparison function for qsort to sort in descending order of score
int comparePlaintexts(const void* a, const void* b) {
    PossiblePlaintext* p1 = (PossiblePlaintext*)a;
    PossiblePlaintext* p2 = (PossiblePlaintext*)b;
    if (p1->score > p2->score) return -1;
    if (p1->score < p2->score) return 1;
    return 0;
}

/**
 * Decrypts a ciphertext with a given shift and calculates a score
 * based on how well its letter frequencies match English.
 *
 * @param ciphertext The input message to decrypt.
 * @param shift The key (0-25) to use for decryption.
 * @param plaintext The buffer to store the decrypted message.
 * @return The score of the decrypted plaintext.
 */
double decrypt_and_score(const char* ciphertext, int shift, char* plaintext) {
    int total_chars = 0;
    int counts[26] = {0};
    double decrypted_freqs[26] = {0.0};
    double score = 0.0;

    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            char decrypted_char = (((ciphertext[i] - base - shift + 26) % 26) + base);
            plaintext[i] = decrypted_char;
            counts[toupper(decrypted_char) - 'A']++;
            total_chars++;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';

    if (total_chars == 0) return 0.0;

    // Calculate frequencies of the decrypted text
    for (int i = 0; i < 26; i++) {
        decrypted_freqs[i] = ((double)counts[i] / total_chars) * 100.0;
        // Calculate the score (sum of squared differences)
        score += pow(decrypted_freqs[i] - ENGLISH_FREQ[i], 2);
    }

    // Return the inverse of the score, so a lower difference means a higher rank.
    return 1.0 / score;
}

int main() {
    char ciphertext[256];
    int num_plaintexts;

    printf("--- Additive Cipher Frequency Analysis Attack ---\n\n");
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove trailing newline

    printf("Enter the number of top plaintexts to display (e.g., 10): ");
    scanf("%d", &num_plaintexts);

    PossiblePlaintext results[26];

    // Try all 26 possible shifts
    for (int shift = 0; shift < 26; shift++) {
        results[shift].score = decrypt_and_score(ciphertext, shift, results[shift].plaintext);
    }

    // Sort the results by score in descending order
    qsort(results, 26, sizeof(PossiblePlaintext), comparePlaintexts);

    printf("\n--- Possible Plaintexts by Likelihood ---\n");
    // Display the top N results
    for (int i = 0; i < num_plaintexts && i < 26; i++) {
        printf("%d. Score: %.4f | Plaintext: %s\n", i + 1, results[i].score, results[i].plaintext);
    }

    return 0;
}
