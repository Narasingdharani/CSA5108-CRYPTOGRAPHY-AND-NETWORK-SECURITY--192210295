#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BLOCK_SIZE 8

#define CFB_SEGMENT_SIZE 1

void mock_encrypt_block(unsigned char *plaintext, unsigned char *key, unsigned char *ciphertext) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        ciphertext[i] = plaintext[i] ^ key[i];
    }
}

void mock_decrypt_block(unsigned char *ciphertext, unsigned char *key, unsigned char *plaintext) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        plaintext[i] = ciphertext[i] ^ key[i];
    }
}


unsigned char* pad_plaintext(const unsigned char *plaintext, size_t plaintext_len, size_t *padded_len) {
    size_t padding_needed = BLOCK_SIZE - (plaintext_len % BLOCK_SIZE);
    if (padding_needed == 0) {
 
        padding_needed = BLOCK_SIZE;
    }

    *padded_len = plaintext_len + padding_needed;
    unsigned char* padded_text = (unsigned char*)malloc(*padded_len);
    if (!padded_text) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Copy original plaintext
    memcpy(padded_text, plaintext, plaintext_len);

    // Apply padding: 1 bit followed by 0s
    padded_text[plaintext_len] = 0x80; // 0x80 is 10000000 in binary
    for (size_t i = 1; i < padding_needed; i++) {
        padded_text[plaintext_len + i] = 0x00;
    }

    return padded_text;
}

// --- Unpadding Function ---
void unpad_plaintext(unsigned char *plaintext, size_t *padded_len) {
    // Find the last block
    size_t last_block_start = *padded_len - BLOCK_SIZE;
    size_t first_one_bit_index = 0;

    // Scan backwards from the end to find the padding '1' bit
    for (int i = BLOCK_SIZE - 1; i >= 0; i--) {
        if (plaintext[last_block_start + i] == 0x80) {
            first_one_bit_index = last_block_start + i;
            break;
        }
    }
    // Update the length to remove padding
    *padded_len = first_one_bit_index;
}

// --- Helper function to print hex data ---
void print_hex(const unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// --- ECB Mode ---
void ecb_encrypt(const unsigned char *plaintext, size_t len, unsigned char *key, unsigned char *ciphertext) {
    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        mock_encrypt_block((unsigned char*)plaintext + i, key, ciphertext + i);
    }
}

void ecb_decrypt(const unsigned char *ciphertext, size_t len, unsigned char *key, unsigned char *plaintext) {
    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        mock_decrypt_block((unsigned char*)ciphertext + i, key, plaintext + i);
    }
}

// --- CBC Mode ---
void cbc_encrypt(const unsigned char *plaintext, size_t len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    unsigned char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char xored_block[BLOCK_SIZE];
        for (int j = 0; j < BLOCK_SIZE; j++) {
            xored_block[j] = plaintext[i + j] ^ prev_block[j];
        }
        mock_encrypt_block(xored_block, key, ciphertext + i);
        memcpy(prev_block, ciphertext + i, BLOCK_SIZE);
    }
}

void cbc_decrypt(const unsigned char *ciphertext, size_t len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    unsigned char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char decrypted_block[BLOCK_SIZE];
        mock_decrypt_block((unsigned char*)ciphertext + i, key, decrypted_block);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i + j] = decrypted_block[j] ^ prev_block[j];
        }
        memcpy(prev_block, ciphertext + i, BLOCK_SIZE);
    }
}

// --- CFB Mode ---
void cfb_encrypt(const unsigned char *plaintext, size_t len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    unsigned char shift_register[BLOCK_SIZE];
    memcpy(shift_register, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += CFB_SEGMENT_SIZE) {
        unsigned char encrypted_shift_reg[BLOCK_SIZE];
        mock_encrypt_block(shift_register, key, encrypted_shift_reg);

        // XOR with a segment of the plaintext
        for (int j = 0; j < CFB_SEGMENT_SIZE; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ encrypted_shift_reg[j];
        }

        // Shift the register
        memmove(shift_register, shift_register + CFB_SEGMENT_SIZE, BLOCK_SIZE - CFB_SEGMENT_SIZE);
        memcpy(shift_register + (BLOCK_SIZE - CFB_SEGMENT_SIZE), ciphertext + i, CFB_SEGMENT_SIZE);
    }
}

void cfb_decrypt(const unsigned char *ciphertext, size_t len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    unsigned char shift_register[BLOCK_SIZE];
    memcpy(shift_register, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += CFB_SEGMENT_SIZE) {
        unsigned char encrypted_shift_reg[BLOCK_SIZE];
        mock_encrypt_block(shift_register, key, encrypted_shift_reg);
        
        // XOR with a segment of the ciphertext
        for (int j = 0; j < CFB_SEGMENT_SIZE; j++) {
            plaintext[i + j] = ciphertext[i + j] ^ encrypted_shift_reg[j];
        }

        // Shift the register
        memmove(shift_register, shift_register + CFB_SEGMENT_SIZE, BLOCK_SIZE - CFB_SEGMENT_SIZE);
        memcpy(shift_register + (BLOCK_SIZE - CFB_SEGMENT_SIZE), ciphertext + i, CFB_SEGMENT_SIZE);
    }
}

