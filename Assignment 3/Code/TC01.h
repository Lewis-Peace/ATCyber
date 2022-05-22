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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MASTER_KEY {7,6,9,3, 1,0xF,0xA,0xC, 9,0xD,0xA,0xB, 2,0xB,3,6}

              //   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   a,  b,  c,   d,   e,   f
uint8_t sbox[16] = {0xa, 0x5, 0x4, 0x2, 0x6, 0x1, 0xf, 0x3, 0xb, 0xe, 0x7, 0, 0x8, 0xd, 0xc, 0x9};
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

void right_rotation(uint8_t* bits, int rotate);
void encrypt(uint8_t* p, uint8_t* c);
void cipher_round(uint8_t* round_input, uint8_t* round_key);
void ARK(uint8_t* input_bits, uint8_t* round_key);
void SC(uint8_t* input_bits) ;
void key_scheduling(uint8_t* key, uint8_t* round_key);
uint64_t convert_into_64bits(uint8_t** matrix) ;
void MC(uint8_t* input_value);
uint8_t** convert_into_matrix(uint64_t input_bits, uint8_t** input_matrix) ;
void SR(uint8_t* input_bits);

void decrypt(uint8_t* c, uint8_t* p);
void decrypt_round(uint8_t* cipher_text, uint8_t* master_key);
void reverse_ARK(uint8_t* input_bits, uint8_t* round_key);
void reverse_SC(uint8_t* input_bits);
void reverse_SR(uint8_t* input_bits);
void reverse_MC(uint8_t* input_value);

void print_state(uint8_t *state) {

  for(int i = 0; i < 16;i++) {
    if(i && i%4==0) printf(" ");
    printf("%x", state[i]);
  }
  printf("\n");

}

void fprint_state(FILE* fh, uint8_t *state) {

  for(int i = 0; i < 16;i++) {
    if(i && i%4==0) fprintf(fh, " ");
    fprintf(fh, "%x", state[i]);
  }
  fprintf(fh, "\n");

}

void encrypt(uint8_t* p, uint8_t* c) {
    uint8_t master_key[] = MASTER_KEY;
    for (int i = 0; i < 16; i++) {
        c[i] = p[i];
    }
    uint8_t* key = malloc(sizeof(uint8_t) * 16);
    for (int i = 0; i < 16; i++) {
        key[i] = master_key[i];
    }
    for (int i = 0; i < 4; i++) {
        uint8_t* round_key = malloc(sizeof(uint8_t) * 16);
        key_scheduling(key, round_key);
        cipher_round(c, round_key);
        free(round_key);
    }
    free(key);
}

void right_rotation(uint8_t* bits, int rotate) {
    if (rotate != 0) {
        uint8_t temp = bits[15];
        for (int i = 15; i > 0; i--) {
            bits[i] = bits[i - 1];
        }
        bits[0] = temp;
        right_rotation(bits, rotate - 1);
    }
}

void cipher_round(uint8_t* round_input, uint8_t* round_key) {
    ARK(round_input, round_key);
    SC(round_input);
    SR(round_input);
    MC(round_input);
}

void ARK(uint8_t* input_bits, uint8_t* round_key) {
    for (int i = 0; i < 16; i++) {
        input_bits[i] ^= round_key[i];
    }
}

void SC(uint8_t* input_bits) {
    for (int i = 0; i < 16; i++) {
        input_bits[i] = sbox[input_bits[i]];
    }
}

void key_scheduling(uint8_t* key, uint8_t* round_key) {
    for (int i = 0; i < 16; i++) {
        round_key[i] = key[i];
    }
    for (int i = 0; i < 8; i++) {
        round_key[i] = 0;
    }
    
    key[15] ^= 0xf;
    key[14] ^= 0x3;
    key[13] ^= 0x3;
    key[12] ^= 0xf;
    right_rotation(key, 4);
}

void MC(uint8_t* input_value) {
    uint8_t temp[16];

    temp[0] = input_value[0] ^ input_value[2]  ; temp[4] = input_value[4] ^ input_value[6] ; temp[8]  = input_value[8] ^ input_value[10]  ; temp[12] = input_value[12] ^ input_value[14];
    temp[1] = input_value[1] ^ input_value[2]  ; temp[5] = input_value[5] ^ input_value[6] ; temp[9]  = input_value[9] ^ input_value[10]  ; temp[13] = input_value[13] ^ input_value[14];
    temp[2] = input_value[0]                   ; temp[6] = input_value[4]                  ; temp[10] = input_value[8]                    ; temp[14] = input_value[12];
    temp[3] = input_value[2] ^ input_value[3]  ; temp[7] = input_value[6] ^ input_value[7] ; temp[11] = input_value[10] ^ input_value[11] ; temp[15] = input_value[14] ^ input_value[15]; 

    memcpy(input_value, temp, 16*sizeof(uint8_t));
}

