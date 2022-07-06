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
#include <list>

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
        if (ONE_SBOX) {
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


#define UNDEFINED -1

using std::list;
using std::vector;

class KeyGeneration
{
private:
    list<int*> keys;

    uint64_t to_master_key(uint64_t pc1_bit_key) {
        uint8_t p[8];
        uint64_t master_key = 0;
        for (int i = 0; i < 8; i++) {
            p[i] = (pc1_bit_key >> (7 * i)) & 0x7F;
        }

        for (int i = 0; i < 8; i++) {
            master_key |= ((p[i] << 1) | mask(p[i])) << (i*8);
        }
        return master_key;
    }

    uint8_t mask(uint8_t value) {
        uint8_t result = 0;
        for (int i = 0; i < 8; i++) {
            result ^= (value >> i) & 0b1;
        }
        return result;
    }

    void copy_list(int* new_value, int* old_value) {
        for (int i = 0; i < 56; i++) {
            new_value[i] = old_value[i];
        }
    }

public:
    KeyGeneration() {
        int* key = (int*) calloc(56, sizeof(int));
        for (int i = 0; i < 56; i++) {
            key[i] = UNDEFINED;
        }
        keys.push_back(key);
    }

    void print_all_keys() {
        FILE* fh = fopen("Key_list.txt", "w");
        for (int* key: keys) {
            for (int j = 0; j < 56; j++) {
                fprintf(fh, "%i ", key[j]);
            }
            fprintf(fh, "\n");
        }
    }

    void add_approximation(vector<int> approximation) {
        int result_of_approximation = approximation.back();
        approximation.pop_back();
        int possible_keys_number = pow(2, approximation.size());
        // For each key update the values with new guessing
        int size = keys.size();
        for (int i = 0; i < size; i++) {
            int* key = keys.back();
            keys.pop_back();
            // Compute possibilities to find correct guessing according to result
            for (uint8_t j = 0; j < possible_keys_number; j++) {
                if (mask(j) == result_of_approximation) {
                    int* possible_k = (int*) calloc(56, sizeof(int));
                    copy_list(possible_k, key);
                    // Update values of key guess
                    bool error = false;
                    for (uint8_t l = 0; l < approximation.size(); l++) {
                        int value_position = 55 - approximation[l];
                        if (possible_k[value_position] == -1) {
                            possible_k[value_position] = (j >> l) & 0b1;
                        } else if ((j >> l) & 0b1 ^ possible_k[value_position] != result_of_approximation) {
                            // Used to solve overlapping on approximations
                            error = true;
                        }
                    }
                    if (!error) {
                        keys.push_front(possible_k);
                    }
                }
            }
            free(key);
        }
    }

    void key_guessing_recursive(uint64_t plain_text, uint64_t cipher_text, uint64_t key_guess, int* key, int i) {
        if (i == 55) {
            //printf("Trying with %lx\n", key_guess);
            uint64_t p = DES::encrypt(plain_text, to_master_key(key_guess));
            if (p == cipher_text) {
                printf("\nThe master key is %lX\n", to_master_key(key_guess));
                exit(0);
            }
        } else if (i < 55) {
            int value = key[55 - i];
            if (value == 0) { // Nothing to do already 0 as number
                key_guessing_recursive(plain_text, cipher_text, key_guess, key, i + 1);
            } else if (value == 1) { // Update bit to 1
                key_guess |= 1 << i;
                key_guessing_recursive(plain_text, cipher_text, key_guess, key, i + 1);
            } else if (value == -1) { // Split using 0 and then 1
                uint64_t k_temp = key_guess; // Should not but maybe in the recursion the key_guess var is modified also here
                key_guessing_recursive(plain_text, cipher_text, key_guess, key, i + 1);
                key_guess = k_temp;
                key_guess |= 1 << i;
                key_guessing_recursive(plain_text, cipher_text, key_guess, key, i + 1);
            } else { // Should not go there
                exit(1);
            }
        }
    }

    void key_guessing(uint64_t plain_text, uint64_t cipher_text) {
        uint64_t key_guess = 0;
        printf("Start..");
        for (int* key: keys) {
            key_guess = 0;
            key_guessing_recursive(plain_text, cipher_text, key_guess, key, 0);
            printf(".");
        }
    }
};
