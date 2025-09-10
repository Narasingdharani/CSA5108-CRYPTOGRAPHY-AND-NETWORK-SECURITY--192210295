#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

int main() {
    unsigned char key[24] = "123456789012345678901234"; // 3DES key
    unsigned char iv[8] = "ABCDEFGH";                  // IV for CBC

    unsigned char plaintext[] = "HELLO CRYPTOGRAPHY!";
    unsigned char ciphertext[128];
    unsigned char decrypted[128];
    int outlen, tmplen;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    // Encryption
    EVP_EncryptInit_ex(ctx, EVP_des_ede3_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &outlen, plaintext, strlen((char*)plaintext));
    EVP_EncryptFinal_ex(ctx, ciphertext + outlen, &tmplen);
    outlen += tmplen;

    printf("Ciphertext: ");
    for(int i=0;i<outlen;i++) printf("%02X ", ciphertext[i]);
    printf("\n");

    // Decryption
    EVP_DecryptInit_ex(ctx, EVP_des_ede3_cbc(), NULL, key, iv);
    int declen;
    EVP_DecryptUpdate(ctx, decrypted, &declen, ciphertext, outlen);
    EVP_DecryptFinal_ex(ctx, decrypted + declen, &tmplen);
    declen += tmplen;
    decrypted[declen] = '\0';

    printf("Decrypted: %s\n", decrypted);

    EVP_CIPHER_CTX_free(ctx);
    return 0;
}