void SR(uint8_t* input_bits) {
  uint8_t temp[16];
  temp[0] = input_bits[0];
  temp[4] = input_bits[4];
  temp[8] = input_bits[8];
  temp[12] = input_bits[12];

  temp[1] = input_bits[5];
  temp[5] = input_bits[9];
  temp[9] = input_bits[13];
  temp[13] = input_bits[1];

  temp[2] = input_bits[10];
  temp[6] = input_bits[14];
  temp[10] = input_bits[2];
  temp[14] = input_bits[6];

  temp[3] = input_bits[15];
  temp[7] = input_bits[3];
  temp[11] = input_bits[7];
  temp[15] = input_bits[11];

  memcpy(input_bits, temp, 16*sizeof(uint8_t));

}

/**
 * Decryption part
 * 
 */


void decrypt(uint8_t* c, uint8_t* d) {
    uint8_t master_key[] = MASTER_KEY;
    for (int i = 0; i < 16; i++) {
        d[i] = c[i];
    }

    uint8_t* key = malloc(sizeof(uint8_t) * 16);
    for (int i = 0; i < 16; i++) {
        key[i] = master_key[i];
    }

    uint8_t** round_keys = calloc(1, sizeof(uint8_t*) * 4);
    for (int i = 0; i < 4; i++)
    {
        uint8_t* round_key = malloc(sizeof(uint8_t) * 16);
        key_scheduling(key, round_key);
        round_keys[i] = round_key;
        
    }

    for (int i = 0; i < 4; i++) {
        decrypt_round(d, round_keys[3 - i]);
        free(round_keys[3 - i]);
    }
    free(round_keys);
    free(key);

}

void decrypt_round(uint8_t* p, uint8_t* round_key) {
    reverse_MC(p);
    reverse_SR(p);
    reverse_SC(p);
    reverse_ARK(p, round_key);
}

void reverse_ARK(uint8_t* input_bits, uint8_t* round_key) {
    for (int i = 0; i < 16; i++) {
        input_bits[i] ^= round_key[i];
    }
}

void reverse_SC(uint8_t* input_bits) {
    for (int i = 0; i < 16; i++) {
        input_bits[i] = reverse_sbox[input_bits[i]];
    }
}

void reverse_SR(uint8_t* input_bits) {
    
  uint8_t temp[16];
  temp[0] = input_bits[0];
  temp[4] = input_bits[4];
  temp[8] = input_bits[8];
  temp[12] = input_bits[12];

  temp[1] = input_bits[13];
  temp[5] = input_bits[1];
  temp[9] = input_bits[5];
  temp[13] = input_bits[9];

  temp[2] = input_bits[10];
  temp[6] = input_bits[14];
  temp[10] = input_bits[2];
  temp[14] = input_bits[6];

  temp[3] = input_bits[7];
  temp[7] = input_bits[11];
  temp[11] = input_bits[15];
  temp[15] = input_bits[3];

  memcpy(input_bits, temp, 16*sizeof(uint8_t));
}

void reverse_MC(uint8_t* input_value) {
    uint8_t temp[16];

    temp[0] = input_value[2]                                    ; temp[4] = input_value[6]                                   ; temp[8] = input_value[10]                                     ; temp[12] = input_value[14];
    temp[1] = input_value[0] ^ input_value[1] ^ input_value[2]  ; temp[5] = input_value[4] ^ input_value[5] ^ input_value[6] ; temp[9] = input_value[8] ^ input_value[9] ^ input_value[10]   ; temp[13] = input_value[12] ^ input_value[13] ^ input_value[14];
    temp[2] = input_value[0] ^ input_value[2]                   ; temp[6] = input_value[4] ^ input_value[6]                  ; temp[10] = input_value[8] ^ input_value[10]                   ; temp[14] = input_value[12] ^ input_value[14];
    temp[3] = input_value[0] ^ input_value[2] ^ input_value[3]  ; temp[7] = input_value[4] ^ input_value[6] ^ input_value[7] ; temp[11] = input_value[8] ^ input_value[10] ^ input_value[11] ; temp[15] = input_value[12] ^ input_value[14] ^ input_value[15]; 

    memcpy(input_value, temp, 16*sizeof(uint8_t));
}
