#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Ciphers/TC01.h"
#include "../Tools/operations.h"

void print_state(uint8_t *state) {

  for(int i = 0; i < 16;i++) {
    if(i && i%4==0) printf(" ");
    printf("%x", state[i]);
  }
  printf("\n");

}

int main() {
    uint8_t const master_key[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1};
    uint8_t plain_text2[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,4};
    uint8_t plain_text[16] = {0,0,0,0, 0,0,1,0, 0,0,0,0, 0,0,0,8};
    uint8_t* cipher_text1; uint8_t* cipher_text2;
    uint8_t* decrypted1; uint8_t* decrypted2;
    printf("plaintext1 ");
    print_state(plain_text);
    printf("plaintext2 ");
    print_state(plain_text2);
    cipher_text1 = cipher(plain_text, master_key);
    printf("ciphertext1 ");
    print_state(cipher_text1);
    cipher_text2 = cipher(plain_text2, master_key);
    printf("ciphertext2 ");
    print_state(cipher_text2);
    decrypted1 = decrypt(cipher_text1, master_key);
    printf("decription1 ");
    print_state(decrypted1);
    decrypted2 = decrypt(cipher_text2, master_key);
    printf("decription2 ");
    print_state(decrypted2);

    return 0;
}


uint8_t* cipher(uint8_t* plain_text, const uint8_t* master_key) {
    uint8_t* data = malloc(sizeof(uint8_t) * 16);
    for (int i = 0; i < 16; i++) {
        data[i] = plain_text[i];
    }
    uint8_t* key = malloc(sizeof(uint8_t) * 16);
    for (int i = 0; i < 16; i++) {
        key[i] = master_key[i];
    }
    

    for (int i = 0; i < 4; i++) {
        uint8_t* round_key = malloc(sizeof(uint8_t) * 16);
        key_scheduling(key, round_key);
        cipher_round(data, round_key);
        free(round_key);
    }
    free(key);
    return data;
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


uint8_t* decrypt(uint8_t* cipher_text, const uint8_t* master_key) {
    uint8_t* data = malloc(sizeof(uint8_t) * 16);
    for (int i = 0; i < 16; i++) {
        data[i] = cipher_text[i];
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
        decrypt_round(data, round_keys[3 - i]);
        free(round_keys[3 - i]);
    }
    free(round_keys);
    free(key);

    return data;
}

void decrypt_round(uint8_t* round_input, uint8_t* round_key) {
    reverse_MC(round_input);
    reverse_SR(round_input);
    reverse_SC(round_input);
    reverse_ARK(round_input, round_key);
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
