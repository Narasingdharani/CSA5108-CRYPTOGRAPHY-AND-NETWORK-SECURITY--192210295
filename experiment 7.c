#include <stdio.h>
#include <string.h>


char substitute(char c) {
 
    switch (c) {
        case '8': return 'a';
        case '(': return 'b';
        case '†': return 'c';
        case '3': return 'd';
        case '6': return 'e';
        case '*': return 'f';
        case '5': return 'g';
        case '4': return 'h';
        case '‡': return 'i';
        case '¶': return 'l';
        case ';': return 'n';
        case ')': return 'o';
        case '0': return 'r';
        case '9': return 's';
        case ':': return 't';
        case '?': return 'u';
        case '1': return 'v';
        case '2': return 'y';
        case ']': return 'w';
        case '—': return 'm';
        case '.': return 'p';
        default: return c; 
    }
}

int main() {
    char ciphertext[] =
        "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83 "
        "(88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8* "
        ";4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81 "
        "(‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?;";

    printf("Decrypted text:\n");
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        printf("%c", substitute(ciphertext[i]));
    }
    printf("\n");
    return 0;
}
