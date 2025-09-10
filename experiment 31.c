#include <stdio.h>
#include <string.h>


#define R_64 0x1B
#define R_128 0x87

void mock_encrypt(unsigned char *output, int block_size) {
    if (block_size == 16) { 
        unsigned char mock_L_128[16] = {0x61, 0x51, 0x61, 0x6d, 0x6e, 0x65, 0x66, 0x72, 0x61, 0x6d, 0x65, 0x6e, 0x74, 0x20, 0x62, 0x6f};
        memcpy(output, mock_L_128, 16);
    } else if (block_size == 8) { 
        unsigned char mock_L_64[8] = {0x91, 0x1f, 0x76, 0x48, 0x1c, 0x8f, 0x93, 0xf6};
        memcpy(output, mock_L_64, 8);
    }
}


void generate_subkey(unsigned char *subkey_out, const unsigned char *input_block, int block_size) {

    int msb_set = (input_block[0] & 0x80) != 0;

    // Perform the left shift
    for (int i = 0; i < block_size - 1; i++) {
        subkey_out[i] = (input_block[i] << 1) | (input_block[i+1] >> 7);
    }
    subkey_out[block_size - 1] = input_block[block_size - 1] << 1;

    // Conditionally XOR with the constant R
    if (msb_set) {
        if (block_size == 16) {
            subkey_out[block_size - 1] ^= R_128;
        } else if (block_size == 8) {
            subkey_out[block_size - 1] ^= R_64;
        }
    }
}

// Function to print a block in hexadecimal format
void print_block(const char* label, const unsigned char* block, int block_size) {
    printf("%s: ", label);
    for (int i = 0; i < block_size; i++) {
        printf("%02X ", block[i]);
    }
    printf("\n");
}

int main() {
    // --- 128-bit block size (e.g., AES) ---
    int block_size_128 = 16;
    unsigned char L_128[16];
    unsigned char K1_128[16];
    unsigned char K2_128[16];

    printf("--- CMAC Subkey Generation for 128-bit Block Size ---\n\n");
    
    // Step 1: Encrypt a block of all zeros to get L
    mock_encrypt(L_128, block_size_128);
    print_block("Mock Encrypted L", L_128, block_size_128);

    // Step 2: Generate K1 from L
    generate_subkey(K1_128, L_128, block_size_128);
    print_block("Subkey K1", K1_128, block_size_128);

    // Step 3: Generate K2 from K1
    generate_subkey(K2_128, K1_128, block_size_128);
    print_block("Subkey K2", K2_128, block_size_128);

    printf("\n");

    // --- 64-bit block size (e.g., DES) ---
    int block_size_64 = 8;
    unsigned char L_64[8];
    unsigned char K1_64[8];
    unsigned char K2_64[8];

    printf("--- CMAC Subkey Generation for 64-bit Block Size ---\n\n");

    // Step 1: Encrypt a block of all zeros to get L
    mock_encrypt(L_64, block_size_64);
    print_block("Mock Encrypted L", L_64, block_size_64);

    // Step 2: Generate K1 from L
    generate_subkey(K1_64, L_64, block_size_64);
    print_block("Subkey K1", K1_64, block_size_64);

    // Step 3: Generate K2 from K1
    generate_subkey(K2_64, K1_64, block_size_64);
    print_block("Subkey K2", K2_64, block_size_64);

    return 0;
}
