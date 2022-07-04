#include "des.h"
#include "des_key.h"
#include "des_data.h"
#include "des_lookup.h"
#include "des_approximations.h"

#include <inttypes.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <limits.h>

//#pragma GCC push_options
#pragma GCC optimize ("unroll-loops")

DES::DES(ui64 key)
{
    keygen(key);
}

ui64 DES::encrypt(ui64 block)
{
    return des(block, false);
}

ui64 DES::decrypt(ui64 block)
{
    return des(block, true);
}

ui64 DES::encrypt(ui64 block, ui64 key)
{
    DES des(key);
    return des.des(block, false);
}

ui64 DES::decrypt(ui64 block, ui64 key)
{
    DES des(key);
    return des.des(block, true);
}

void DES::keygen(ui64 key)
{
    // initial key schedule calculation
    ui64 permuted_choice_1 = 0; // 56 bits
    for (ui8 i = 0; i < 56; i++)
    {
        permuted_choice_1 <<= 1;
        permuted_choice_1 |= (key >> (64-PC1[i])) & LB64_MASK;
    }

    // 28 bits
    ui32 C = (ui32) ((permuted_choice_1 >> 28) & 0x000000000fffffff);
    ui32 D = (ui32)  (permuted_choice_1 & 0x000000000fffffff);

    // Calculation of the 16 keys
    for (ui8 i = 0; i < ROUNDS; i++)
    {
        // key schedule, shifting Ci and Di
        for (ui8 j = 0; j < ITERATION_SHIFT[i]; j++)
        {
            C = (0x0fffffff & (C << 1)) | (0x00000001 & (C >> 27));
            D = (0x0fffffff & (D << 1)) | (0x00000001 & (D >> 27));
        }

        ui64 permuted_choice_2 = (((ui64) C) << 28) | (ui64) D;

        sub_key[i] = 0; // 48 bits (2*24)
        for (ui8 j = 0; j < 48; j++)
        {
            sub_key[i] <<= 1;
            sub_key[i] |= (permuted_choice_2 >> (56-PC2[j])) & LB64_MASK;
        }
    }
}

ui64 DES::des(ui64 block, bool mode)
{
    // applying initial permutation
    //block = ip(block);

    // dividing T' into two 32-bit parts
    ui32 L = (ui32) (block >> 32) & L64_MASK;
    ui32 R = (ui32) (block & L64_MASK);

    // 16 rounds
    for (ui8 i = 0; i < ROUNDS; i++)
    {
        ui32 F = mode ? f(R, sub_key[ROUNDS-i]) : f(R, sub_key[i]);
        feistel(L, R, F);
    }

    // swapping the two parts
    block = (((ui64) R) << 32) | (ui64) L;
    // applying final permutation
    //return fp(block);
    return block;
}

ui64 DES::ip(ui64 block)
{
    // initial permutation
    ui64 result = 0;
    for (ui8 i = 0; i < 64; i++)
    {
        result <<= 1;
        result |= (block >> (64-IP[i])) & LB64_MASK;
    }
    return result;
}

ui64 DES::fp(ui64 block)
{
    // inverse initial permutation
    ui64 result = 0;
    for (ui8 i = 0; i < 64; i++)
    {
        result <<= 1;
        result |= (block >> (64-FP[i])) & LB64_MASK;
    }
    return result;
}

void DES::feistel(ui32 &L, ui32 &R, ui32 F)
{
    ui32 temp = R;
    R = L ^ F;
    L = temp;
}

ui32 DES::f(ui32 R, ui64 k) // f(R,k) function
{
    // applying expansion permutation and returning 48-bit data
    ui64 s_input = 0;
    for (ui8 i = 0; i < 48; i++)
    {
        s_input <<= 1;
        s_input |= (ui64) ((R >> (32-EXPANSION[i])) & LB32_MASK);
    }

    // XORing expanded Ri with Ki, the round key
    s_input = s_input ^ k;

    // applying S-Boxes function and returning 32-bit data
    ui32 s_output = 0;
    for (ui8 i = 0; i < 8; i++)
    {
        // Outer bits
        char row = (char) ((s_input & (0x0000840000000000 >> 6*i)) >> (42-6*i));
        row = (row >> 4) | (row & 0x01);

        // Middle 4 bits of input
        char column = (char) ((s_input & (0x0000780000000000 >> 6*i)) >> (43-6*i));

        s_output <<= 4;
        if (ONE_SBOX == 0) {
            s_output |= (ui32) (SBOX[4][16*row + column] & 0x0f);
        } else {
            s_output |= (ui32) (SBOX[i][16*row + column] & 0x0f);
        }
    }

    // applying the round permutation
    ui32 f_result = 0;
    for (ui8 i = 0; i < 32; i++)
    {
        f_result <<= 1;
        f_result |= (s_output >> (32 - PBOX[i])) & LB32_MASK;
    }

    return f_result;
}

//#pragma GCC pop_options

#define MASTER_KEY 0x0

class tools {
private:

public:
    tools(/* args */);
    ~tools();

    int static mask_and_xor(uint64_t value, uint64_t mask) {
        value = value & mask;
        uint64_t result = 0;
        for (int i = 0; i < 64; i++) {
            result ^= value & 0b1;
            value = value >> 1;    
        }
        return result;
    }

    static uint64_t random_plain() {
        uint64_t value = 0;
        for (int i = 0; i < 64; i++) {
            value += (rand() % 2);
            value = value << 1;
        }
        return value;
    }

