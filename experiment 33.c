#include <stdio.h>
#include <string.h>
#include <stdint.h>

const int IP_TABLE[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};


const int FP_TABLE[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};


const int E_TABLE[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

// P-box Permutation (P) table
const int P_TABLE[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

// S-box tables (S1 to S8)
const int S_BOX[8][4][16] = {
    // S1
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    // S4
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 4, 2, 8},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2}},
    // S5
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    // S6
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    // S7
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    // S8
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
};

// --- Function Prototypes ---
void perform_permutation(uint64_t *output, uint64_t input, const int *table, int size);
void perform_expansion(uint64_t *output, uint32_t input);
uint32_t perform_s_box(uint64_t input);
uint32_t feistel_function(uint32_t right_half, uint64_t round_key);
void des_encrypt_block(uint64_t *ciphertext, uint64_t plaintext, uint64_t keys[16]);
void des_decrypt_block(uint64_t *plaintext, uint64_t ciphertext, uint64_t keys[16]);

// --- Core DES Functions ---

// Performs a generic permutation on a block
void perform_permutation(uint64_t *output, uint64_t input, const int *table, int size) {
    *output = 0;
    for (int i = 0; i < size; i++) {
        int pos = table[i] - 1;
        uint64_t bit = (input >> (63 - pos)) & 1;
        *output |= (bit << (63 - i));
    }
}

// Performs the 32-bit to 48-bit expansion permutation
void perform_expansion(uint64_t *output, uint32_t input) {
    *output = 0;
    for (int i = 0; i < 48; i++) {
        int pos = E_TABLE[i] - 1;
        uint64_t bit = (uint64_t)((input >> (31 - pos)) & 1);
        *output |= (bit << (47 - i));
    }
}

// Performs the S-box substitution
uint32_t perform_s_box(uint64_t input) {
    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        // Extract 6-bit block for S-box i
        uint64_t sub_block = (input >> (42 - (i * 6))) & 0x3F;
        int row = ((sub_block >> 5) & 1) | ((sub_block & 1) << 1);
        int col = (sub_block >> 1) & 0x0F;
        int sbox_val = S_BOX[i][row][col];
        output |= (sbox_val << (28 - (i * 4)));
    }
    return output;
}

// The core Feistel function (f-function) of DES
uint32_t feistel_function(uint32_t right_half, uint64_t round_key) {
    uint64_t expanded_right;
    perform_expansion(&expanded_right, right_half);

    uint64_t xored = expanded_right ^ round_key;

    uint32_t s_box_output = perform_s_box(xored);

    uint32_t p_box_output = 0;
    for (int i = 0; i < 32; i++) {
        int pos = P_TABLE[i] - 1;
        uint32_t bit = (s_box_output >> (31 - pos)) & 1;
        p_box_output |= (bit << (31 - i));
    }

    return p_box_output;
}

// Main encryption function
void des_encrypt_block(uint64_t *ciphertext, uint64_t plaintext, uint64_t keys[16]) {
    uint64_t ip_result;
    perform_permutation(&ip_result, plaintext, IP_TABLE, 64);

    uint32_t left_half = (uint32_t)(ip_result >> 32);
    uint32_t right_half = (uint32_t)ip_result;

    for (int i = 0; i < 16; i++) {
        uint32_t temp = left_half;
        left_half = right_half;
        right_half = temp ^ feistel_function(right_half, keys[i]);
    }

    uint64_t temp_combined = ((uint64_t)right_half << 32) | left_half;

    perform_permutation(ciphertext, temp_combined, FP_TABLE, 64);
}

// Main decryption function (same as encryption but with reversed keys)
void des_decrypt_block(uint64_t *plaintext, uint64_t ciphertext, uint64_t keys[16]) {
    uint64_t ip_result;
    perform_permutation(&ip_result, ciphertext, IP_TABLE, 64);

    uint32_t left_half = (uint32_t)(ip_result >> 32);
    uint32_t right_half = (uint32_t)ip_result;

    for (int i = 15; i >= 0; i--) {
        uint32_t temp = right_half;
        right_half = left_half;
        left_half = temp ^ feistel_function(left_half, keys[i]);
    }

    uint64_t temp_combined = ((uint64_t)right_half << 32) | left_half;

    perform_permutation(plaintext, temp_combined, FP_TABLE, 64);
}

// Function to print a 64-bit block in binary format
void print_block_binary(const char* label, uint64_t block) {
    printf("%s: ", label);
    for (int i = 63; i >= 0; i--) {
        printf("%c", ((block >> i) & 1) ? '1' : '0');
        if (i % 8 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}

int main() {
    // Note: The key schedule generation is not fully implemented here for brevity.
    // It is a complex process of its own involving PC-1, PC-2, and left shifts.
    // The keys array is a placeholder for 16 48-bit subkeys.
    uint64_t keys[16];
    memset(keys, 0, sizeof(keys));

    printf("--- DES (Data Encryption Standard) Implementation ---\n\n");
    printf("Note: This program provides a simplified, non-robust implementation of DES logic.\n");
    printf("      The key schedule is not fully implemented. Subkeys are placeholders.\n\n");

    uint64_t plaintext = 0x1234567890ABCDEF; // Example 64-bit plaintext
    uint64_t ciphertext;
    uint64_t decrypted_plaintext;

    print_block_binary("Original Plaintext", plaintext);
    printf("Original Plaintext (Hex): 0x%016llX\n\n", plaintext);

    // Placeholder for subkeys
    for (int i = 0; i < 16; i++) {
        keys[i] = 0x111111111111ULL;
    }

    // Encryption
    des_encrypt_block(&ciphertext, plaintext, keys);
    print_block_binary("Encrypted Ciphertext", ciphertext);
    printf("Encrypted Ciphertext (Hex): 0x%016llX\n\n", ciphertext);

    // Decryption
    des_decrypt_block(&decrypted_plaintext, ciphertext, keys);
    print_block_binary("Decrypted Plaintext", decrypted_plaintext);
    printf("Decrypted Plaintext (Hex): 0x%016llX\n\n", decrypted_plaintext);

    if (plaintext == decrypted_plaintext) {
        printf("Success: Decrypted text matches the original plaintext.\n");
    } else {
        printf("Failure: Decrypted text does not match the original plaintext.\n");
    }

    return 0;
}
