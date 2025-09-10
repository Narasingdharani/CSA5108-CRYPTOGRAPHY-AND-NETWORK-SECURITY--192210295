#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define BLOCK_SIZE 8 
#define SEGMENT_SIZE 1 

void mock_cipher_encrypt(uint8_t* output, const uint8_t* input, const uint8_t* key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i];
    }
}

void mock_cipher_decrypt(uint8_t* output, const uint8_t* input, const uint8_t* key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i];
    }
}

size_t add_padding(uint8_t* plaintext, size_t plaintext_len) {
    size_t new_len = plaintext_len;
    size_t rem = plaintext_len % BLOCK_SIZE;
    size_t padding_len = BLOCK_SIZE - rem;

    // Pad every message, even if the last block is full
    if (rem == 0) {
        padding_len = BLOCK_SIZE;
    }

    // Allocate memory for the padded plaintext
    uint8_t* padded = realloc(plaintext, plaintext_len + padding_len);
    if (padded == NULL) {
        perror("Failed to reallocate memory for padding");
        return 0;
    }
    plaintext = padded;

    // Append '1' bit (0x80) to the end of the plaintext, and pad with '0' bits
    plaintext[plaintext_len] = 0x80;
    for (size_t i = 1; i < padding_len; i++) {
        plaintext[plaintext_len + i] = 0x00;
    }

    return plaintext_len + padding_len;
}

// Function to print a block in hexadecimal
void print_block(const char* label, const uint8_t* block, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02X", block[i]);
    }
    printf("\n");
}

// --- Cipher Modes of Operation ---

// Electronic Codebook (ECB) Mode
// Encrypts each block independently
void encrypt_ecb(uint8_t* ciphertext, const uint8_t* plaintext, size_t len, const uint8_t* key) {
    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        mock_cipher_encrypt(ciphertext + i, plaintext + i, key);
    }
}

void decrypt_ecb(uint8_t* plaintext, const uint8_t* ciphertext, size_t len, const uint8_t* key) {
    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        mock_cipher_decrypt(plaintext + i, ciphertext + i, key);
    }
}

// Cipher Block Chaining (CBC) Mode
// Each plaintext block is XORed with the previous ciphertext block before encryption
void encrypt_cbc(uint8_t* ciphertext, const uint8_t* plaintext, size_t len, const uint8_t* key, const uint8_t* iv) {
    uint8_t prev_ciphertext[BLOCK_SIZE];
    memcpy(prev_ciphertext, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        uint8_t xored_block[BLOCK_SIZE];
        for (int j = 0; j < BLOCK_SIZE; j++) {
            xored_block[j] = plaintext[i + j] ^ prev_ciphertext[j];
        }
        mock_cipher_encrypt(ciphertext + i, xored_block, key);
        memcpy(prev_ciphertext, ciphertext + i, BLOCK_SIZE);
    }
}

void decrypt_cbc(uint8_t* plaintext, const uint8_t* ciphertext, size_t len, const uint8_t* key, const uint8_t* iv) {
    uint8_t prev_ciphertext[BLOCK_SIZE];
    memcpy(prev_ciphertext, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        uint8_t decrypted_block[BLOCK_SIZE];
        mock_cipher_decrypt(decrypted_block, ciphertext + i, key);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i + j] = decrypted_block[j] ^ prev_ciphertext[j];
        }
        memcpy(prev_ciphertext, ciphertext + i, BLOCK_SIZE);
    }
}

// Cipher Feedback (CFB) Mode
// Encrypts an IV, XORs with plaintext segment, uses the resulting ciphertext segment as the next IV
void encrypt_cfb(uint8_t* ciphertext, const uint8_t* plaintext, size_t len, const uint8_t* key, const uint8_t* iv) {
    uint8_t prev_cipher_block[BLOCK_SIZE];
    memcpy(prev_cipher_block, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += SEGMENT_SIZE) {
        uint8_t stream[BLOCK_SIZE];
        mock_cipher_encrypt(stream, prev_cipher_block, key);

        for (int j = 0; j < SEGMENT_SIZE; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ stream[j];
        }

        // Update the previous cipher block
        memmove(prev_cipher_block, prev_cipher_block + SEGMENT_SIZE, BLOCK_SIZE - SEGMENT_SIZE);
        memcpy(prev_cipher_block + BLOCK_SIZE - SEGMENT_SIZE, ciphertext + i, SEGMENT_SIZE);
    }
}

