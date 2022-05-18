#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int addition(unsigned int, unsigned int);
unsigned int multiplication(unsigned int, unsigned int);
unsigned int reduction(unsigned int);
/**
 * @brief Converts the unsigned number into a string representing the binary number
 * 
 * @param x The number to convert
 * @param len The length of the string
 * @return char* The string representing the binary number
 */
char* print_bit_string(unsigned int, int);

int main(int argc, char** argv) {
    if (argc <= 4) {
        char operation = argv[1][0];
        unsigned int x1 = strtol(argv[2], NULL, 2);
        unsigned int x2;
        if (argc == 4) {
            x2 = strtol(argv[3], NULL, 2);
        }
        switch (operation) {
            case '+':
                addition(x1, x2);
                break;
            case 'x':
                reduction(multiplication(x1, x2));
                break;
            case 'r':
                printf("%s\n", print_bit_string(reduction(x1), 4));
                break;
            default:
                printf("Error in the operation input\n");
                break;
        }
    }
    return 0;
}

unsigned int addition(unsigned int x1, unsigned int x2) {
    unsigned int result = x1 ^ x2;
    printf("%s + %s = %s\n", print_bit_string(x1, 4), print_bit_string(x2, 4), print_bit_string(result, 4));
    return result;
}

unsigned int multiplication(unsigned int x1, unsigned int x2) {
    unsigned int result = 0;
    int count = 3;
    for (unsigned i = 1 << 3; i > 0; i = i >> 1) {
        if (x2 & i) {
            result = result ^ (x1 << count);
        }
        count--;
    }
    printf("%s x %s = %s\n", print_bit_string(x1, 4), print_bit_string(x2, 4), print_bit_string(reduction(result), 4));
    return result;
}

int most_significant_bit_position(unsigned int x) {
    unsigned int temp = x;
    int msb_pos = 0;
    while (temp != 0)
    {
        temp = temp / 2;
        msb_pos++;
    }
    return msb_pos;
}

unsigned int reduction(unsigned int x) {
    unsigned int irreducible_polynomial = 0b10011;
    int msb = most_significant_bit_position(x);
    if (msb < 5) { // No need of reduction
        return x;
    } else {
        unsigned int shifted_ip = irreducible_polynomial << (msb - 5);
        return reduction(x ^ shifted_ip);
    }
} 

char* print_bit_string(unsigned x, int len) {
    char* bit_string = malloc(sizeof(char) * len);
    int count = 0;
    for (unsigned i = 1 << (len - 1); i > 0; i = i >> 1) {
        bit_string[count] = (x & i) ? '1' : '0';
        count++;
    }
    return bit_string;
}