/**
 * @file tools.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <inttypes.h>
#include <iostream>
#include <iomanip>
#include "src/DES.c"
#include <math.h>
#include <vector>
#include <limits.h>

#define MASTER_KEY 0x0
#define ATTACK_REPEAT 10
using std::vector;

class tools {
private:

public:
    tools(/* args */);
    ~tools();

    static int max_element(int array[ATTACK_REPEAT]) {
        int result = INT_MIN;
        for (size_t i = 0; i < ATTACK_REPEAT; i++) {
            if (array[i] > result) {
                result = array[i];
            }
        }
        return result;
    }

    static int min_element(int array[ATTACK_REPEAT]) {
        int result = INT_MAX;
        for (size_t i = 0; i < ATTACK_REPEAT; i++) {
            if (array[i] < result) {
                result = array[i];
            }
        }
        return result;
    }

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
    uint64_t r_key[ROUNDS];
    r_key[0] = MASTER_KEY;
    uint64_t next_key;

    for(int ii = 0; ii < ROUNDS; ii++)
    {
        key_schedule(&r_key[ii], &next_key, ii);
        if(ii != ROUNDS - 1)
            r_key[ii + 1] = next_key;
    }

    uint8_t uno = (r_key[0] >> 22) & 0b1;
    uint8_t due = (r_key[2] >> 22) & 0b1;
    if (uno ^ due == 1) {
        printf("Values %i %i\n", uno, due);
    } else {
        printf("Values %i %i\n", uno, due);
    }

    for (int i = 0; i < ROUNDS; i++) {
        printf("K%i = %lx\n", i, r_key[i]);
    }
    
}

void revert_key() {
    uint64_t key = 0x400000;
    uint64_t new_key = 0;
    for (int i = 0; i < ROUNDS; i++) {
        printf("K%i = %lX\n", ROUNDS - i, key);
        addbit(&new_key, key, i, PC2[i] - 1);
    }
    
}

void cipher(uint64_t* data, bool encrypt_mode) {
    uint64_t key = MASTER_KEY;
    //
    // 1. Verify parity bits of the key
    //
    
    if(!key_parity_verify(key))
    {
        printf("The key you used is malformated\n"); // More error msg in function
        exit(EXIT_FAILURE);
    }

    //
    // 2. Get the ROUNDS subkeys
    //

    uint64_t r_key[ROUNDS];
    r_key[0] = key;
    uint64_t next_key;

    for(int ii = 0; ii < ROUNDS; ii++)
    {
        key_schedule(&r_key[ii], &next_key, ii);
        if(ii != ROUNDS - 1)
            r_key[ii + 1] = next_key;
    }

    if (encrypt_mode) {
        encrypt(data, r_key);
    } else {
        decrypt(data, r_key);
    }

}

void attack(uint64_t plain_mask, uint64_t cipher_mask, float p) {
    int plaintext_number = 2 * pow(abs(p - 0.5), -2); // Calculating amount of plaintexts to achieve a ...
                                                      // ... 99.8% success rate
    printf("Using masks %lX and %lX\n", plain_mask, cipher_mask);
    int N = plaintext_number, T = 0, N_half = N/2;
    int Ti[ATTACK_REPEAT];
    uint64_t plain[N], data[N];
    for (int j = 0; j < ATTACK_REPEAT; j++) {
        for (int i = 0; i < N; i++) {
            plain[i] = tools::random_plain();
            data[i] = plain[i];
            cipher(&data[i], true);
            int plain_masked = tools::mask_and_xor(plain[i], plain_mask);
            int cipher_masked = tools::mask_and_xor(data[i], cipher_mask);
            if (plain_masked == cipher_masked) {
                T++;
            }
        }
        Ti[j] = T;
        T = 0;
    }
    
    int max_T = tools::max_element(Ti);
    int min_T = tools::min_element(Ti);
    for (int i = 0; i < ATTACK_REPEAT; i++) {
        printf("%i ", Ti[i]);
    }
    printf("\n");
    
    printf("max_T = %i, min_T = %i\n", max_T, min_T);
    if (abs(max_T - N_half) > abs(min_T - N_half)) {
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

// Main
int main(int argc, char ** argv) {
    printf("Running DES of %i rounds\n", ROUNDS);
    printf("Using version with ");
    ONE_SBOX == 0 ? printf("one sBox\n") : printf("all the sBox\n");

    srand(time(0));
    
    //print_keys();

    vector<int> plain = {32+15, 7,18,24,27,28,29,30,31};
    vector<int> cipher = {32+15, 7,18,24,27,28,29,30,31};

    uint64_t plain_mask = obtain_mask(plain);
    uint64_t cipher_mask = obtain_mask(cipher);
    attack(plain_mask, cipher_mask, 0.519);


    vector<int> m = {31,30,28,27};
    printf("%lX\n", obtain_mask(m));
    //for (int i = 0; i < 8; i++) {
    //    tools::compute_table(i);
    //}

    // 11011000000000000000000000000000
    // 011011 110000 000000 000000 000000 000000 000000 000001

    return 0;
}