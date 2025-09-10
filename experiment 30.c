#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 8 
void mock_encrypt(unsigned char *output, const unsigned char *input, const unsigned char *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i];
    }
}


void cbc_mac(unsigned char *mac, const unsigned char *message, int message_len, const unsigned char *key) {
    unsigned char current_block[BLOCK_SIZE];
    unsigned char temp_block[BLOCK_SIZE];

    memset(current_block, 0, BLOCK_SIZE);

    int num_blocks = message_len / BLOCK_SIZE;
    
    for (int i = 0; i < num_blocks; i++) {

        for (int j = 0; j < BLOCK_SIZE; j++) {
            temp_block[j] = message[i * BLOCK_SIZE + j] ^ current_block[j];
        }

        // 2. Encrypt the result
        mock_encrypt(current_block, temp_block, key);
    }

    // The final output of the last encryption is the MAC
    memcpy(mac, current_block, BLOCK_SIZE);
}

// Function to print a block in hexadecimal format
void print_block(const char* label, const unsigned char* block) {
    printf("%s: ", label);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", block[i]);
    }
    printf("\n");
}

int main() {
    // Shared secret key
    unsigned char key[BLOCK_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    // Original one-block message
    unsigned char message_x[BLOCK_SIZE] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};

    // Variables to store MACs
    unsigned char mac_T[BLOCK_SIZE];
    unsigned char forged_mac[BLOCK_SIZE];
    
    printf("--- Demonstrating CBC-MAC Vulnerability ---\n\n");
    print_block("Secret Key", key);
    print_block("Original Message X", message_x);
    printf("\n");

    // Step 1: Compute the MAC for the one-block message X
    printf("1. Legitimate user computes T = MAC(K, X)\n");
    cbc_mac(mac_T, message_x, BLOCK_SIZE, key);
    print_block("Computed MAC T", mac_T);
    printf("\n");

    // --- Adversary's Actions ---
    // The adversary has observed the MAC T for the message X.
    // They can now forge a new valid message.

    // Step 2: Adversary constructs a new message X' = X || (X XOR T)
    unsigned char x_xor_t[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++) {
        x_xor_t[i] = message_x[i] ^ mac_T[i];
    }

    unsigned char forged_message[BLOCK_SIZE * 2];
    memcpy(forged_message, message_x, BLOCK_SIZE);
    memcpy(forged_message + BLOCK_SIZE, x_xor_t, BLOCK_SIZE);

    printf("2. Adversary forges a new message: X' = X || (X XOR T)\n");
    print_block("Forged Block (X XOR T)", x_xor_t);
    printf("\n");
    print_block("Forged Two-Block Message", forged_message);
    printf("\n");

    // Step 3: Adversary predicts the MAC for the forged message
    printf("3. Adversary knows the MAC for the new message will also be T.\n");
    printf("   This is because the first block of the forged message (X) produces T, ");
    printf("and the second block (X XOR T) then cancels out T.\n\n");

    // Step 4: The system computes the MAC for the forged message to verify it
    printf("4. System computes MAC(K, X || (X XOR T))\n");
    cbc_mac(forged_mac, forged_message, BLOCK_SIZE * 2, key);
    print_block("Verified MAC", forged_mac);

    // Final verification
    if (memcmp(mac_T, forged_mac, BLOCK_SIZE) == 0) {
        printf("\nRESULT: The forged MAC matches the original MAC T. The attack is successful!\n");
    } else {
        printf("\nRESULT: The MACs do not match. The attack was not successful.\n");
    }

    return 0;
}
