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

/**
 * Cipher part
 * 
*/

uint64_t cipher(uint64_t plain_text, uint64_t master_key);
uint64_t cipher_round(uint64_t round_input, uint64_t master_key, int round_number);

uint64_t ARK(uint64_t input_bits, uint64_t round_key);
uint64_t SC(uint64_t input_bits);
uint8_t** SR(uint64_t input_bits, uint8_t** matrix);
uint64_t MC(uint8_t** input_matrix);

uint64_t convert_into_64bits(uint8_t** matrix);

uint64_t key_scheduling(uint64_t master_key, int round);

/**
 * Decryption part
 * 
*/

uint64_t decrypt(uint64_t cipher_text, uint64_t master_key);
uint64_t decrypt_round(uint64_t cipher_text, uint64_t master_key, int round);

uint64_t reverse_ARK(uint64_t input_bits, uint64_t round_key);
uint64_t reverse_SC(uint64_t input_bits);
uint64_t reverse_SR(uint8_t** matrix);
uint8_t** reverse_MC(uint64_t input_bits, uint8_t** input_matrix);
