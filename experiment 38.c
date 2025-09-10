#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define DIM 2 
#define MOD 26


int mod_inverse(int a) {
    a = a % MOD;
    for (int x = 1; x < MOD; x++) {
        if ((a * x) % MOD == 1) {
            return x;
        }
    }
    return -1; 
}


void matrix_mult(int A[DIM][DIM], int B[DIM][DIM], int C[DIM][DIM]) {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            C[i][j] = 0;
            for (int k = 0; k < DIM; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
            C[i][j] %= MOD;
        }
    }
}

// Function to find the inverse of a 2x2 matrix modulo 26
int matrix_inverse(int A[DIM][DIM], int inv_A[DIM][DIM]) {
    int det = (A[0][0] * A[1][1] - A[0][1] * A[1][0]) % MOD;
    if (det < 0) det += MOD;
    int det_inv = mod_inverse(det);

    if (det_inv == -1) {
        printf("Error: Matrix is not invertible modulo 26. Choose a different plaintext.\n");
        return 0; // Failure
    }

    inv_A[0][0] = (A[1][1] * det_inv) % MOD;
    inv_A[0][1] = (-A[0][1] * det_inv) % MOD;
    inv_A[1][0] = (-A[1][0] * det_inv) % MOD;
    inv_A[1][1] = (A[0][0] * det_inv) % MOD;

    // Ensure all values are positive
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (inv_A[i][j] < 0) {
                inv_A[i][j] += MOD;
            }
        }
    }
    return 1; // Success
}

// Function to convert plaintext to a matrix of numbers
void text_to_matrix(const char* text, int matrix[DIM][DIM]) {
    int k = 0;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            matrix[i][j] = toupper(text[k]) - 'A';
            k++;
        }
    }
}

// Function to convert a matrix of numbers back to plaintext
void matrix_to_text(const int matrix[DIM][DIM], char* text) {
    int k = 0;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            text[k] = matrix[i][j] + 'A';
            k++;
        }
    }
    text[k] = '\0';
}

int main() {
    // Original key matrix (known only to Alice and Bob)
    int key_matrix[DIM][DIM] = {{9, 4}, {5, 7}};

    // Known plaintext and corresponding ciphertext
    const char* known_plaintext = "HELP";
    char ciphertext[5];
    int plaintext_matrix[DIM][DIM];
    int ciphertext_matrix[DIM][DIM];
    char recovered_text[5];

    printf("--- Hill Cipher Known Plaintext Attack Demonstration ---\n\n");

    // Alice and Bob's encryption process
    text_to_matrix(known_plaintext, plaintext_matrix);
    matrix_mult(plaintext_matrix, key_matrix, ciphertext_matrix);
    matrix_to_text(ciphertext_matrix, ciphertext);

    printf("Step 1: The Attacker Observes a Known Pair\n");
    printf("Known Plaintext:  %s\n", known_plaintext);
    printf("Observed Ciphertext: %s\n\n", ciphertext);

    // --- The Attack ---
    // The attacker's goal is to find the key matrix using the known pair.
    
    // Step 2: Form the plaintext and ciphertext matrices (the attacker's perspective)
    int attacker_P[DIM][DIM];
    int attacker_C[DIM][DIM];
    text_to_matrix(known_plaintext, attacker_P);
    text_to_matrix(ciphertext, attacker_C);

    // Step 3: Find the inverse of the plaintext matrix P^-1
    int inv_P[DIM][DIM];
    printf("Step 3: Calculating the inverse of the Plaintext Matrix P^-1...\n");
    if (!matrix_inverse(attacker_P, inv_P)) {
        return 1;
    }
    printf("P^-1 is successfully calculated.\n\n");

    // Step 4: Multiply P^-1 by C to find the key matrix K'
    int recovered_key[DIM][DIM];
    printf("Step 4: Multiplying P^-1 by C to recover the key...\n");
    matrix_mult(inv_P, attacker_C, recovered_key);

    // Print the result of the attack
    printf("The original key matrix was:\n");
    printf("    [%d, %d]\n", key_matrix[0][0], key_matrix[0][1]);
    printf("    [%d, %d]\n", key_matrix[1][0], key_matrix[1][1]);
    printf("\nThe key matrix recovered by the attacker is:\n");
    printf("    [%d, %d]\n", recovered_key[0][0], recovered_key[0][1]);
    printf("    [%d, %d]\n", recovered_key[1][0], recovered_key[1][1]);

    // Verify if the recovered key is the same as the original key
    if (recovered_key[0][0] == key_matrix[0][0] && recovered_key[0][1] == key_matrix[0][1] &&
        recovered_key[1][0] == key_matrix[1][0] && recovered_key[1][1] == key_matrix[1][1]) {
        printf("\nResult: The attacker has successfully recovered the key.\n");
    } else {
        printf("\nResult: Failed to recover the key.\n");
    }

    return 0;
}
