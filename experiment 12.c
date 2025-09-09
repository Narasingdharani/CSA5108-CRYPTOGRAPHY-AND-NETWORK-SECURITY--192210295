#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26


int key[2][2] = {{9,4},{5,7}};
int invKey[2][2] = {{5,12},{25,25}}; 


int charToNum(char c) { return toupper(c) - 'A'; }


char numToChar(int n) { return (n % MOD) + 'A'; }

// Encrypt digraph
void encryptPair(char a, char b) {
    int p1 = charToNum(a);
    int p2 = charToNum(b);
    int c1 = (key[0][0]*p1 + key[0][1]*p2) % MOD;
    int c2 = (key[1][0]*p1 + key[1][1]*p2) % MOD;
    printf("%c%c", numToChar(c1), numToChar(c2));
}

// Decrypt digraph
void decryptPair(char a, char b) {
    int c1 = charToNum(a);
    int c2 = charToNum(b);
    int p1 = (invKey[0][0]*c1 + invKey[0][1]*c2) % MOD;
    int p2 = (invKey[1][0]*c1 + invKey[1][1]*c2) % MOD;
    printf("%c%c", numToChar(p1), numToChar(p2));
}

int main() {
    char text[] = "meet me at the usual place at ten rather than eight oclock";
    char filtered[200];
    int n=0;

    // filter plaintext
    for(int i=0;i<strlen(text);i++){
        if(isalpha(text[i])) filtered[n++] = toupper(text[i]);
    }
    if(n%2!=0) filtered[n++]='X'; // pad
    filtered[n]='\0';

    printf("Plaintext: %s\n", filtered);

    printf("\nCiphertext: ");
    for(int i=0;i<n;i+=2) encryptPair(filtered[i], filtered[i+1]);

    printf("\nDecrypted : ");
    for(int i=0;i<n;i+=2) decryptPair(filtered[i], filtered[i+1]);
    printf("\n");

    return 0;
}
