/**

This is a C implementation for the the TC01 cipher. 

The key scheduling is not included. You should write a program for generating round keys from a given 64 bit key 

author: arnab roy 

 **/



#include <iostream>
#include <cstdint>
#include <cstring>

#define MASTER_KEY {7,6,9,3, 1,0xF,0xA,0xC, 9,0xD,0xA,0xB, 2,0xB,3,6}

#define rounds 4

uint8_t sbox[] = {0xa, 0x5, 0x4, 0x2, 0x6, 0x1, 0xf, 0x3, 0xb, 0xe, 0x7, 0, 0x8, 0xd, 0xc, 0x9};

uint8_t inverse_sbox[16] = {0xb, 5, 3, 7, 2, 1, 4, 0xa, 0xc, 0xf, 0, 8, 0xe, 0xd, 9, 6};


void add_roundkey(uint8_t* state, uint8_t* rkey)
{

  for(int i = 0;i < 16;i++)
    state[i] ^= rkey[i];

}

/*
An array of plaintext or state say A[0], ... , A[15] is mapped to a 4 X 4 matrix as

   A[0] A[1] A[2] A[3]
   A[4] ... 

   A[12] ...     A[15]

*/


void mixcol(uint8_t* state)
{

  uint8_t temp[16];

  temp[0] = state[0]^state[8]; temp[1] = state[1]^state[9]; temp[2] = state[2]^state[10]; temp[3] = state[3]^state[11];
  temp[4] = state[4]^state[8]; temp[5] = state[5]^state[9]; temp[6] = state[6]^state[10]; temp[7] = state[7]^state[11];
  temp[8] = state[0]; temp[9] = state[1]; temp[10] = state[2]; temp[11] = state[3];
  temp[12] = state[8]^state[12]; temp[13] = state[9]^state[13]; temp[14] = state[10]^state[14]; temp[15] = state[11]^state[15];

  memcpy(state, temp, 16*sizeof(uint8_t));

}

void inverse_mixcol(uint8_t* state)
{

  uint8_t temp[16];

  temp[0] = state[8]; temp[1] = state[9]; temp[2] = state[10]; temp[3] = state[11];
  temp[4] = state[0]^state[4]^state[8]; temp[5] = state[1]^state[5]^state[9]; temp[6] = state[2]^state[6]^state[10]; temp[7] = state[3]^state[7]^state[11];
  temp[8] = state[0]^state[8]; temp[9] = state[9]^state[1]; temp[10] = state[10]^state[2]; temp[11] = state[11]^state[3];
  temp[12] = state[0]^state[8]^state[12]; temp[13] = state[1]^state[9]^state[13]; temp[14] = state[2]^state[10]^state[14]; temp[15] = state[3]^state[11]^state[15];

  memcpy(state, temp, 16*sizeof(uint8_t));

}


void shift_row(uint8_t* state)
{

  uint8_t temp[16];
  int i;
  for(i = 0;i < 4;i++)
    temp[i] = state[i];

  temp[4] = state[5];
  temp[5] = state[6];
  temp[6] = state[7];
  temp[7] = state[4];

  temp[8] = state[10];
  temp[9] = state[11];
  temp[10] = state[8];
  temp[11] = state[9];

  temp[12] = state[15];
  temp[13] = state[12];
  temp[14] = state[13];
  temp[15] = state[14];

  memcpy(state, temp, 16*sizeof(uint8_t));

}

void inverse_shift_row(uint8_t* state)
{

  uint8_t temp[16];
  int i;
  for(i = 0;i < 4;i++)
    temp[i] = state[i];

  temp[5] = state[4];
  temp[6] = state[5];
  temp[7] = state[6];
  temp[4] = state[7];

  temp[10] = state[8];
  temp[11] = state[9];
  temp[8] = state[10];
  temp[9] = state[11];

  temp[15] = state[12];
  temp[12] = state[13];
  temp[13] = state[14];
  temp[14] = state[15];

  memcpy(state, temp, 16*sizeof(uint8_t));

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

void left_rotation(uint8_t* bits, int rotate) {
    if (rotate != 0) {
        uint8_t temp = bits[0];
        for (int i = 0; i < 16; i++) {
            bits[i] = bits[i + 1];
        }
        bits[15] = temp;
        left_rotation(bits, rotate - 1);
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


void subcell(uint8_t *state)
{
  for(int i = 0;i < 16;i++) state[i] = sbox[state[i]];
}

void inverse_subcell(uint8_t *state)
{
  for(int i = 0;i < 16;i++) state[i] = inverse_sbox[state[i]];
}


void print_state(uint8_t *state) {

  for(int i = 0; i < 16;i++) {
    if(i && i%4==0) printf(" ");
    printf("%x", state[i]);
  }
  printf("\n");

}



void encrypt(uint8_t *pt,  uint8_t *ct)
{

  uint8_t key[16] = MASTER_KEY;
  uint8_t roundkey[16];
  memset(roundkey, 0, 16*sizeof(uint8_t) );
  

  int nRound = rounds; // Total number of rounds of the cipher
  uint8_t state[16];
  memcpy(state, pt, 16*sizeof(uint8_t));

  uint8_t tk[16];
  memcpy(tk, key, 16*sizeof(uint8_t));
  
  for(int i = 0;i < nRound;i++) {

    memcpy(roundkey, tk, 16*sizeof(uint8_t));
    memset(roundkey, 0, 8*sizeof(uint8_t));
    //printf("round key at round %d\n", i);
    //print_state(roundkey);

    for(int j = 8;j < 16;j++) state[j] ^= roundkey[j];

    subcell(state);
    shift_row(state);
    mixcol(state);
    
    tk[12] ^= 0xf; tk[14] ^= 0x3;
    tk[13] ^= 0x3; tk[15] ^= 0xf;    // k[i]
    uint8_t temp[16];
    memcpy(temp, tk, 16*sizeof(uint8_t));
    for(int j = 15;j >= 4; j--) tk[j] = temp[j-4];
    tk[0] = temp[12]; tk[1] = temp[13];
    tk[2] = temp[14]; tk[3] = temp[15];
  }

  memcpy(ct, state, 16*sizeof(uint8_t));

  //print_state(state);
}

#define empty_array {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
void decrypt(uint8_t *ct,  uint8_t *pt) {
  uint8_t roundkey[16];
  uint8_t key[16] = MASTER_KEY;
  memset(roundkey, 0, 16*sizeof(uint8_t) );
  
  int nRound = rounds; // Total number of rounds of the cipher
  uint8_t state[16];
  memcpy(state, ct, 16*sizeof(uint8_t));

  uint8_t round_keys[4][16] = {empty_array, empty_array, empty_array, empty_array};

  for (int i = 0; i < 4; i++)
  {
    key_scheduling(key, roundkey);
    for (int j = 0; j < 16; j++)
    {
      round_keys[i][j] = roundkey[j];
    }
    
  }
  
  for (int l = nRound; l > 0; l--)
  {
    //printf("round key at round %d\n", l);
    //print_state(round_keys[l - 1]);
    inverse_mixcol(state);

    inverse_shift_row(state);

    inverse_subcell(state);

    for (int j = 8; j < 16; j++)
    {
      state[j] ^= round_keys[l - 1][j];
    }

    memcpy(pt, state, 16*sizeof(uint8_t));
    
  }
  

}
