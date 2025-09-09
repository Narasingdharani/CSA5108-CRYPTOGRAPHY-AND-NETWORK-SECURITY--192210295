#include <stdio.h>


int main() {
    printf("--- Error Propagation in ECB and CBC ---\n");

    printf("ECB Mode:\n");
    printf("Error in C1 -> Only P1 corrupted\n\n");

    printf("CBC Mode:\n");
    printf("Error in C1 -> Corrupts P1 and P2\n");
    printf("No effect beyond P2\n\n");

    printf("If error in P1 before encryption:\n");
    printf("This propagates into all ciphertext blocks (C1, C2, ...)\n");
    printf("At receiver: entire message may be corrupted\n");

    return 0;
}
