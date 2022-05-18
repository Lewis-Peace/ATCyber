#include <stdio.h>

void possible_values(unsigned int, unsigned int);
unsigned int permutation(unsigned int);

int main() {
    unsigned int input_diff = 0x1;

    for (unsigned int i = 0; i < 16; i++) {
        printf("Output difference %X\n", i);
        possible_values(input_diff, i);
        printf("\n\n");
    }
}

void possible_values(unsigned int input, unsigned int output) {
    for (unsigned int i = 0; i < 16; i++) {
        for (unsigned int j = 0; j < 16; j++) {
            if ((i ^ j) == input) {
                if ((permutation(i) ^ permutation(j)) == output) {
                    printf("%X, %X\n", i, j);
                }
            }
        }
    }
}

unsigned int permutation(unsigned int x) {
    
    unsigned int list[16] = {0xa,5,4,2,6,1,0xf,3,0xb,0xe,7,0,8,0xd,0xc,9};
    return list[x];
}