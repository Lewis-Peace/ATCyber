#include "../Assignment 3/Code/TAES01.cpp"
#include <iostream>
#include <cstdint>
#include <vector>

#define empty_data (uint8_t*) calloc(16, sizeof(uint8_t))

#define plains_number 4096 * 128

void get_random_plain_text(uint8_t* p1) {
    for (int i = 0; i < 16; i++) {
        p1[i] = (rand() % 0xFF);
    }
}

void do_inverse_round(uint8_t* x) {
    inverse_mixcol(x);
    inverse_shift_row(x);
    inverse_subcell(x);
}
void do_round(uint8_t* x) {
    subcell(x);
    shift_row(x);
    mixcol(x);
}

void print_valid_keys(uint8_t* valid_keys) {
    std::vector<int> a;
    int temp = 0;
    for (int i = 0; i < 256; i++) {
        if (valid_keys[i] > temp) {
            temp = valid_keys[i];
        }
    }
    for (int i = 0; i < 256; i++) {
        if (valid_keys[i] == temp) {
            a.push_back(i);
        }
    }/*
    FILE* fh = fopen("key_guesses_TCAES01.txt", "w");
    for (int i = 0; i < 256; i++) {
        fprintf(fh, "%i ", valid_keys[i]);
    }
    fprintf(fh, "\n");*/

    for (int i = 0; i < a.size(); i++) {
        printf("%X ", a[i]);
    }
    printf("\n");
}

int xoring(uint8_t* a, uint8_t* b) {
    uint8_t xor_value;
    for (int i = 0; i < 16; i++) {
        xor_value = (a[i] ^ b[i]);
        if (xor_value != 0) {
            return xor_value;
        }
    }
    return 0;
}

void attack(int starting_sbox, int middle_sbox, int end_sbox) {
    uint8_t* valid_keys_1 = (uint8_t*) calloc(256, sizeof(uint8_t));
    uint8_t* valid_keys_2 = (uint8_t*) calloc(256, sizeof(uint8_t));
    for (int qwerty = 0; qwerty < plains_number; qwerty++) {
        uint8_t* p1 = empty_data;
        uint8_t* c1 = empty_data;
        uint8_t* p2 = empty_data;
        uint8_t* c2 = empty_data;
        get_random_plain_text(p1);
        for (int i = 0; i < 16; i++) {
            p2[i] = p1[i];
        }
        p2[starting_sbox] ^= 0x5; 

        encrypt(p1, c1);
        encrypt(p2, c2);
        // Filtering
        if (xoring(c1, c2) == 0x53) {
            for (uint8_t key_guess_1 = 0; key_guess_1 < 0xFF; key_guess_1++) {
                uint8_t* p1_temp = empty_data;
                uint8_t* p2_temp = empty_data;
                for (int i = 0; i < 16; i++) {
                    p1_temp[i] = p1[i];
                    p2_temp[i] = p2[i];
                }
                do_round(p1_temp);
                do_round(p2_temp);
                p1_temp[middle_sbox] ^= key_guess_1;
                p2_temp[middle_sbox] ^= key_guess_1;
                subcell(p1_temp);
                subcell(p2_temp);
                if (xoring(p1_temp, p2_temp) == 0x53) {
                    for (uint8_t key_guess_2 = 0; key_guess_2 < 0xFF; key_guess_2++) {
                        uint8_t* c1_temp = empty_data;
                        uint8_t* c2_temp = empty_data;
                        for (int i = 0; i < 16; i++) {
                            c1_temp[i] = c1[i];
                            c2_temp[i] = c2[i];
                        }
                        c1_temp[end_sbox] ^= key_guess_2;
                        c2_temp[end_sbox] ^= key_guess_2;
                        do_inverse_round(c1_temp);
                        do_inverse_round(c2_temp);

                        if (xoring(c1_temp, c2_temp) == 8) {
                            valid_keys_1[key_guess_1]++;
                            valid_keys_2[key_guess_2]++;
                        }

                        free(c1_temp);
                        free(c2_temp);
                    }
                }
                free(p1_temp);
                free(p2_temp);
            }
        }

        free(p1);
        free(c1);
        free(p2);
        free(c2);
    }
    
    print_valid_keys(valid_keys_1);
    print_valid_keys(valid_keys_2);
    
    free(valid_keys_1);
    free(valid_keys_2);
}

void print_key_scheduling(uint8_t* key) {
    for (int i = 0; i < 2; i++)
    {
        uint8_t* round_key = (uint8_t*) calloc(16, sizeof(uint8_t));
        for (int i = 0; i < 16; i++) {
            round_key[i] = key[i];
        }

        printf("Subkey %i: ", i);
        print_state(round_key);
        
        key[15] ^= 0xff;
        key[14] ^= 0x33;
        key[13] ^= 0x33;
        key[12] ^= 0xff;
        right_rotation(key, 4);
        free(round_key);
    }
}

int main() {
    uint8_t master_key[] = MASTER_KEY;
    print_key_scheduling(master_key);
    printf("\n15\n");
    attack(15, 12, 13);
    printf("\n12\n");
    attack(12, 13, 14);
    printf("\n13\n");
    attack(13, 14, 15);
    printf("\n14\n");
    attack(14, 15, 12);
    return 0;
}