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
#include "des.cpp"


#define MASTER_KEY 0x0

class tools {
private:

public:
    tools(/* args */);
    ~tools();

    static void right_rotation(uint64_t* data, int rotate) {
        *data = ((*data << (28 - rotate)) | (*data >> rotate)) & 0xFFFFFFF;
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

uint64_t inverse_key_schedule(uint64_t round_key, int round) {
    const int inverse_PC2[56] = {
        4, 23, 6, 15, 5, 9, 19, 17, 
        -1, 11, 2, 14, 22, 0, 8, 18, 
        1, -1, 13, 21, 10, -1, 12, 3, 
        -1, 16, 20, 7, 46, 30, 26, 47, 
        34, 40, -1, 45, 27, -1, 38, 31, 
        24, 43, -1, 36, 33, 42, 28, 35, 
        37, 44, 32, 25, 41, -1, 29, 39
    };
    const int inverse_PC1[64] = {
        7, 15, 23, 55, 51, 43, 35, -1, 
        6, 14, 22, 54, 50, 42, 34, -1, 
        5, 13, 21, 53, 49, 41, 33, -1, 
        4, 12, 20, 52, 48, 40, 32, -1, 
        3, 11, 19, 27, 47, 39, 31, -1, 
        2, 10, 18, 26, 46, 38, 30, -1, 
        1, 9, 17, 25, 45, 37, 29, -1, 
        0, 8, 16, 24, 44, 36, 28, -1
    };
    
    uint64_t inverted_round = 0;
    for (int i = 0; i < 56; i++) {
        if (inverse_PC2[i] != -1) {
            inverted_round |= ((round_key >> i) & LB64_MASK) << inverse_PC2[i];
        }
    }

    uint64_t left_part = (inverted_round >> 28) & 0xFFFFFFF;
    uint64_t right_part = inverted_round & 0xFFFFFFF;

    for (int i = round; i > -1; i--) {
        tools::right_rotation(&left_part, ITERATION_SHIFT[i]);
        tools::right_rotation(&right_part, ITERATION_SHIFT[i]);
        printf("Rotating by %i at round %i\n", ITERATION_SHIFT[i], i);
    }
    
    return (left_part << 28) | right_part;
}

void print_keys() {
    DES cipher = DES((uint64_t) MASTER_KEY);
    ui64 permuted_choice_1 = 0; // 56 bits
    for (ui8 i = 0; i < 56; i++)
    {
        permuted_choice_1 <<= 1;
        permuted_choice_1 |= (MASTER_KEY >> (64-PC1[i])) & LB64_MASK;
    }
    printf("DES key = %lx\n", permuted_choice_1);
    for (int i = 0; i < ROUNDS; i++) {
        printf("K%i = %lx\n", i, cipher.sub_key[i]);
    }

    int round = 2;
    uint64_t reverted_key = inverse_key_schedule(cipher.sub_key[round], round);
    printf("Inverted = %lx\n", reverted_key);
    if (permuted_choice_1 == reverted_key) {
        printf("Inversion correct\n");
    }
}

void cipher(uint64_t* data) {
    uint64_t key = MASTER_KEY;
    uint64_t cipher_text = DES::encrypt(*data, key);
    *data = cipher_text;
}

void algorithm1(uint64_t plain_mask, uint64_t cipher_mask, float p, vector<int> keybits_that_are_influenced) {
    int plaintext_number = 2 * pow(abs(p - 0.5), -2); // Calculating amount of plaintexts to achieve a ...
                                                      // ... 99.8% success rate
    // printf("Using masks %016lX and %016lX with %i plaintext ciphertext pairs\n", plain_mask, cipher_mask, plaintext_number);
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
            keybits_that_are_influenced.push_back(0);
            key_approximations.push_back(keybits_that_are_influenced);
            // printf("K[1,2,..., c] = 0\n");
        } else {
            keybits_that_are_influenced.push_back(1);
            key_approximations.push_back(keybits_that_are_influenced);
            // printf("K[1,2,..., c] = 1\n");
        }
    } else {
        if (p > 0.5) {
            keybits_that_are_influenced.push_back(1);
            key_approximations.push_back(keybits_that_are_influenced);
            // printf("K[1,2,..., c] = 1\n");
        } else {
            keybits_that_are_influenced.push_back(0);
            key_approximations.push_back(keybits_that_are_influenced);
            // printf("K[1,2,..., c] = 0\n");
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

void compute_tables() {
    for (int i = 0; i < 8; i++) {
        tools::compute_table(i);
    }
}

void attack() {
    if (plain_masks.size() != cipher_masks.size()) {
        printf("Error\n");
        exit(1);
    }
    int approximation_number = plain_masks.size();
    for (int i = 0; i < approximation_number; i++) {
        float p = probabilities[i];
        uint64_t plain_mask = obtain_mask(plain_masks[i]);
        uint64_t cipher_mask = obtain_mask(cipher_masks[i]);
        algorithm1(plain_mask, cipher_mask, probabilities[i], key_bits_influenced[i]);
    }
    
    int size = 0;
    for (int j = 0; j < key_approximations.size(); j++) {
        vector<int> bits = key_approximations[j];
        size = bits.size();
        for (int i = 0; i < size; i++) {
            if (size - 1 == i) {
                printf("= %i", bits[i]);
            } else {
                printf("%i ", bits[i]);
            }
        }
        printf("\n");
    }
}

void key_guessing() {
    KeyGeneration k = KeyGeneration();
    for (vector<int> bits : key_approximations) {
        k.add_approximation(bits);
    }
    
    uint64_t p = 0x9876543210;
    uint64_t c = 0x9876543210;
    cipher(&c);

    printf("Guessing with plain %lx, and cipher %lx\n", p, c);
    //k.print_all_keys();

    k.key_guessing(p, c);
}

// Main
int main(int argc, char ** argv) {
    printf("Running DES of %i rounds and one SBox\n", ROUNDS);

    srand(time(0));
    
    attack();
    //print_keys();

    key_guessing();
    
    return 0;
}