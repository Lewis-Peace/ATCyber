#include <stdio.h>
#include "../Assignment 3/Code/TC01.h"

uint8_t* possible_sbox_inputs(uint8_t* list, int* list_len, uint8_t input_difference, uint8_t output_difference);
uint64_t get_state_before_ARK(uint64_t cipher_text);
void possible_round_keys(int* list, uint8_t input_difference, uint8_t sbox_output, uint8_t output_difference);

int main() {/*
    uint64_t valid_plain_text;
    uint64_t starting_number = 0xC0000000000;
    for (uint64_t plain_text = starting_number; plain_text < starting_number + 0xFFF00000000; plain_text = plain_text + 0x100000000) {
        uint64_t plain_text_2 = plain_text ^ 0xC0000000000;
        uint64_t cipher_text_1 = cipher(plain_text);
        uint64_t cipher_text_2 = cipher(plain_text_2);
        if ((cipher_text_2 ^ cipher_text_1) == 0x50000000000) {
            valid_plain_text = plain_text;
            printf("%lx -> %lx\n%lx -> %lx\n", plain_text, cipher_text_1, plain_text_2, cipher_text_2);
            printf("Output diff = %lx\n", cipher_text_1 ^ cipher_text_2);
            printf("State before first ARK %lx\n", get_state_before_ARK(cipher_text_1));
            printf("State before first ARK %lx\n", get_state_before_ARK(cipher_text_2));
            break;
            printf("Hello\n");
        }
    }*/
/*
    uint64_t plain_text_1 = 0x120000000000;
    uint64_t cipher_text_1 = cipher(plain_text_1);
    uint64_t plain_text_2 = plain_text_1 ^ 0xC0000000000;
    uint64_t cipher_text_2 = cipher(plain_text_2);
    printf("%lx -> %lx\n%lx -> %lx\n", plain_text_1, cipher_text_1, plain_text_2, cipher_text_2);
    printf("Output diff = %lx\n", cipher_text_1 ^ cipher_text_2);
    printf("State before first ARK %lx\n", get_state_before_ARK(cipher_text_1));
    printf("State before first ARK %lx\n", get_state_before_ARK(cipher_text_2));
*/
    FILE* fh = fopen("plain_cipher_groups.txt", "w");
    fprintf(fh, "\n");
    fclose(fh);
    fh = fopen("plain_cipher_groups.txt", "a");
    int count = 0;
    int* possible_values = calloc(1, sizeof(int) * 16);
    for (uint64_t plain_text = 0x120000000000; plain_text < 0x120000000320; plain_text++) {
        uint64_t plain_text_2 = plain_text ^ 0xC0000000000;
        uint64_t cipher_text_1 = cipher(plain_text);
        uint64_t cipher_text_2 = cipher(plain_text_2);
        if ((cipher_text_2 ^ cipher_text_1) == 0x50000000000) {
            count++;
            uint64_t state_before_ARK = get_state_before_ARK(cipher_text_1);
            fprintf(fh, "plain_text_1 %lx\tplain_text_2 %lx\ncipher_text_1 %lx\tcipher_text_2 %lx\n", plain_text, plain_text_2, cipher_text_1, cipher_text_2);
            uint8_t active_sbox_output = (state_before_ARK >> 6 * 4) & 0xf;
            fprintf(fh, "State before ARK %lx, active sbox output %x\n\n", state_before_ARK, active_sbox_output);
            possible_round_keys(possible_values, 0x1, active_sbox_output, 0x5);
        }
    }
    fclose(fh);
    printf("Found %i couples\n", count);
    //possible_round_keys(possible_values, 0x1, 0x4, 0x5);
    //possible_round_keys(possible_values, 0x3, 0x8, 0x1);
    //possible_round_keys(possible_values, 0x8, 0xc, 0x3);
    for (int i = 0; i < 16; i++)
    {
        printf("%x ", possible_values[i]);
    }
    printf("\n");

    for (int i = 0; i < 4; i++)
    {
        printf("Round %i key = %lx\n", i, key_scheduling(0x101, i));
    }
    
    
}

void possible_round_keys(int* list, uint8_t input_difference, uint8_t sbox_output, uint8_t output_difference) {
    uint8_t* possible_inputs;
    int len = 0;
    possible_inputs = possible_sbox_inputs(possible_inputs, &len, input_difference, output_difference);
    
    for (int i = 0; i < len; i++) {
        uint8_t value_before_ARK = possible_inputs[i];
        uint8_t a = value_before_ARK;
        uint8_t b = value_before_ARK ^ input_difference;
        uint8_t x = sbox_output;
        uint8_t y = sbox_output ^ output_difference;
        uint8_t key_test_1 = a ^ reverse_sbox[x];
        uint8_t key_test_2 = b ^ reverse_sbox[y];
        if (key_test_1 == key_test_2) {
            //printf("a = %x b = %x  x = %x y = %x\n",a,b,x,y);
            //printf("k1 = %x  k2 = %x\n", key_test_1, key_test_2);
            list[key_test_1]++;
        }
    }
}

uint64_t get_state_before_ARK(uint64_t cipher_text) {
    uint8_t** matrix = reverse_MC(cipher_text, matrix);
    uint64_t round_data = reverse_SR(matrix);
    round_data = reverse_SC(round_data);
    return round_data;
}

uint8_t* possible_sbox_inputs(uint8_t* possible_inputs, int* list_len, uint8_t input_difference, uint8_t output_difference) {
    int len = *list_len;
    for (int i = 0; i < 16; i++) {
        if ((sbox[i] ^ sbox[i ^ input_difference]) == output_difference) {
            possible_inputs = append_uint8(possible_inputs, len, (uint8_t) i);
            len++;
        }
    }
    *list_len = len;
    return possible_inputs;
}