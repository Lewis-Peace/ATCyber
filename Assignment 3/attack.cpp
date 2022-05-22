#include "../Assignment 3/Code/TC01.cpp"
#include <iostream>
#include <cstdint>
#include <ctime> 
#include <cstring>
#include <assert.h>
#include <vector>

#define TRUE 1
#define FALSE 0

#define PLAIN_NUMBER 75 * 50
#define empty_data (uint8_t*) calloc(16, sizeof(uint8_t))

int possible_keys[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

std::vector<int> get_valid_keys(int key_nibble) {
    std::vector<int> a;
    int temp = 0;
    for (int i = 0; i < 16; i++) {
        if (possible_keys[key_nibble][i] > temp) {
            temp = possible_keys[key_nibble][i];
        }
    }
    for (int i = 0; i < 16; i++) {
        if (possible_keys[key_nibble][i] == temp) {
            a.push_back(i);
        }
    }
    return a;
}

void local_decrypt(uint8_t *ct,  uint8_t *pt, uint8_t* key) {
  uint8_t roundkey[16];
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

void convert_bits_to_array(uint8_t* output_bits, uint64_t input_bits) {
    for (int i = 0; i < 16; i++) {
        output_bits[15 - i] = input_bits & 0xF;
        input_bits = input_bits >> 4;
    }
}

uint8_t* xor_values_obtaining_new(uint8_t* x, uint8_t* y) {
    uint8_t* difference = empty_data;
    for (int i = 0; i < 16; i++) {
        difference[i] = x[i] ^ y[i];
    }
    return difference;
}

void xor_values_modifying_one(uint8_t* to_be_modified, uint8_t* y) {
    for (int i = 0; i < 16; i++) {
        to_be_modified[i] ^= y[i];
    }
}

int compare_values(uint8_t* x, uint8_t* y) {
    for (int i = 0; i < 16; i++) {
        if (x[i] != y[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

void get_random_plain_text(uint8_t* p1) {
    for (int i = 0; i < 16; i++) {
        p1[i] = (rand() % 16);
    }
}

void print_key_scheduling() {
    uint8_t master_key[16] = MASTER_KEY;
    uint8_t* round_key = empty_data;
    for (int i = 0; i < 4; i++) {
        key_scheduling(master_key, round_key);
        //printf("Round %i ", i);
        //print_state(round_key);
    }
    //printf("\n");
}

void do_inverse_round(uint8_t* c) {
    inverse_mixcol(c);
    inverse_shift_row(c);
    inverse_subcell(c);
}

void do_a_round(uint8_t* p) {
    subcell(p);
    shift_row(p);
    mixcol(p);
}

int count = 0;
uint8_t key_guess[16] = empty_array;
void possible_keys_print(int index) {
    int temp = 0;
    for (int i = 0; i < 16; i++) {
        if (possible_keys[index][i] > possible_keys[index][temp])
        {
            temp = i;
        }
    }

    key_guess[index] = temp;
    
    assert(count != 0);
    printf("Should be %X with %i right guess and used %i plain\n", temp, possible_keys[index][temp], count);
}

uint8_t** append(uint8_t** old_list, int len, uint8_t* value) {
    uint8_t** list;
    if (len == 0) {
        list = (uint8_t**) malloc(sizeof(uint8_t*));
        list[0] = value;
    } else {
        list = (uint8_t**) malloc(sizeof(uint8_t*) * (len + 1));
        for (int i = 0; i < len; i++)
        {
            list[i] = old_list[i];
        }
        free(old_list);
        list[len] = value;
    }
    return list;
}

uint8_t* clone(uint8_t* x) {
    uint8_t* clone = empty_data;
    for (int i = 0; i < 16; i++)
    {
        clone[i] = x[i];
    }
    return clone;
}

void attack(uint8_t starting_diff, int input_sbox, uint8_t second_diff, int second_sbox, uint8_t ending_diff, int end_sbox) {
    for (int i = 0; i < PLAIN_NUMBER; i++) {
        uint8_t* p1 = empty_data;
        uint8_t* p2 = empty_data;
        uint8_t* c1 = empty_data;
        uint8_t* c2 = empty_data;
        get_random_plain_text(p1);
        for (int s = 0; s < 16; s++) {
            p2[s] = p1[s];
        }
        p2[input_sbox] ^= starting_diff;
        encrypt(p1, c1);
        encrypt(p2, c2);
        do_inverse_round(c1);
        do_inverse_round(c2);
        if ((c1[end_sbox] ^ c2[end_sbox]) == ending_diff) {
            
            for (uint8_t k = 0; k < 16; k++)
            {
                uint8_t* p1_temp = empty_data;
                uint8_t* p2_temp = empty_data;

                for (int s = 0; s < 16; s++) {
                    p1_temp[s] = p1[s];
                    p2_temp[s] = p2[s];
                }
                

                p2_temp[input_sbox] ^= k;
                p1_temp[input_sbox] ^= k;
                subcell(p1_temp);
                subcell(p2_temp);
                if ((p1_temp[second_sbox] ^ p2_temp[second_sbox]) == second_diff) {
                    possible_keys[input_sbox][k]++;
                }
                free(p1_temp);
                free(p2_temp);
            }

            count++;
        }
        free(p1);
        free(c1);
        free(c2);
    }
    possible_keys_print(input_sbox);
    count = 0;
    memset(possible_keys, 0, sizeof(uint8_t) * 16);
}

void attack2(int input_sbox) {
    for (int i = 0; i < PLAIN_NUMBER; i++) {
        uint8_t* p1 = empty_data;
        uint8_t* p2 = empty_data;
        uint8_t* c1 = empty_data;
        uint8_t* c2 = empty_data;
        get_random_plain_text(p1);
        for (int s = 0; s < 16; s++) {
            p2[s] = p1[s];
        }
        p2[input_sbox] ^= 0xc;
        encrypt(p1, c1);
        encrypt(p2, c2);
        do_inverse_round(c1);
        do_inverse_round(c2);
        uint8_t check_1 = (c1[input_sbox - 8] ^ c2[input_sbox - 8]) == 1;
        uint8_t check_2 = (c1[input_sbox - 6] ^ c2[input_sbox - 6]) == 1;
        uint8_t check_3 = (c1[input_sbox + 2] ^ c2[input_sbox + 2]) == 1;
        if (check_1 && check_2 && check_3) {
            
            for (uint8_t k = 0; k < 16; k++)
            {
                uint8_t* p1_temp = empty_data;
                uint8_t* p2_temp = empty_data;

                for (int s = 0; s < 16; s++) {
                    p1_temp[s] = p1[s];
                    p2_temp[s] = p2[s];
                }
                

                p2_temp[input_sbox] ^= k;
                p1_temp[input_sbox] ^= k;
                subcell(p1_temp);
                subcell(p2_temp);
                if ((p1_temp[input_sbox] ^ p2_temp[input_sbox]) == 0x8) {
                    possible_keys[input_sbox][k]++;
                }
                free(p1_temp);
                free(p2_temp);
            }

            count++;
        }
        free(p1);
        free(c1);
        free(c2);
    }
    possible_keys_print(input_sbox);
    count = 0;
    memset(possible_keys, 0, sizeof(uint8_t) * 16);
}


void attack3(int input_sbox) {
    for (int i = 0; i < PLAIN_NUMBER; i++) {
        uint8_t* p1 = empty_data;
        uint8_t* p2 = empty_data;
        uint8_t* c1 = empty_data;
        uint8_t* c2 = empty_data;
        get_random_plain_text(p1);
        for (int s = 0; s < 16; s++) {
            p2[s] = p1[s];
        }
        p2[input_sbox] ^= 0xc;
        encrypt(p1, c1);
        encrypt(p2, c2);
        do_inverse_round(c1);
        do_inverse_round(c2);
        uint8_t check_1 = (c1[input_sbox - 10] ^ c2[input_sbox - 10]) == 1;
        uint8_t check_2 = (c1[input_sbox - 8] ^ c2[input_sbox - 8]) == 1;
        uint8_t check_3 = (c1[input_sbox - 2] ^ c2[input_sbox - 2]) == 1;
        if (check_1 && check_2 && check_3) {
            
            for (uint8_t k = 0; k < 16; k++)
            {
                uint8_t* p1_temp = empty_data;
                uint8_t* p2_temp = empty_data;

                for (int s = 0; s < 16; s++) {
                    p1_temp[s] = p1[s];
                    p2_temp[s] = p2[s];
                }
                

                p2_temp[input_sbox] ^= k;
                p1_temp[input_sbox] ^= k;
                subcell(p1_temp);
                subcell(p2_temp);
                if ((p1_temp[input_sbox] ^ p2_temp[input_sbox]) == 0x8) {
                    possible_keys[input_sbox][k]++;
                }
                free(p1_temp);
                free(p2_temp);
            }

            count++;
        }
        free(p1);
        free(c1);
        free(c2);
    }
    possible_keys_print(input_sbox);
    count = 0;
    memset(possible_keys, 0, sizeof(uint8_t) * 16);
}

void get_nibble_value(int nibble_index, int delete_from_array, uint8_t* key) {
    int temp = 0;
    for (int i = 0; i < 16; i++) {
        if (possible_keys[nibble_index][i] > possible_keys[nibble_index][temp])
        {
            temp = i;
        }
    }
    key[nibble_index] = temp;
    if (delete_from_array == TRUE) {
        possible_keys[nibble_index][temp] = 0;
    } 
}

void test_key(uint8_t* key, uint8_t* c, uint8_t* p) {
    uint8_t* d = empty_data;
    uint8_t* temp_c = empty_data;
    for (int z = 0; z < 16; z++) { temp_c[z] = c[z]; }
    local_decrypt(temp_c, d, key);
    free(temp_c);
    int condition = TRUE;
    for (int y = 0; y < 16; y++) {
        condition &= p[y] == d[y];
        if (condition == FALSE) { break; }
    }
    if (condition == TRUE) {
        print_state(key);
        exit(0);
    }
}

int main() {
    uint8_t key[] = MASTER_KEY;
    srand((unsigned)time(0)); 
    print_state(key);
    for (int i = 12; i < 16; i++) {
        int j = i - 1;
        if (j == 11) {
            j = 15;
        }
        attack(0xc, i, 8, i, 1, j);
    }
    attack2(8);
    attack2(9);
    //attack3(10);
    //attack3(11);
    // Attak and attack 2 are consistently good, attack3 sometimes doesn't even get the key by the highest
    print_state(key_guess);
    
    FILE* fh = fopen("guess_list.txt", "w");
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            fprintf(fh, "%i ", possible_keys[i][j]);
        }
        fprintf(fh, "\n");
    }
    std::vector<std::vector<int>> valid_keys_vectors;
    for (int i = 8; i < 16; i++)
    {
        std::vector<int> v = get_valid_keys(i);
        valid_keys_vectors.push_back(v);
        for (int j: v)
        {
            printf("%X ", j);
        }
        printf("\n");
        
    }

    // Bruteforce for the other bits
    uint8_t* p = empty_data;
    uint8_t* c = empty_data;
    int change8 = FALSE;
    int change9 = FALSE;
    int change10 = FALSE;
    int change11 = FALSE;
    int change12 = FALSE;
    int change13 = FALSE;
    int change14 = FALSE;
    int change15 = FALSE;
    get_random_plain_text(p);
    encrypt(p, c);
    uint8_t* master_key_guess = empty_data;
    for (uint64_t i = 0x100000000; i <= 0xFFFFFFFF00000000; i = i + 0x100000000)
    {
        convert_bits_to_array(master_key_guess, i);

        for (std::vector<int> keys_vector: valid_keys_vectors) {
            
            test_key(master_key_guess, c, p);
        }

    }
    
    return 0;
}