int main() {
    // A plaintext that is a multiple of the block size (8 bytes)
    const unsigned char original_plaintext_multiple_block[] = "ABCDEFGH";
    size_t original_len_multiple = strlen((char*)original_plaintext_multiple_block);

    // A plaintext that is NOT a multiple of the block size
    const unsigned char original_plaintext_not_multiple_block[] = "Hello";
    size_t original_len_not_multiple = strlen((char*)original_plaintext_not_multiple_block);

    unsigned char key[BLOCK_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    unsigned char iv[BLOCK_SIZE] = {0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};

    size_t padded_len;
    unsigned char *padded_plaintext;

    // --- Case 1: Plaintext that is already a multiple of the block size ---
    printf("--- Case 1: Plaintext is already a multiple of block size ---\n");
    printf("Original plaintext: %s\n", original_plaintext_multiple_block);
    printf("Original plaintext (hex): ");
    print_hex(original_plaintext_multiple_block, original_len_multiple);
    
    padded_plaintext = pad_plaintext(original_plaintext_multiple_block, original_len_multiple, &padded_len);
    printf("Padded plaintext (adds a full block): ");
    print_hex(padded_plaintext, padded_len);

    // --- ECB Mode Demonstration ---
    printf("\n--- ECB Mode Demonstration ---\n");
    unsigned char ecb_ciphertext[padded_len];
    ecb_encrypt(padded_plaintext, padded_len, key, ecb_ciphertext);
    printf("ECB Ciphertext: ");
    print_hex(ecb_ciphertext, padded_len);

    unsigned char ecb_decryptedtext[padded_len];
    ecb_decrypt(ecb_ciphertext, padded_len, key, ecb_decryptedtext);
    size_t ecb_decrypted_len = padded_len;
    unpad_plaintext(ecb_decryptedtext, &ecb_decrypted_len);
    printf("ECB Decrypted text: %s\n", ecb_decryptedtext);
    printf("ECB Decrypted text (hex): ");
    print_hex(ecb_decryptedtext, ecb_decrypted_len);

    // --- CBC Mode Demonstration ---
    printf("\n--- CBC Mode Demonstration ---\n");
    unsigned char cbc_ciphertext[padded_len];
    cbc_encrypt(padded_plaintext, padded_len, key, iv, cbc_ciphertext);
    printf("CBC Ciphertext: ");
    print_hex(cbc_ciphertext, padded_len);

    unsigned char cbc_decryptedtext[padded_len];
    cbc_decrypt(cbc_ciphertext, padded_len, key, iv, cbc_decryptedtext);
    size_t cbc_decrypted_len = padded_len;
    unpad_plaintext(cbc_decryptedtext, &cbc_decrypted_len);
    printf("CBC Decrypted text: %s\n", cbc_decryptedtext);
    printf("CBC Decrypted text (hex): ");
    print_hex(cbc_decryptedtext, cbc_decrypted_len);
    
    // --- CFB Mode Demonstration ---
    // Note: CFB padding is often handled differently, but we apply the same padding rule for consistency.
    printf("\n--- CFB Mode Demonstration (CFB Segment Size: %d) ---\n", CFB_SEGMENT_SIZE);
    unsigned char cfb_ciphertext[padded_len];
    cfb_encrypt(padded_plaintext, padded_len, key, iv, cfb_ciphertext);
    printf("CFB Ciphertext: ");
    print_hex(cfb_ciphertext, padded_len);

    unsigned char cfb_decryptedtext[padded_len];
    cfb_decrypt(cfb_ciphertext, padded_len, key, iv, cfb_decryptedtext);
    size_t cfb_decrypted_len = padded_len;
    unpad_plaintext(cfb_decryptedtext, &cfb_decrypted_len);
    printf("CFB Decrypted text: %s\n", cfb_decryptedtext);
    printf("CFB Decrypted text (hex): ");
    print_hex(cfb_decryptedtext, cfb_decrypted_len);

    free(padded_plaintext);

    // --- Case 2: Plaintext that is NOT a multiple of the block size ---
    printf("\n--- Case 2: Plaintext is NOT a multiple of block size ---\n");
    printf("Original plaintext: %s\n", original_plaintext_not_multiple_block);
    printf("Original plaintext (hex): ");
    print_hex(original_plaintext_not_multiple_block, original_len_not_multiple);

    padded_plaintext = pad_plaintext(original_plaintext_not_multiple_block, original_len_not_multiple, &padded_len);
    printf("Padded plaintext (padded to a full block): ");
    print_hex(padded_plaintext, padded_len);

    // Demonstrate ECB again with the new padded text
    unsigned char ecb_ciphertext2[padded_len];
    ecb_encrypt(padded_plaintext, padded_len, key, ecb_ciphertext2);
    printf("ECB Ciphertext: ");
    print_hex(ecb_ciphertext2, padded_len);

    unsigned char ecb_decryptedtext2[padded_len];
    ecb_decrypt(ecb_ciphertext2, padded_len, key, ecb_decryptedtext2);
    size_t ecb_decrypted_len2 = padded_len;
    unpad_plaintext(ecb_decryptedtext2, &ecb_decrypted_len2);
    printf("ECB Decrypted text: %s\n", ecb_decryptedtext2);
    printf("ECB Decrypted text (hex): ");
    print_hex(ecb_decryptedtext2, ecb_decrypted_len2);

    free(padded_plaintext);

    return 0;
}
