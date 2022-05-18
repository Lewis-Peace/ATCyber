/**
 * @file 04-05-2022.c
 * @date 2022-05-04
 * 
 * Cipher composition is:
 *  Add round key
 *  SBox
 *  Shiftrow
 *  Mixcol
 * 
 * The cipher takes a 64 bit plaintext and does a number of rounds with these steps:
 * 
 *  The round key is added by xoring to the text as input of the round.
 *  The SBox is established as AES SBox.
 *  Shiftrow is same as AES but there is the version where it is used and a versione where is not.
 *  Mixcol is based on this matrix:
 *      1 0 1 0
 *      0 1 1 0
 *      1 0 0 0
 *      0 0 1 1
 * 
 * The key round scheduling is based on a master key K and some round keys RK scheduled like this.
 *  kr = (k(r-1) ^ 0xf33ff33f) >>> 32
 * 
 *  RK = kr & 0000000000000000FFFFFFFFFFFFFFFF
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "04-05-2022.h"
#include "../Tools/operations.h"

#define uint unsigned int

int main() {
    __uint128_t master_key = 0x1;
    int rounds = 1;
    uint64_t alpha = 0x0;
    uint64_t plain_text = 0x5252525252525251;
    uint64_t cipher_text_1 = cipher(master_key, plain_text, rounds);
    uint64_t cipher_text_2 = cipher(master_key, plain_text, rounds);
    printf("%lx xor %lx = %lx\n", cipher_text_1, cipher_text_2, cipher_text_1 ^ cipher_text_2);
}

uint64_t cipher(__uint128_t master_key, uint64_t plain_text, int rounds) {
    uint64_t round_value = plain_text;
    for (int i = 0; i < rounds; i++)
    {
        round_value = cipher_round(round_value, i, master_key);
    }
    
    return round_value;
}

uint64_t cipher_round(uint64_t round_input, int round_number, __uint128_t master_key) {
    uint64_t value;
    value = ARK(round_input, master_key, round_number);
    printf("ARK -> %lx\n", value);
    value = SB(value);
    printf("SB -> %lx\n", value);
    //value = SR(value);
    value = MC(value);
    printf("MC -> %lx\n", value);
    return value;
}

__uint128_t shift_rotate_rigth(__uint128_t x, int rotation) {
    return (x >> rotation) | (x << (128 - rotation));
}

uint64_t key_scheduling(__uint128_t master_key, int round) {
    for (int i = 0; i < round; i++) {
        master_key = shift_rotate_rigth((master_key ^ 0xf33ff33f), 32);
    }
    return master_key & 0xFFFFFFFFFFFFFFFF;
}

uint64_t ARK(uint64_t round_input, __uint128_t master_key, int round) {
    uint64_t round_key = key_scheduling(master_key, round);
    printf("RK = %lx\n", round_key);
    return round_input ^ round_key;
}

uint64_t SB(uint64_t sBox_input) {
    uint64_t result = 0;
    uint64_t temp = sBox_input;
    for (int i = 0; i < 8; i++) {
        uint8_t value_before_sbox = temp & 0xFF;
        uint64_t value_after_sbox = (uint64_t) sbox[value_before_sbox];
        result += (value_after_sbox << i * 8);
        temp = temp >> 8;
    }
    return result;
}


void free_matrix(unsigned int** matrix) {
    for (int i = 0; i < 4; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

uint** matrix_conversion(uint64_t value) {
    uint** matrix = calloc(1, sizeof(uint*) * 4);
    for (int i = 0; i < 4; i++) {
        matrix[i] = calloc(1, sizeof(uint) * 4);
    }
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            matrix[3 - col][3 - row] = value & 0xF;
            value = value >> 4;
        }
    }
    return matrix;
}

uint** matrix_multiplication(uint** left_matrix, uint** right_matrix) {
    uint** result = malloc(sizeof(unsigned int*) * 4);
    for (int i = 0; i < 4; i++)
    {
        result[i] = malloc(sizeof(unsigned int) * 4);
    }
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int k = 0; k < 4; k++) {
                result[row][col] = result[row][col] ^ (left_matrix[row][k] * right_matrix[k][col]);
            }
        }
    }
    free_matrix(right_matrix);
    return result;
}

uint64_t matrix_to_uint64(uint** matrix) {
    uint64_t value = 0;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            value += matrix[row][col] << ((16 - (row + col*4 + 1)) * 4);
        }
    }
    free_matrix(matrix);
    return value;
}

uint64_t MC(uint64_t mixcol_input) {
    uint** matrix = matrix_conversion(mixcol_input);
    matrix = matrix_multiplication(fixed_matrix, matrix);
    return matrix_to_uint64(matrix);
}
