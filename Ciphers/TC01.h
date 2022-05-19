/**
 * @file TC01.h
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

#include <stdint.h>

                //   0,  1, 2, 3, 4, 5,  6,  7,  8,   9,  a, b, c,  d,   e,  f
uint8_t sbox[] = {0xa, 0x5, 0x4, 0x2, 0x6, 0x1, 0xf, 0x3, 0xb, 0xe, 0x7, 0, 0x8, 0xd, 0xc, 0x9};
                        //   0   1  2  3  4  5  6   7    8    9   A  B   C    D   E  F
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

uint8_t* cipher(uint8_t* plain_text, const uint8_t* master_key);
void cipher_round(uint8_t* round_input, uint8_t* master_key);
void ARK(uint8_t* input_bits, uint8_t* round_key);
void SC(uint8_t* input_bits) ;
void key_scheduling(uint8_t* key, uint8_t* round_key);
uint64_t convert_into_64bits(uint8_t** matrix) ;
void MC(uint8_t* input_value);
uint8_t** convert_into_matrix(uint64_t input_bits, uint8_t** input_matrix) ;
void SR(uint8_t* input_bits);


/**
 * Decryption part
 * 
 */

uint8_t* decrypt(uint8_t* cipher_text, const uint8_t* master_key);
void decrypt_round(uint8_t* cipher_text, uint8_t* master_key);
void reverse_ARK(uint8_t* input_bits, uint8_t* round_key);
void reverse_SC(uint8_t* input_bits);
void reverse_SR(uint8_t* input_bits);
void reverse_MC(uint8_t* input_value);
