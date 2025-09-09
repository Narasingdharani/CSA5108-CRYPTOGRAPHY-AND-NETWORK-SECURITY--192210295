#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26


int charToNum(char c) { return toupper(c) - 'A'; }
char numToChar(int n) { return (n % MOD) + 'A'; }


void decryptShift(char *cipher, int key, char *plain) {
    int n = strlen(cipher);
    for (int i=0;i<n;i++) {
        if (isalpha(cipher[i])) {
            int c = charToNum(cipher[i]);
            int p = (c - key + MOD) % MOD;
            plain[i] = numToChar(p);
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[n] = '\0';
}

// Find most frequent letter in ciphertext
char mostFrequentLetter(char *cipher) {
    int freq[26] = {0};
    int n = strlen(cipher);
    for (int i=0;i<n;i++) {
        if (isalpha(cipher[i])) {
            freq[charToNum(cipher[i])]++;
        }
    }
    int max=0, idx=0;
    for (int i=0;i<26;i++) {
        if (freq[i] > max) { max = freq[i]; idx = i; }
    }
    return numToChar(idx);
}

int main() {
    char ciphertext[500];
    int topN;

    printf("Enter ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    printf("How many top guesses to display? ");
    scanf("%d", &topN);

    // Find most frequent letter in ciphertext
    char mf = mostFrequentLetter(ciphertext);
    int keyGuess = (charToNum(mf) - charToNum('E') + MOD) % MOD;

    printf("\nMost frequent letter in ciphertext: %c\n", mf);
    printf("Initial key guess (assuming maps to E): %d\n\n", keyGuess);

    char plaintext[500];
    for (int k=0; k<topN && k<26; k++) {
        int tryKey = (keyGuess + k) % 26;
        decryptShift(ciphertext, tryKey, plaintext);
        printf("Key = %2d -> %s\n", tryKey, plaintext);
    }

    return 0;
}
