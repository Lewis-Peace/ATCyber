#include <stdio.h>
#include <stdlib.h>

typedef struct tuples {
    unsigned int input: 4;
    unsigned int output: 4;
} tuples;

unsigned int reverse_permutation(unsigned int);
unsigned int permutation(unsigned int);
void guess_key2(unsigned int, unsigned int, unsigned int, unsigned int*);
void guess_key1(unsigned int, unsigned int, unsigned int, unsigned int*);

void fill_with_zeros(unsigned int*, int);
void print_results(unsigned int*, unsigned int*);

unsigned int k1_list[16];
unsigned int k2_list[16];

int main() {
    
    // Preparing the counter arrays

    fill_with_zeros(k1_list, 16);
    fill_with_zeros(k2_list, 16);

    // Setting up the input data

    tuples tuple1 = {
        .input = 0b0110,
        .output = 0b1000
    };
    tuples tuple2 = {
        .input = 0b0000,
        .output = 0b1100
    };
    tuples tuple3 = {
        .input = 0b1101,
        .output = 0b1011
    };

    // Computing k2 viable keys

    unsigned int u12 = tuple1.input ^ tuple2.input;
    unsigned int u23 = tuple2.input ^ tuple3.input;
    unsigned int u31 = tuple3.input ^ tuple1.input;
    
    guess_key2(u12, tuple1.output, tuple2.output, k2_list);
    guess_key2(u23, tuple2.output, tuple3.output, k2_list);
    guess_key2(u31, tuple3.output, tuple1.output, k2_list);

    // Computing k1 viable keys

    unsigned int v12 = tuple1.output ^ tuple2.output;
    unsigned int v23 = tuple2.output ^ tuple3.output;
    unsigned int v31 = tuple3.output ^ tuple1.output;

    guess_key1(v12, tuple1.input, tuple2.input, k1_list);
    guess_key1(v23, tuple2.input, tuple3.input, k1_list);
    guess_key1(v31, tuple3.input, tuple1.input, k1_list);

    // Printing results

    //printf("We know for sure that:\n\tU1 xor U2 = %u\n\tU2 xor U3 = %u\n\tU3 xor U1 = %u\n", u12, u23, u31);
    //printf("We know for sure that:\n\tV1 xor V2 = %u\n\tV2 xor V3 = %u\n\tV3 xor V1 = %u\n", v12, v23, v31);
    print_results(k1_list, k2_list);
}

/**
 * @brief Guesses k2 and checks if this returns a value legit for the umn
 * 
 * @param umn result of um xor un
 * @param ym output of tuple m
 * @param yn output of tuple n
 * @param keys_list The array of counters to update every time a key guess is viable
 */
void guess_key2(unsigned int umn, unsigned int ym, unsigned int yn, unsigned int* keys_list) {
    for (unsigned int k2 = 0; k2 < 16; k2++) {
        unsigned int vm = k2 ^ ym;
        unsigned int vn = k2 ^ yn;
        unsigned int possible_um = reverse_permutation(vm);
        unsigned int possible_un = reverse_permutation(vn);
        //printf("%u -> %u, %u -> %u\n", v1, possible_u1, v2, possible_u2);
        unsigned int check = possible_um ^ possible_un;
        if (check == umn) {
            //printf("\tKey %u is viable\n", k2);
            keys_list[k2]++;
        }
    }
}

/**
 * @brief Guesses k1 and checks if this returns a value legit for the vmn
 * 
 * @param vmn result of vm xor vn
 * @param xm input of tuple m
 * @param xn input of tuple n
 * @param keys_list The array of counters to update every time a key guess is viable
 */
void guess_key1(unsigned int vmn, unsigned int xm, unsigned int xn, unsigned int* keys_list) {
    for (unsigned int k1 = 0; k1 < 16; k1++) {
        unsigned int um = k1 ^ xm;
        unsigned int un = k1 ^ xn;
        unsigned int possible_vm = permutation(um);
        unsigned int possible_vn = permutation(un);
        //printf("%u -> %u, %u -> %u\n", um, possible_vm, un, possible_vn);
        unsigned int check = possible_vm ^ possible_vn;
        if (check == vmn) {
            //printf("\tKey %u is viable\n", k1);
            keys_list[k1]++;
        }
    }
}

/**
 * @brief Prints the result in a way that is visible the counters of each key and the result
 * 
 * @param k1_list The counters of viable keys for each value of k1
 * @param k2_list The counters of viable keys for each value of k2
 */
void print_results(unsigned int* k1_list, unsigned int* k2_list) {
    printf("Val 0 1 2 3 4 5 6 7 8 9 a b c d e f\n");
    printf("K1  ");
    for (int i = 0; i < 16; i++)
    {
        printf("%u ", k1_list[i]);
    }
    printf("\n");
    printf("K2  ");
    for (int i = 0; i < 16; i++)
    {
        printf("%u ", k2_list[i]);
    }
    printf("\n");
    
}

/**
 * @brief Fills the array with zeros
 * 
 * @param array The array to fill with zeros
 * @param l The array length
 */
void fill_with_zeros(unsigned int* array, int l) {
    for (int i = 0; i < l; i++)
    {
        array[i] = 0;
    }
}

unsigned int reverse_permutation(unsigned int v) {
    switch (v)
    {
    case 6:
        return 0;
    case 4:
        return 1;
    case 12:
        return 2;
    case 5:
        return 3;
    case 0:
        return 4;
    case 7:
        return 5;
    case 2:
        return 6;
    case 14:
        return 7;
    case 1:
        return 8;
    case 15:
        return 9;
    case 3:
        return 10;
    case 13:
        return 11;
    case 8:
        return 12;
    case 10:
        return 13;
    case 9:
        return 14;
    case 11:
        return 15;
    
    default:
        return 100;
    }
}

unsigned int permutation(unsigned int v) {
    switch (v)
    {
    case 0:
        return 6;
    case 1:
        return 4;
    case 2:
        return 12;
    case 3:
        return 5;
    case 4:
        return 0;
    case 5:
        return 7;
    case 6:
        return 2;
    case 7:
        return 14;
    case 8:
        return 1;
    case 9:
        return 15;
    case 10:
        return 3;
    case 11:
        return 13;
    case 12:
        return 8;
    case 13:
        return 10;
    case 14:
        return 9;
    case 15:
        return 11;
    
    default:
        return 100;
    }
}