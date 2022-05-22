#include <stdio.h>
#include "../Assignment 3/Code/TC01.h"

#define TRUE 1
#define FALSE 0

#define PLAIN_NUMBER 300

int possible_keys[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void convert_bits_to_array(uint8_t* output_bits, uint64_t input_bits) {
    for (int i = 0; i < 16; i++) {
        output_bits[15 - i] = input_bits & 0xF;
        input_bits = input_bits >> 4;
    }
}

uint64_t convert_array_to_bits(uint8_t* input_bits) {
    uint64_t result = 0;
    for (int i = 0; i < 16; i++) {
        result += input_bits[15-i];
        result = result << 4;
    }
    return result;
}

uint8_t* xor_values_obtaining_new(uint8_t* x, uint8_t* y) {
    uint8_t* difference = malloc(sizeof(uint8_t) * 16);
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

uint8_t* get_random_plain_text(uint8_t* p1) {
    for (int i = 0; i < 16; i++) {
        p1[i] = rand() % 16;
    }
}

void print_key_scheduling() {
    uint8_t master_key[16] = MASTER_KEY;
    uint8_t* round_key = malloc(sizeof(uint8_t) * 16);
    for (int i = 0; i < 4; i++) {
        key_scheduling(master_key, round_key);
        printf("Round %i ", i);
        print_state(round_key);
    }
    printf("\n");
}

void do_inverse_round(uint8_t* c) {
    reverse_MC(c);
    reverse_SR(c);
    reverse_SC(c);
}

void do_a_round(uint8_t* p) {
    SC(p);
    SR(p);
    MC(p);
}

int good_pair(uint8_t* c1, uint8_t* c2, uint8_t* ending_difference) {
    uint8_t* difference;
    do_inverse_round(c1);
    do_inverse_round(c2);
    difference = xor_values_obtaining_new(c1, c2);
    if (compare_values(difference, ending_difference)) {
        free(difference);
        printf("t");
        return TRUE;
    } 
    free(difference);
    return FALSE;
}

void top_down(uint8_t* p1, uint8_t* p2, int active_subcell) {
    uint8_t* difference = malloc(sizeof(uint8_t) * 16);
    for (uint8_t key_guess = 0; key_guess < 16; key_guess++) {
        p1[active_subcell] ^= key_guess;
        p2[active_subcell] ^= key_guess;
        SC(p1);
        SC(p2);
        difference = xor_values_obtaining_new(p1, p2);
        if (difference[active_subcell] == 0x8) {
            possible_keys[key_guess]++;
        }
    }
}


void bottom_up(uint8_t* c1, uint8_t* c2) {
    uint8_t* difference;
    for (uint8_t key_guess = 0; key_guess < 16; key_guess++) {
        for (uint8_t j = 0; j < 16; j++) {
            c1[11] ^= key_guess;
            c2[11] ^= key_guess;
            c1[11] ^= j;
            c2[11] ^= j;
            do_inverse_round(c1);
            do_inverse_round(c2);
            difference = xor_values_obtaining_new(c1, c2);
            if (difference[7] == 0x3) {
                possible_keys[key_guess]++;
            }
        }  
    }
}

void possible_keys_print() {
    int temp = 0;
    for (int i = 0; i < 16; i++) {
        if (possible_keys[i] > possible_keys[temp])
        {
            temp = i;
        }
    }
    for (int i = 0; i < 16; i++)
    {
        printf("%i ", possible_keys[i]);
    }
    printf("\n");
    
    printf("Should be %X with %i right guess\n", temp, possible_keys[temp]);
}

uint8_t** append(uint8_t** old_list, int len, uint8_t* value) {
    uint8_t** list;
    if (len == 0) {
        list = malloc(sizeof(uint8_t*));
        list[0] = value;
    } else {
        list = malloc(sizeof(uint8_t*) * (len + 1));
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
    uint8_t* clone = malloc(sizeof(uint8_t) * 16);
    for (int i = 0; i < 16; i++)
    {
        clone[i] = x[i];
    }
    return clone;
}

uint8_t** valid_plains;
int count = 0;
int attack(uint8_t starting_diff, int input_sbox, uint8_t second_diff, int second_sbox, uint8_t ending_diff, int end_sbox) {
    for (int i = 0; i < PLAIN_NUMBER; i++) {
        uint8_t* p1 = calloc(16, sizeof(uint8_t));
        uint8_t* c1 = calloc(16, sizeof(uint8_t));
        uint8_t* c2 = calloc(16, sizeof(uint8_t));
        get_random_plain_text(p1);
        encrypt(p1, c1);
        p1[input_sbox] ^= starting_diff;
        encrypt(p1, c2);
        do_inverse_round(c1);
        do_inverse_round(c2);
        if ((c1[end_sbox] ^ c2[end_sbox]) == ending_diff) {
            valid_plains = append(valid_plains, count, p1);
            count++;
        }
        free(c1);
        free(c2);
    }
    for (int i = 0; i < count; i++) {
        for (uint8_t k = 0; k < 16; k++)
        {
            uint8_t* p1 = valid_plains[i];
            uint8_t* p2 = clone(valid_plains[i]);
            p2[input_sbox] ^= starting_diff;
            p1[input_sbox] ^= k;
            p2[input_sbox] ^= k;
            SC(p1);
            SC(p2);
            uint8_t* dif = xor_values_obtaining_new(p1, p2);
            if (dif[second_sbox] == second_diff) {
                possible_keys[k]++;
            }
            free(p2);
        }
    }
    possible_keys_print();
}

void cipher_test() {
    uint8_t* bits = malloc(sizeof(uint8_t) * 16);
    convert_bits_to_array(bits, 0x19dca6fb3e470285);
    uint8_t* c = malloc(sizeof(uint8_t) * 16);
    uint8_t* p = malloc(sizeof(uint8_t) * 16);
    encrypt(bits, c);
    print_state(bits);
    print_state(c);
    decrypt(c, p);
    print_state(p);
    /*
    convert_bits_to_array(bits, 0x76931fac9dab2b3a);
    encrypt(bits, c);
    print_state(bits);
    print_state(c);
    decrypt(c, p);
    print_state(p);*/
}

int main() {
    int initial_sbox_active = 15;
    int ending_sbox_active = 11;
    print_key_scheduling();
    attack(0xc, initial_sbox_active, 8, initial_sbox_active, 1, ending_sbox_active);
    //cipher_test();
}