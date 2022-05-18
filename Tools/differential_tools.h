#include <stdio.h>

#define uint unsigned int

int** compute_table(const uint8_t* sBox, int** table, int bits) {
    for (int input = 0; input < (0b1 << bits); input++) {
        for (int alpha = 0; alpha < (0b1 << bits); alpha++) {
            uint beta = sBox[input ^ alpha] ^ sBox[input];
            table[alpha][beta] += 1;
        }
    }
    return table;
}