#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "operations.h"

#define uint unsigned int

int mask_operation(uint , uint , int );
int b_calc(uint , uint , uint , uint , int );
uint SBox(uint);
uint** istantiate_matrix(int, int);
uint** sbox_linear_approximation();
uint* satisfy_table(uint, uint, uint);
uint permutation(uint);

int main() {
    //uint alpha = 0b101;
    //uint beta = 0b11;
    //uint** matrix = sbox_linear_approximation(alpha, beta);
    //print_matrix(matrix, 16,16);
    //printf("6 -> 8 %i\n", b_calc(6, alpha, 8, beta, 4));
    //printf("0 -> c %i\n", b_calc(0, alpha, 0xc, beta, 4));
    //printf("d -> b %i\n", b_calc(0xd, alpha, 0xb, beta, 4));
    //satisfy_table(0b101, 0b1101, 0b11);
    printf("%i\n", permutation(0b10000));
}

uint** istantiate_matrix(int row, int cols) {
    uint** matrix = malloc(sizeof(uint*) * row);
    for (int i = 0; i < row; i++) {
        matrix[i] = malloc(sizeof(uint) * cols);
    }
    return matrix;
}

uint** sbox_linear_approximation() {
    uint** matrix = istantiate_matrix(16, 16);
    for (int alpha = 0; alpha < 16; alpha++) {
        for (int beta = 0; beta < 16; beta++) {
            for (int i = 0; i < 16; i++) {
                int y = SBox(i);
                if (b_calc(i, alpha, y, beta, 4) == 0) {
                    matrix[alpha][beta] += 1;
                }
            }
        }
    }
    return matrix;
}

uint* append(uint* array, int* len, uint value) {
    int l = *len;
    *len = *len + 1;
    uint* new_array = malloc(sizeof(uint) * (l + 1));
    for (int i = 0; i < l; i++) {
        new_array[i] = array[i];
    }
    new_array[l] = value;
    free(array);
    return new_array;
}

uint* satisfy_table(uint alpha, uint output, uint beta) {
    uint* possible_inputs;
    int len = 0;
    for (int possible_input = 0; possible_input < 16; possible_input++) {
        if (b_calc(possible_input, alpha, output, beta, 4) == 0) {
            possible_inputs = append(possible_inputs, &len, possible_input);
            printf("%i\n", possible_input);
        }
    }
    return possible_inputs;
}

int b_calc(uint input, uint alpha_mask, uint output, uint beta_mask, int bits) {
    return mask_operation(input, alpha_mask, bits) ^ mask_operation(output, beta_mask, bits);
}

int mask_operation(uint a, uint b, int bits) {
    int result = 0;
    for (uint i = 0; i < bits; i++) {
        uint ai = (a >> i) & 0x1;
        uint bi = (b >> i) & 0x1;
        result = result ^ (ai* bi);
    }
    return result;
}

uint SBox(uint a) {
    uint list[16] = {6,4,0xc,5,0,7,2,0xe,1,0xf,3,0xd,8,0xa,9,0xb};
    return list[a];
}

uint permutation(uint a) {
    uint result = 0;
    uint list[16] = {6,4,0xc,5,0,7,2,0xe,1,0xf,3,0xd,8,0xa,9,0xb};
    for (int i = 0; i < 16; i++) {
        uint value = (a >> i) & 0b1;
        result += value << list[i];
    }
    return result;
}