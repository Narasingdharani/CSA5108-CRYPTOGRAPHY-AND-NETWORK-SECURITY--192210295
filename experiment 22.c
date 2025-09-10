#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void print_binary(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d", data[i]);
        if ((i + 1) % 4 == 0) {
            printf(" ");
        }
    }
}


const int P10_BOX[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8_BOX[] = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP_BOX[] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_INV_BOX[] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP_BOX[] = {4, 1, 2, 3, 2, 3, 4, 1};
const int P4_BOX[] = {2, 4, 3, 1};

const int S0_BOX[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

const int S1_BOX[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Function to perform a permutation on an array of bits
void permute(const unsigned char *input, unsigned char *output, const int *pbox, int in_len, int out_len) {
    for (int i = 0; i < out_len; i++) {
        output[i] = input[pbox[i] - 1];
    }
}

// Function to left shift a binary array
void left_shift(unsigned char *arr, int len, int num_shifts) {
    unsigned char temp[len];
    memcpy(temp, arr, len);
    for (int i = 0; i < len; i++) {
        arr[i] = temp[(i + num_shifts) % len];
    }
}

// Function to generate subkeys K1 and K2
void generate_subkeys(unsigned char *key, unsigned char *subkey1, unsigned char *subkey2) {
    unsigned char p10_key[10];
    permute(key, p10_key, P10_BOX, 10, 10);

    unsigned char left_half[5], right_half[5];
    memcpy(left_half, p10_key, 5);
    memcpy(right_half, p10_key + 5, 5);

    left_shift(left_half, 5, 1);
    left_shift(right_half, 5, 1);

    unsigned char shifted_key_1[10];
    memcpy(shifted_key_1, left_half, 5);
    memcpy(shifted_key_1 + 5, right_half, 5);
    permute(shifted_key_1, subkey1, P8_BOX, 10, 8);

    left_shift(left_half, 5, 2);
    left_shift(right_half, 5, 2);

    unsigned char shifted_key_2[10];
    memcpy(shifted_key_2, left_half, 5);
    memcpy(shifted_key_2 + 5, right_half, 5);
    permute(shifted_key_2, subkey2, P8_BOX, 10, 8);
}

// Fk function
void fk(unsigned char *input, unsigned char *subkey, unsigned char *output) {
    unsigned char ep_output[8];
    permute(input + 4, ep_output, EP_BOX, 4, 8);

    unsigned char xor_result[8];
    for (int i = 0; i < 8; i++) {
        xor_result[i] = ep_output[i] ^ subkey[i];
    }

    // S-Boxes
    int s0_row = (xor_result[0] << 1) | xor_result[3];
    int s0_col = (xor_result[1] << 1) | xor_result[2];
    int s1_row = (xor_result[4] << 1) | xor_result[7];
    int s1_col = (xor_result[5] << 1) | xor_result[6];

    unsigned char s0_val[2], s1_val[2];
    s0_val[0] = (S0_BOX[s0_row][s0_col] >> 1) & 1;
    s0_val[1] = S0_BOX[s0_row][s0_col] & 1;
    s1_val[0] = (S1_BOX[s1_row][s1_col] >> 1) & 1;
    s1_val[1] = S1_BOX[s1_row][s1_col] & 1;
    
    unsigned char s_box_output[4];
    memcpy(s_box_output, s0_val, 2);
    memcpy(s_box_output + 2, s1_val, 2);

    unsigned char p4_output[4];
    permute(s_box_output, p4_output, P4_BOX, 4, 4);

    for (int i = 0; i < 4; i++) {
        output[i] = input[i] ^ p4_output[i];
        output[i + 4] = input[i + 4];
    }
}

// Function for S-DES encryption of a single block
void sdes_encrypt_block(unsigned char *plaintext_block, unsigned char *k1, unsigned char *k2, unsigned char *ciphertext_block) {
    unsigned char ip_output[8];
    permute(plaintext_block, ip_output, IP_BOX, 8, 8);

    unsigned char round1_output[8];
    fk(ip_output, k1, round1_output);
    
    // Swap
    unsigned char temp[4];
    memcpy(temp, round1_output, 4);
    memcpy(round1_output, round1_output + 4, 4);
    memcpy(round1_output + 4, temp, 4);

    unsigned char round2_output[8];
    fk(round1_output, k2, round2_output);
    
    permute(round2_output, ciphertext_block, IP_INV_BOX, 8, 8);
}

// Function for S-DES decryption of a single block
void sdes_decrypt_block(unsigned char *ciphertext_block, unsigned char *k1, unsigned char *k2, unsigned char *plaintext_block) {
    // S-DES decryption is identical to encryption but with keys in reverse order
    sdes_encrypt_block(ciphertext_block, k2, k1, plaintext_block);
}

// Function for CBC Encryption
void cbc_encrypt(unsigned char *plaintext, int len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    unsigned char k1[8], k2[8];
    generate_subkeys(key, k1, k2);
    
    unsigned char prev_block[8];
    memcpy(prev_block, iv, 8);

    for (int i = 0; i < len; i += 8) {
        unsigned char xored_block[8];
        for (int j = 0; j < 8; j++) {
            xored_block[j] = plaintext[i + j] ^ prev_block[j];
        }
        sdes_encrypt_block(xored_block, k1, k2, ciphertext + i);
        memcpy(prev_block, ciphertext + i, 8);
    }
}

// Function for CBC Decryption
void cbc_decrypt(unsigned char *ciphertext, int len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    unsigned char k1[8], k2[8];
    generate_subkeys(key, k1, k2);

    unsigned char prev_block[8];
    memcpy(prev_block, iv, 8);
    
    unsigned char temp_ciphertext_block[8];

    for (int i = 0; i < len; i += 8) {
        memcpy(temp_ciphertext_block, ciphertext + i, 8);
        unsigned char decrypted_block[8];
        sdes_decrypt_block(temp_ciphertext_block, k1, k2, decrypted_block);

        for (int j = 0; j < 8; j++) {
            plaintext[i + j] = decrypted_block[j] ^ prev_block[j];
        }
        memcpy(prev_block, temp_ciphertext_block, 8);
    }
}


int main() {
    // Test data in binary format
    unsigned char plaintext[16] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1};
    unsigned char key[10] = {0, 1, 1, 1, 1, 1, 1, 1, 0, 1};
    unsigned char iv[8] = {1, 0, 1, 0, 1, 0, 1, 0};

    unsigned char ciphertext[16];
    unsigned char decryptedtext[16];

    printf("Plaintext:        ");
    print_binary(plaintext, 16);
    printf("\nKey:              ");
    print_binary(key, 10);
    printf("\nIV:               ");
    print_binary(iv, 8);

    // CBC Encryption
    cbc_encrypt(plaintext, 16, key, iv, ciphertext);

    printf("\n\nCiphertext (expected: 1111 0100 0000 1011):\n");
    printf("Encrypted:        ");
    print_binary(ciphertext, 16);

    // CBC Decryption
    cbc_decrypt(ciphertext, 16, key, iv, decryptedtext);

    printf("\n\nDecrypted text:\n");
    printf("Decrypted:        ");
    print_binary(decryptedtext, 16);
    printf("\n\n");

    if (memcmp(plaintext, decryptedtext, 16) == 0) {
        printf("Decryption successful! Plaintext and decrypted text match.\n");
    } else {
        printf("Decryption failed.\n");
    }

    return 0;
}
