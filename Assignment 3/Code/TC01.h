/**
 * @brief 
 * 
 *
 * @author your name (you@domain.com)
 * @brief The cipher explained in the second assignment. The round is structured with 4 steps:
 * ARK that adds the key
 * SC this function applies sBox
 * SR is the shiftrow function
 * MC is the mixcolumn function with matrix:
 *      1 0 1 0
 *      0 1 1 0
 *      1 0 0 0
 *      0 0 1 1
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "./operations.h"
#include <stdint.h>

uint8_t sbox[16] = {0xa, 5, 4, 2, 6, 1, 0xf, 3, 0xb, 0xe, 7, 0, 8, 0xd, 0xc, 9};
uint8_t reverse_sbox[16] = {0xb, 5, 3, 7, 2, 1, 4, 0xa, 0xc, 0xf, 0, 8, 0xe, 0xd, 9, 6};

uint8_t fixed_matrix[4][4] = {
    {1,0,1,0},
    {0,1,1,0},
    {1,0,0,0},
    {0,0,1,1}
};

uint8_t inverse_matrix[4][4] = {
    {0,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,1}
};

#define MASTER_KEY 0x101

uint64_t cipher(uint64_t plain_text);
uint64_t cipher_round(uint64_t round_input, uint64_t master_key, int round_number);
uint64_t ARK(uint64_t input_bits, uint64_t round_key);
uint64_t SC(uint64_t input_bits);
uint8_t** SR(uint64_t input_bits, uint8_t** matrix);
uint64_t MC(uint8_t** input_matrix);
uint64_t key_scheduling(uint64_t master_key, int round);
uint64_t decrypt(uint64_t cipher_text);
uint64_t decrypt_round(uint64_t cipher_text, uint64_t master_key, int round);
uint64_t reverse_ARK(uint64_t input_bits, uint64_t round_key);
uint64_t reverse_SC(uint64_t input_bits);
uint64_t reverse_SR(uint8_t** matrix);
uint8_t** reverse_MC(uint64_t input_bits, uint8_t** input_matrix);


uint64_t cipher(uint64_t plain_text) {
    uint64_t data = plain_text;
    for (int i = 0; i < 4; i++) {
        data = cipher_round(data, MASTER_KEY, i);
    }
    return data;
}


uint64_t cipher_round(uint64_t round_input, uint64_t master_key, int round_number) {
    uint64_t round_key = key_scheduling(master_key, round_number);
    uint64_t temp = ARK(round_input, round_key);
    temp = SC(temp);
    uint8_t** matrix = SR(temp, matrix);
    temp = MC(matrix);
    for (int i = 0; i < 4; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    return temp;
}

uint64_t ARK(uint64_t input_bits, uint64_t round_key) {
    return input_bits ^ round_key;
}

uint64_t SC(uint64_t input_bits) {
    uint64_t result = 0;
    uint64_t temp = input_bits;
    for (int i = 0; i < 16; i++) {
        uint8_t value_before_sbox = temp & 0xF;
        uint64_t value_after_sbox = (uint64_t) sbox[value_before_sbox];
        result += (value_after_sbox << i * 4);
        temp = temp >> 4;
    }
    return result;
}

uint64_t key_scheduling(uint64_t master_key, int round) {
    uint64_t round_key = master_key;
    for (int i = 0; i < round; i++) {
        round_key = shift_rotate64_right(round_key ^ 0xF33F, 16);
    }
    return round_key & 0x00000000FFFFFFFF;
}

uint64_t convert_into_64bits(uint8_t** matrix) {
    uint64_t bit_string = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            bit_string |= matrix[j][i] & 0xF;
            if (!(i == 3 && j == 3)) {
                bit_string = bit_string << 4;
            }
        }
    }
    
    return bit_string;
}

uint64_t MC(uint8_t** input_matrix) {
    uint8_t** result = instantiate_matrix(result, 4, 4);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result[i][j] = result[i][j] ^ (fixed_matrix[i][k] * input_matrix[k][j]);
            }
        }
    }
    return convert_into_64bits(result);
}

uint8_t** convert_into_matrix(uint64_t input_bits, uint8_t** input_matrix) {
    input_matrix = instantiate_matrix(input_matrix, 4, 4);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            uint8_t cell_value = input_bits & 0x0F;
            input_matrix[3 - j][3 - i] = cell_value;
            input_bits = input_bits >> 4;
        }
    }
    return input_matrix;
}

uint8_t** SR(uint64_t input_bits, uint8_t** input_matrix) {
    input_matrix = convert_into_matrix(input_bits, input_matrix);

    uint8_t temp = input_matrix[1][0];
    input_matrix[1][0] = input_matrix[1][1];
    input_matrix[1][1] = input_matrix[1][2];
    input_matrix[1][2] = input_matrix[1][3];
    input_matrix[1][3] = temp;
    temp = input_matrix[2][0];
    uint8_t temp2 = input_matrix[2][1];
    input_matrix[2][0] = input_matrix[2][2];
    input_matrix[2][1] = input_matrix[2][3];
    input_matrix[2][2] = temp;
    input_matrix[2][3] = temp2;
    temp = input_matrix[3][0];
    temp2 = input_matrix[3][1];
    uint8_t temp3 = input_matrix[3][2];
    input_matrix[3][0] = input_matrix[3][3];
    input_matrix[3][1] = temp;
    input_matrix[3][2] = temp2;
    input_matrix[3][3] = temp3;

    return input_matrix;
}

/**
 * Decryption part
 * 
 */