void decrypt_cfb(uint8_t* plaintext, const uint8_t* ciphertext, size_t len, const uint8_t* key, const uint8_t* iv) {
    uint8_t prev_cipher_block[BLOCK_SIZE];
    memcpy(prev_cipher_block, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += SEGMENT_SIZE) {
        uint8_t stream[BLOCK_SIZE];
        mock_cipher_encrypt(stream, prev_cipher_block, key);

        for (int j = 0; j < SEGMENT_SIZE; j++) {
            plaintext[i + j] = ciphertext[i + j] ^ stream[j];
        }

        // Update the previous cipher block
        memmove(prev_cipher_block, prev_cipher_block + SEGMENT_SIZE, BLOCK_SIZE - SEGMENT_SIZE);
        memcpy(prev_cipher_block + BLOCK_SIZE - SEGMENT_SIZE, ciphertext + i, SEGMENT_SIZE);
    }
}


int main() {
    uint8_t key[BLOCK_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint8_t iv[BLOCK_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    uint8_t* plaintext = (uint8_t*)malloc(sizeof(uint8_t) * 10);
    if (!plaintext) return 1;
    memcpy(plaintext, "Hello!", 6);
    size_t plaintext_len = 6;
    
    printf("Original plaintext: 'Hello!' (length %zu)\n", plaintext_len);

    // Add padding to the plaintext
    size_t padded_len = add_padding(plaintext, plaintext_len);
    printf("Padded plaintext length: %zu bytes\n", padded_len);
    print_block("Padded Plaintext", plaintext, padded_len);
    printf("\n");

    uint8_t* ciphertext_ecb = (uint8_t*)malloc(padded_len);
    uint8_t* decrypted_ecb = (uint8_t*)malloc(padded_len);
    if (!ciphertext_ecb || !decrypted_ecb) return 1;
    printf("--- ECB Mode ---\n");
    encrypt_ecb(ciphertext_ecb, plaintext, padded_len, key);
    print_block("ECB Ciphertext", ciphertext_ecb, padded_len);
    decrypt_ecb(decrypted_ecb, ciphertext_ecb, padded_len, key);
    print_block("ECB Decrypted", decrypted_ecb, padded_len);
    printf("\n");

    uint8_t* ciphertext_cbc = (uint8_t*)malloc(padded_len);
    uint8_t* decrypted_cbc = (uint8_t*)malloc(padded_len);
    if (!ciphertext_cbc || !decrypted_cbc) return 1;
    printf("--- CBC Mode ---\n");
    encrypt_cbc(ciphertext_cbc, plaintext, padded_len, key, iv);
    print_block("CBC Ciphertext", ciphertext_cbc, padded_len);
    decrypt_cbc(decrypted_cbc, ciphertext_cbc, padded_len, key, iv);
    print_block("CBC Decrypted", decrypted_cbc, padded_len);
    printf("\n");

    // CFB mode uses segment size, so padding is still based on block size.
    uint8_t* ciphertext_cfb = (uint8_t*)malloc(padded_len);
    uint8_t* decrypted_cfb = (uint8_t*)malloc(padded_len);
    if (!ciphertext_cfb || !decrypted_cfb) return 1;
    printf("--- CFB Mode (8-bit segment) ---\n");
    encrypt_cfb(ciphertext_cfb, plaintext, padded_len, key, iv);
    print_block("CFB Ciphertext", ciphertext_cfb, padded_len);
    decrypt_cfb(decrypted_cfb, ciphertext_cfb, padded_len, key, iv);
    print_block("CFB Decrypted", decrypted_cfb, padded_len);
    printf("\n");

    free(plaintext);
    free(ciphertext_ecb);
    free(decrypted_ecb);
    free(ciphertext_cbc);
    free(decrypted_cbc);
    free(ciphertext_cfb);
    free(decrypted_cfb);

    return 0;
}
