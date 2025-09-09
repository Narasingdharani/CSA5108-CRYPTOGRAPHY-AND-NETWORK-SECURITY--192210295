#include <stdio.h>
#include <string.h>



int PC1[56] = {
    57,49,41,33,25,17,9,
    1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,
    19,11,3,60,52,44,36,
    63,55,47,39,31,23,15,
    7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,
    21,13,5,28,20,12,4
};


int PC2[48] = {
    14,17,11,24,1,5,
    3,28,15,6,21,10,
    23,19,12,4,26,8,
    16,7,27,20,13,2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32
};

// Shift schedule for 16 rounds
int shifts[16] = {
    1,1,2,2,2,2,2,2,
    1,2,2,2,2,2,2,1
};

// Left circular shift
void leftShift(int *arr, int size, int shifts) {
    int temp[28];
    for(int i=0;i<shifts;i++) {
        temp[0] = arr[0];
        for(int j=0;j<size-1;j++) arr[j] = arr[j+1];
        arr[size-1] = temp[0];
    }
}

// Generate 16 subkeys
void generateKeys(char *key64, int subkeys[16][48]) {
    int key56[56], C[28], D[28];

    // Apply PC-1
    for(int i=0;i<56;i++)
        key56[i] = key64[PC1[i]-1] - '0';

    // Split into C and D
    for(int i=0;i<28;i++) {
        C[i] = key56[i];
        D[i] = key56[i+28];
    }

    // Generate 16 keys
    for(int round=0; round<16; round++) {
        // Left shift
        leftShift(C, 28, shifts[round]);
        leftShift(D, 28, shifts[round]);

        int CD[56];
        for(int i=0;i<28;i++) {
            CD[i] = C[i];
            CD[i+28] = D[i];
        }

        // Apply PC-2
        for(int i=0;i<48;i++) {
            subkeys[round][i] = CD[PC2[i]-1];
        }
    }
}

// Print 48-bit subkey
void printKey(int key[48]) {
    for(int i=0;i<48;i++) printf("%d", key[i]);
    printf("\n");
}

int main() {
    char key64[65];
    int subkeys[16][48];

    printf("Enter 64-bit key (as 0/1 string): ");
    scanf("%64s", key64);

    generateKeys(key64, subkeys);

    printf("\n--- Encryption Subkeys (K1 -> K16) ---\n");
    for(int i=0;i<16;i++) {
        printf("K%2d: ", i+1);
        printKey(subkeys[i]);
    }

    printf("\n--- Decryption Subkeys (K16 -> K1) ---\n");
    for(int i=15;i>=0;i--) {
        printf("K%2d: ", i+1);
        printKey(subkeys[i]);
    }

    return 0;
}
