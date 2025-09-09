#include <stdio.h>
#include <string.h>

#define MOD 26


int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

int modInverse(int a, int m) {
    a = (a % m + m) % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}


int inverseMatrix(int m[2][2], int inv[2][2]) {
    int det = (m[0][0]*m[1][1] - m[0][1]*m[1][0]) % MOD;
    if (det < 0) det += MOD;
    int invDet = modInverse(det, MOD);
    if (invDet == -1) return 0;

    inv[0][0] = ( m[1][1] * invDet) % MOD;
    inv[0][1] = (-m[0][1] * invDet) % MOD;
    inv[1][0] = (-m[1][0] * invDet) % MOD;
    inv[1][1] = ( m[0][0] * invDet) % MOD;

    for (int i=0;i<2;i++) {
        for (int j=0;j<2;j++) {
            if (inv[i][j] < 0) inv[i][j] += MOD;
        }
    }
    return 1;
}

// Multiply two 2x2 matrices mod 26
void multiplyMatrix(int a[2][2], int b[2][2], int res[2][2]) {
    for (int i=0;i<2;i++) {
        for (int j=0;j<2;j++) {
            res[i][j] = 0;
            for (int k=0;k<2;k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
            res[i][j] %= MOD;
        }
    }
}

int main() {
    // Known plaintext digraphs
    int P[2][2] = {{7,11},{4,15}};   // HE (7,4), LP (11,15)
    // Corresponding ciphertext digraphs
    int C[2][2] = {{23,13},{12,0}};  // XM (23,12), NA (13,0)

    int Pinv[2][2], K[2][2];

    printf("Known Plaintext Attack on Hill Cipher\n");
    printf("--------------------------------------\n");

    if (!inverseMatrix(P, Pinv)) {
        printf("Plaintext matrix not invertible mod 26!\n");
        return 0;
    }

    // K = C * P^-1
    multiplyMatrix(C, Pinv, K);

    printf("Recovered Key Matrix K:\n");
    for (int i=0;i<2;i++) {
        for (int j=0;j<2;j++) {
            printf("%3d ", K[i][j]);
        }
        printf("\n");
    }

    return 0;
}
