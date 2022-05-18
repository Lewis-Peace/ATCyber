/**
 * @file attack 1.c
 * @author your name (you@domain.com)
 * @brief Attack on cipher 04-05-2022 with a differential attack
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include "../Ciphers/04-05-2022.c"
#include "../Tools/differential_tools.h"

int find_max_number_in_matrix(int** m);

int main() {
    __uint128_t master_key = 0x101;
    int rounds = 1;
    uint64_t alpha = 0x0;
    uint64_t plain_text = 0x1;
    uint64_t cipher_text_1 = cipher(0x101, plain_text, rounds);
    uint64_t cipher_text_2 = cipher(0x101, plain_text, rounds);
    printf("%lx xor %lx = %lx\n", cipher_text_1, cipher_text_2, cipher_text_1 ^ cipher_text_2);
    //int** table = malloc(sizeof(int*) * 256);
    //for (int i = 0; i < 256; i++)
    //{
    //    table[i] = malloc(sizeof(int) * 256);
    //}
    //compute_table(sbox, table, 8);
    //fprint_int_matrix("matrix.txt", table, 256, 256);
    /*
    FILE *fh = fopen("valid_pairs.txt", "w");
    uint64_t start = 0x4000000000000000;
    for (uint64_t i = start; i < start + 0x8; i++)
    {
        uint64_t plain_text_1 = i;
        uint64_t cipher_text_1 = cipher(master_key, plain_text_1, rounds);
        uint64_t plain_text_2 = plain_text_1 ^ alpha;
        uint64_t cipher_text_2 = cipher(master_key, plain_text_2, rounds);
        uint64_t beta = cipher_text_1 ^ cipher_text_2;
        printf("%lx xor %lx = %lx\n", plain_text_1, plain_text_2, alpha);
        printf("%lx xor %lx = %lx\n", cipher_text_1, cipher_text_2, beta);
        if (beta == 0x0) {
            fprintf(fh, "%lx xor %lx = %lx\n", plain_text_1, plain_text_2, plain_text_1 ^ plain_text_2);
            fprintf(fh, "%lx xor %lx = %lx\n", cipher_text_1, cipher_text_2, beta);
        }
    }
    fclose(fh);*/
    

}

int find_max_number_in_matrix(int** m) {
    int max = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if (m[i][j] > max & (i != 0 & j != 0))
            {
                max = m[i][j];
            }
            
        }
        
    }
    return max;
    
}