uint64_t decrypt(uint64_t cipher_text) {
    uint64_t data = cipher_text;
    for (int i = 3; i >= 0; i--) {
        data = decrypt_round(data, MASTER_KEY, i);
    }
    
}

uint64_t decrypt_round(uint64_t cipher_text, uint64_t master_key, int round) {
    uint64_t round_key = key_scheduling(master_key, round);
    uint8_t** matrix = reverse_MC(cipher_text, matrix);
    uint64_t round_data = reverse_SR(matrix);
    round_data = reverse_SC(round_data);
    round_data = reverse_ARK(round_data, round_key);
    return round_data;
}

uint64_t reverse_ARK(uint64_t input_bits, uint64_t round_key) {
    return input_bits ^ round_key;
}

uint64_t reverse_SC(uint64_t input_bits) {
    uint64_t result = 0;
    uint64_t temp = input_bits;
    for (int i = 0; i < 16; i++) {
        uint8_t value_before_sbox = temp & 0xF;
        uint64_t value_after_sbox = (uint64_t) reverse_sbox[value_before_sbox];
        result += (value_after_sbox << i * 4);
        temp = temp >> 4;
    }
    return result;
}

uint64_t reverse_SR(uint8_t** input_matrix) {
    
    uint8_t temp = input_matrix[1][3];
    input_matrix[1][3] = input_matrix[1][2];
    input_matrix[1][2] = input_matrix[1][1];
    input_matrix[1][1] = input_matrix[1][0];
    input_matrix[1][0] = temp;
    temp = input_matrix[2][3];
    uint8_t temp2 = input_matrix[2][2];
    input_matrix[2][3] = input_matrix[2][1];
    input_matrix[2][2] = input_matrix[2][0];
    input_matrix[2][1] = temp;
    input_matrix[2][0] = temp2;
    temp = input_matrix[3][3];
    temp2 = input_matrix[3][2];
    uint8_t temp3 = input_matrix[3][1];
    input_matrix[3][3] = input_matrix[3][0];
    input_matrix[3][2] = temp;
    input_matrix[3][1] = temp2;
    input_matrix[3][0] = temp3;

    return convert_into_64bits(input_matrix);
}

uint8_t** reverse_MC(uint64_t input_bits, uint8_t** input_matrix) {
    input_matrix = convert_into_matrix(input_bits, input_matrix);
    uint8_t** result = instantiate_matrix(result, 4, 4);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result[i][j] = result[i][j] ^ (inverse_matrix[i][k] * input_matrix[k][j]);
            }
        }
    }
    return result;
}
