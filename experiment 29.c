#include <stdio.h>
#include <stdbool.h>

#define LANES_X 5
#define LANES_Y 5
#define TOTAL_LANES (LANES_X * LANES_Y)
#define RATE_LANES 16
#define CAPACITY_LANES (TOTAL_LANES - RATE_LANES)


bool state[LANES_X][LANES_Y];
bool new_state[LANES_X][LANES_Y];


void print_state(int round) {
    printf("State after round %d:\n", round);
    for (int y = 0; y < LANES_Y; y++) {
        for (int x = 0; x < LANES_X; x++) {
            printf("%d ", state[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

// Check if all capacity lanes have at least one nonzero bit
bool all_capacity_lanes_nonzero() {
    // In our simplified model, the capacity lanes are the last 9 lanes (starting at index 16)
    int count = 0;
    for (int i = 0; i < TOTAL_LANES; i++) {
        int x = i % LANES_X;
        int y = i / LANES_X;
        if (i >= RATE_LANES) {
            if (state[x][y]) {
                count++;
            }
        }
    }
    return count == CAPACITY_LANES;
}

// Simplified Theta (?) step simulation
// The non-zero-ness of a column parity check C[x] propagates to all lanes in that column,
// as well as to the adjacent columns due to the XOR with D[x].
void theta_step() {
    bool C[LANES_X];
    bool D[LANES_X];

    // Compute C[x] - the boolean OR of all lanes in column x
    for (int x = 0; x < LANES_X; x++) {
        C[x] = false;
        for (int y = 0; y < LANES_Y; y++) {
            C[x] = C[x] || state[x][y];
        }
    }

    // Compute D[x] - the boolean OR of C[x-1] and C[x+1] (with rotation)
    // The rotation and XOR in the real algorithm means any non-zero C[x] affects D[x]
    for (int x = 0; x < LANES_X; x++) {
        D[x] = C[(x + LANES_X - 1) % LANES_X] || C[(x + 1) % LANES_X];
    }

    // Apply D[x] to the state
    for (int x = 0; x < LANES_X; x++) {
        for (int y = 0; y < LANES_Y; y++) {
            new_state[x][y] = state[x][y] || D[x];
        }
    }
    // Update the state for the next step
    for (int x = 0; x < LANES_X; x++) {
        for (int y = 0; y < LANES_Y; y++) {
            state[x][y] = new_state[x][y];
        }
    }
}

// Simplified Chi (?) step simulation
// The non-zero-ness of one lane propagates to its neighbors
// S'[x,y] = S[x,y] XOR ((~S[x+1,y]) AND S[x+2,y])
// In our simplified boolean logic, this is equivalent to:
// S'[x,y] = S[x,y] || (S[x+1,y] && S[x+2,y]) || (S[x][y] && !S[x+1][y]) || ...
// The simplest propagation model is that if any of the three lanes are non-zero,
// the output lane might become non-zero.
void chi_step() {
    for (int y = 0; y < LANES_Y; y++) {
        for (int x = 0; x < LANES_X; x++) {
            bool s_x = state[x][y];
            bool s_x1 = state[(x + 1) % LANES_X][y];
            bool s_x2 = state[(x + 2) % LANES_X][y];
            
            // This is a simplified logic. If s_x is non-zero, it will affect the result.
            // if (~s_x1 && s_x2) is true, it will also affect the result.
            // The boolean OR captures the 'at least one nonzero bit' condition.
            new_state[x][y] = s_x || (s_x2 && !s_x1);
        }
    }
    for (int x = 0; x < LANES_X; x++) {
        for (int y = 0; y < LANES_Y; y++) {
            state[x][y] = new_state[x][y];
        }
    }
}

int main() {
    // Initialize the state matrix to all zeros
    for (int x = 0; x < LANES_X; x++) {
        for (int y = 0; y < LANES_Y; y++) {
            state[x][y] = false;
        }
    }

    printf("Simulating SHA-3 bit propagation with block size 1024 bits.\n\n");

    // Absorption of the first message block (P0)
    // The problem states that each lane in P0 has at least one nonzero bit.
    // This is XORed into the first 16 lanes of the state (the rate portion).
    for (int i = 0; i < RATE_LANES; i++) {
        int x = i % LANES_X;
        int y = i / LANES_X;
        state[x][y] = true;
    }

    printf("Initial state after absorbing the first message block (P0):\n");
    printf("Rate lanes (first 16) are now non-zero. Capacity lanes are all zero.\n");
    print_state(0);

    int rounds = 0;
    while (!all_capacity_lanes_nonzero()) {
        rounds++;
        
        // Apply the round function steps that cause propagation
        // Per the prompt, we ignore the permutation steps (rho and pi).
        theta_step();
        chi_step();
        // Iota step does not cause propagation, so it is ignored in this model.
    }

    printf("Propagation is complete.\n");
    printf("It took %d rounds for all capacity lanes to have at least one nonzero bit.\n", rounds);

    return 0;
}