    void static compute_table(int sBox_index) {
        const uint8_t DesSbox[8][4][16] = {
        {
            {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
            { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
            { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
            {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
        },
        
        {
            {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
            { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
            { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
            {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
        },
        
        {
            {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
            {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
            {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
            { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
        },
        
        {
            { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
            {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
            {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
            { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
        },
        
        {
            { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
            {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
            { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
            {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
        },
        
        {
            {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
            {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
            { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
            { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
        },
        
        {
            { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
            {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
            { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
            { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
        },
        
        {
            {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
            { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
            { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
            { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
        },
        };
        int matrix[16][64];
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 64; j++) {
                matrix[i][j] = -32;
            }
        }
        for (uint8_t i = 0; i < 16; i++) {
            for (uint8_t j = 0; j < 64; j++) {
                for (uint8_t input = 0; input < 64; input++) {
                    uint8_t sBox_value = DesSbox[sBox_index][((input & 0b100000) >> 4) | input & 0b1][(input & 0b11110) >> 1];
                    if (mask_and_xor(input, j) == mask_and_xor(sBox_value, i)) {
                        matrix[i][j]++;
                    }
                }
            }
        }
        char* file_name = (char*) calloc(10, sizeof(char));
        sprintf(file_name, "table%i.txt", sBox_index);
        FILE* fh = fopen(file_name, "w");
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 16; j++) {
                fprintf(fh, "%4d ", matrix[j][i]);
            }
            fprintf(fh, "\n");
        }
        

    }

};

void print_keys() {
    DES cipher = DES(MASTER_KEY);

    for (int i = 0; i < ROUNDS; i++) {
        printf("K%i = %lx\n", i, cipher.sub_key[i]);
    }
    
}

void left_rotation(uint32_t* data, int rotate) {
    printf("%X\n", *data);
    *data = ((*data >> (28 - rotate)) | (*data << rotate)) & 0xFFFFFFF;
    printf("%X\n", *data);
}

void inverse_key_schedule(uint64_t round_key, int round) {
    const int inverse_PC2[56] = {
        4,23,6,15,5,9,19,17,
        -1,11,2,14,22,0,8,18,
        1,-1,13,21,10,-1,12,3,
        -1,16,20,7,41,30,26,47,
        34,40,-1,45,27,-1,38,31,
        24,43,-1,36,33,42,28,35,
        37,44,32,25,41,-1,29,39
    };
    uint64_t inverted_round = 0;
    uint64_t temp = round_key;
    for (int i = 0; i < 56; i++) {
        if (inverse_PC2[i] != -1) { // converning right part
            inverted_round |= (temp & (uint64_t) 0b1) << inverse_PC2[i];
        }
        temp = temp >> 1;
    }
    // Used for inversion as testing
    //uint64_t testing = 0;
    //temp = inverted_round;
    //for (int i = 0; i < 56; i++) {
    //    if (PC2[i] != -1) { // converning right part
    //        testing |= (temp & (uint64_t) 0b1) << PC2[i] - 1;
    //    }
    //    temp = temp >> 1;
    //}
    //printf("l = %016lx\n", testing);

    uint32_t left_part = (inverted_round & 0xFFFFFFF0000000) >> 28;
    uint32_t right_part = inverted_round & 0xFFFFFFF;

    printf("%lx\n", inverted_round);
    printf("L = %x, R = %x\n", left_part, right_part);
}

void cipher(uint64_t* data) {
    uint64_t key = MASTER_KEY;
    uint64_t cipher_text = DES::encrypt(*data, key);
    *data = cipher_text;
}

void attack(uint64_t plain_mask, uint64_t cipher_mask, float p) {
    int plaintext_number = 2 * pow(abs(p - 0.5), -2); // Calculating amount of plaintexts to achieve a ...
                                                      // ... 99.8% success rate
    printf("Using masks %016lX and %016lX with %i plaintext ciphertext pairs\n", plain_mask, cipher_mask, plaintext_number);
    int N = plaintext_number, T = 0;
    uint64_t plain[N], data[N];
    for (int i = 0; i < N; i++) {
        plain[i] = tools::random_plain();
        data[i] = plain[i];
        cipher(&data[i]);
        int plain_masked = tools::mask_and_xor(plain[i], plain_mask);
        int cipher_masked = tools::mask_and_xor(data[i], cipher_mask);
        if (plain_masked == cipher_masked) {
            T++;
        }
    }

    if (T > N/2) {
        if (p > 0.5) {
            printf("K[1,2,..., c] = 0\n");
        } else {
            printf("K[1,2,..., c] = 1\n");
        }
    } else {
        if (p > 0.5) {
            printf("K[1,2,..., c] = 1\n");
        } else {
            printf("K[1,2,..., c] = 0\n");
        }
    }
}

uint64_t obtain_mask(vector<int> indexes_list) {
    uint64_t mask = 0;
    for (int i = 0; i < indexes_list.size(); i++) {
        mask |= (uint64_t) 1 << indexes_list[i];
    }
    return mask;
}

vector<float> probabilities = {
    //0.519
    0.7,
    0.7,
    0.7
    };

// Main
int main(int argc, char ** argv) {
    printf("Running DES of %i rounds and one SBox\n", ROUNDS);

    srand(time(0));
    
    //print_keys();

    //for (int i = 0; i < plain_masks.size(); i++) {
    //    float p = probabilities[i];
    //    uint64_t plain_mask = obtain_mask(plain_masks[i]);
    //    uint64_t cipher_mask = obtain_mask(cipher_masks[i]);
    //    attack(plain_mask, cipher_mask, 0.7);
    //}
    
    //for (int i = 0; i < 8; i++) {
    //    tools::compute_table(i);
    //}

    inverse_key_schedule(0x545810C8145672, 1);

    uint32_t data = 0xF000000;
    left_rotation(&data, 4);
    return 0;
}