#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


const double english_freq[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, // A-G
    6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749, // H-N
    7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, // O-U
    0.978, 2.360, 0.150, 1.974, 0.074  // V-Z
};


typedef struct {
    char c;
    double freq;
} CharFreq;

// Comparison function for qsort to sort in descending order of frequency
int compare_freq(const void* a, const void* b) {
    CharFreq* charFreqA = (CharFreq*)a;
    CharFreq* charFreqB = (CharFreq*)b;
    if (charFreqA->freq < charFreqB->freq) return 1;
    if (charFreqA->freq > charFreqB->freq) return -1;
    return 0;
}

/**
 * Calculates the letter frequencies of a given text.
 * @param text The input string.
 * @param freqs An array to store the calculated frequencies (26 elements).
 * @param sorted_freqs An array of CharFreq to store sorted frequencies.
 */
void calculate_freq(const char* text, double freqs[26], CharFreq sorted_freqs[26]) {
    int total_chars = 0;
    for (int i = 0; i < 26; i++) {
        freqs[i] = 0;
        sorted_freqs[i].c = 'A' + i;
    }

    // Count character occurrences
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            freqs[toupper(text[i]) - 'A']++;
            total_chars++;
        }
    }

    // Convert counts to percentages and copy to sorted array
    for (int i = 0; i < 26; i++) {
        if (total_chars > 0) {
            sorted_freqs[i].freq = (freqs[i] / total_chars) * 100;
        } else {
            sorted_freqs[i].freq = 0;
        }
    }

    // Sort the frequencies in descending order
    qsort(sorted_freqs, 26, sizeof(CharFreq), compare_freq);
}

/**
 * Generates a decryption key based on frequency analysis.
 * The key is an array mapping each encrypted character to a decrypted character.
 * @param ciphertext_freq The sorted frequencies of the ciphertext.
 * @param key The key array to be filled (26 characters).
 */
void generate_key(const CharFreq ciphertext_freq[26], char key[26]) {
    // A simple heuristic: map the most frequent ciphertext letters to the most frequent
    // English letters. This can be improved with more complex algorithms.
    for (int i = 0; i < 26; i++) {
        key[ciphertext_freq[i].c - 'A'] = 'E'; // Placeholder, will be replaced by a more robust method
    }

    // A more sophisticated approach: map based on ranked order
    CharFreq sorted_english[26];
    for (int i = 0; i < 26; i++) {
        sorted_english[i].c = 'A' + i;
        sorted_english[i].freq = english_freq[i];
    }
    qsort(sorted_english, 26, sizeof(CharFreq), compare_freq);

    for (int i = 0; i < 26; i++) {
        key[ciphertext_freq[i].c - 'A'] = sorted_english[i].c;
    }
}

/**
 * Decrypts a ciphertext using a given key.
 * @param ciphertext The message to decrypt.
 * @param key The decryption key.
 * @param plaintext The buffer to store the decrypted message.
 */
void decrypt(const char* ciphertext, const char key[26], char* plaintext) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            plaintext[i] = key[toupper(ciphertext[i]) - 'A'];
            if (islower(ciphertext[i])) {
                plaintext[i] = tolower(plaintext[i]);
            }
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters are not changed
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int main() {
 
    const char* ciphertext = "LFDPHQFLWKDWLPHDQHFNFPHQWIRUHDWLRQ";
    char plaintext[200];
    int num_plaintexts;

    double cipher_freqs[26];
    CharFreq sorted_cipher_freqs[26];
    char key[26];

    printf("--- Monoalphabetic Substitution Cipher Cracker ---\n");
    printf("Ciphertext: %s\n\n", ciphertext);

 
    calculate_freq(ciphertext, cipher_freqs, sorted_cipher_freqs);


    generate_key(sorted_cipher_freqs, key);

    decrypt(ciphertext, key, plaintext);
    printf("Most likely plaintext: %s\n\n", plaintext);


    printf("Enter the number of top plaintexts to show (1 is the only option here): ");
    scanf("%d", &num_plaintexts);

    printf("\nTop %d possible plaintexts:\n", num_plaintexts);
    if (num_plaintexts >= 1) {
        printf("1. %s\n", plaintext);
    }
    printf("\n");

    return 0;
}
