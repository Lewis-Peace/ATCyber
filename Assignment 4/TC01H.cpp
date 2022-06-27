#include <iostream>
#include <string.h>
#include <cstdlib>
#include <inttypes.h>

void print_state(uint8_t *state) {

  for(int i = 0; i < 16;i++) {
    if(i && i%4==0) printf(" ");
    printf("%x", state[i]);
  }
  printf("\n");

}

class TC01H
{
private:

                    //   0,  1, 2, 3, 4, 5,  6,  7,  8,   9,  a, b, c,  d,   e,  f
    uint8_t sbox[16] = {0xa, 0x5, 0x4, 0x2, 0x6, 0x1, 0xf, 0x3, 0xb, 0xe, 0x7, 0, 0x8, 0xd, 0xc, 0x9};
                            //   0   1  2  3  4  5  6   7    8    9   A  B   C    D   E  F
    uint8_t reverse_sbox[16] = {0xb, 5, 3, 7, 2, 1, 4, 0xa, 0xc, 0xf, 0, 8, 0xe, 0xd, 9, 6};

    uint8_t round_constant[4][16] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0xf,0x3,0x3,0xf,0,0,0,0}
    };

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

    void cipher_round(uint8_t* round_input, int round) {
        ARK(round_input, round);
        SC(round_input);
        SR(round_input);
        MC(round_input);
    }

    void ARK(uint8_t* input_bits, int round) {
        for (int i = 0; i < 16; i++) {
            input_bits[i] ^= round_constant[round][i];
        }
    }

    void SC(uint8_t* input_bits) {
        for (int i = 0; i < 16; i++) {
            input_bits[i] = sbox[input_bits[i]];
        }
    }

    void MC(uint8_t* input_value) {
        uint8_t temp[16];

        temp[0]  = input_value[0] ^ input_value[4]  ; temp[1]  = input_value[1] ^ input_value[5]  ; temp[2]  = input_value[2] ^ input_value[6]  ; temp[3]  = input_value[3] ^ input_value[7] ;
        temp[4]  = input_value[0] ^ input_value[8]  ; temp[5]  = input_value[1] ^ input_value[9]  ; temp[6]  = input_value[2] ^ input_value[10] ; temp[7]  = input_value[3] ^ input_value[11];
        temp[8]  = input_value[4] ^ input_value[8]  ; temp[9]  = input_value[5] ^ input_value[9]  ; temp[10] = input_value[6] ^ input_value[10] ; temp[11] = input_value[7] ^ input_value[11];
        temp[12] = input_value[4] ^ input_value[12] ; temp[13] = input_value[5] ^ input_value[13] ; temp[14] = input_value[6] ^ input_value[14] ; temp[15] = input_value[7] ^ input_value[15]; 

        memcpy(input_value, temp, 16*sizeof(uint8_t));
    }

    void SR(uint8_t* input_bits) {
    uint8_t temp[16];
    temp[0] = input_bits[0];
    temp[1] = input_bits[1];
    temp[2] = input_bits[2];
    temp[3] = input_bits[3];

    temp[4] = input_bits[5];
    temp[5] = input_bits[6];
    temp[6] = input_bits[7];
    temp[7] = input_bits[4];

    temp[8] = input_bits[10];
    temp[9] = input_bits[11];
    temp[10] = input_bits[8];
    temp[11] = input_bits[9];

    temp[12] = input_bits[15];
    temp[13] = input_bits[12];
    temp[14] = input_bits[13];
    temp[15] = input_bits[14];

    memcpy(input_bits, temp, 16*sizeof(uint8_t));

    }

    void truncate(uint8_t* data) {
        for (int i = 8; i < 16; i++) {
            data[i] = 0;
        }
    }

    void encrypt(uint8_t* plain_text, uint8_t* cipher_text) {
        for (int i = 0; i < 16; i++) {
            cipher_text[i] = plain_text[i];
        }
        
        for (int i = 0; i < 4; i++) {
            cipher_round(cipher_text, i);
        }
    }

public:

    void hash(uint8_t* plain_text, uint8_t* cipher_text) {
        encrypt(plain_text, cipher_text);
        truncate(cipher_text);
    }
    
};

void bruteforce_test() {
    TC01H cipher = TC01H();
    uint8_t plain_text[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,5,0};
    uint8_t plain_text2[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,5,0};
    bool collision = true;
    for (int k = 0; k < 16; k++)
    {
    for (int i = 1; i < 16; i++)
    {
        plain_text2[k] = i;
        uint8_t* cipher_text1 = (uint8_t*) calloc(16, sizeof(uint8_t));
        uint8_t* cipher_text2 = (uint8_t*) calloc(16, sizeof(uint8_t));
        cipher.hash(plain_text, cipher_text1);
        cipher.hash(plain_text2, cipher_text2);
        for (int j = 0; j < 16; j++) {
            if (cipher_text1[j] != cipher_text2[j]) {
                collision = false;
            }
        }
        if (collision == true) {
            //print_state(plain_text);
            //print_state(plain_text2);
            //print_state(cipher_text1);
            //print_state(cipher_text2);
            printf("Collision found k = %i, i = %i\n", k, i);
        }
        collision = true;
        plain_text2[k] = 0;
    }
    }
}

int main() {
    TC01H cipher = TC01H();
    uint8_t plain_text[16] = {0,0,5,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    uint8_t plain_text2[16] = {0,0,5,0, 0,0,0,0, 0,0,0,0, 5,12,11,0};
    uint8_t* cipher_text1 = (uint8_t*) calloc(16, sizeof(uint8_t));
    uint8_t* cipher_text2 = (uint8_t*) calloc(16, sizeof(uint8_t));
    cipher.hash(plain_text, cipher_text1);
    cipher.hash(plain_text2, cipher_text2);
    print_state(plain_text);
    print_state(plain_text2);
    print_state(cipher_text1);
    print_state(cipher_text2);

    //bruteforce_test();

    return 0;
}
