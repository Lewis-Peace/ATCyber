#include <iostream>
#include <cstdint>
#include <cstring>

#define empty_data (uint8_t*) calloc(16, sizeof(uint8_t))

class Hash01 {
    public: 
        uint8_t key[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1};

        static void print_state(uint8_t* s) {
            for (int i = 0; i < 16; i++) {
                printf("%X", s[i]);
                (i + 1) % 4 == 0 ? printf(" "): 0;
                i < 8 ? s[i] = 0: 0;
            }
            printf("\n");
        }

        void hash(uint8_t* plain, uint8_t* hash) {
            uint8_t* c = empty_data;
            encrypt(plain, c);
            //print_state(c);
            memcpy(hash, c, 16 * sizeof(uint8_t));
        }

    private:
        
        uint8_t sbox[16] = {0xa, 0x5, 0x4, 0x2, 0x6, 0x1, 0xf, 0x3, 0xb, 0xe, 0x7, 0, 0x8, 0xd, 0xc, 0x9};
    
        void subcell(uint8_t *state) {
            for(int i = 0;i < 16;i++) state[i] = sbox[state[i]];
        }
        
        void shift_row(uint8_t* state) {

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
        
        void mixcol(uint8_t* state) {
            uint8_t temp[16];

            temp[0] = state[0]^state[8]; temp[1] = state[1]^state[9]; temp[2] = state[2]^state[10]; temp[3] = state[3]^state[11];
            temp[4] = state[4]^state[8]; temp[5] = state[5]^state[9]; temp[6] = state[6]^state[10]; temp[7] = state[7]^state[11];
            temp[8] = state[0]; temp[9] = state[1]; temp[10] = state[2]; temp[11] = state[3];
            temp[12] = state[8]^state[12]; temp[13] = state[9]^state[13]; temp[14] = state[10]^state[14]; temp[15] = state[11]^state[15];

            memcpy(state, temp, 16*sizeof(uint8_t));
        }
        
        void encrypt(uint8_t *pt,  uint8_t *ct) {

            uint8_t* key = this->key;
            uint8_t roundkey[16];
            memset(roundkey, 0, 16*sizeof(uint8_t) );
            

            int nRound = 14; // Total number of rounds of the cipher
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
};

void get_random_plain(uint8_t* p) {
    for (int i = 0; i < 16; i++) {
        p[i] = (rand() % 16);
    }
}

int main() {
    uint8_t p[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    uint8_t* h = empty_data;
    srand(time(0));
    Hash01 t;
    for (int i = 0; i < 16; i++) {
        get_random_plain(p);
        t.hash(p, h);
        for (int i = 8; i < 16; i++) {
            printf("%X", h[i]);
            (i + 1) % 4 == 0 ? printf(" "): 0;
        }
        printf("\n");
    }
    return 0;
}