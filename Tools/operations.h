#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void print_uint_matrix(unsigned int* matrix[], int rows, int cols) {
    printf("   ");
    for (int i = 0; i < cols; i++) {
        printf("%X  ", i);
    }
    printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("%X ", i);
        for (int j = 0; j < cols; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_uint8_matrix(uint8_t** matrix, int rows, int cols) {
    printf("   ");
    for (int i = 0; i < cols; i++) {
        printf("%X  ", i);
    }
    printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("%X ", i);
        for (int j = 0; j < cols; j++) {
            printf("%2X ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_int_matrix(int* matrix[], int rows, int cols) {
    printf("   ");
    for (int i = 0; i < cols; i++) {
        printf("%X  ", i);
    }
    printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("%X ", i);
        for (int j = 0; j < cols; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void fprint_uint_matrix(char* filename, unsigned int* matrix[], int rows, int cols) {
    FILE* fh = fopen(filename, "w");
    fprintf(fh, "   ");
    for (int i = 0; i < cols; i++) {
        fprintf(fh, "%X  ", i);
    }
    fprintf(fh, "\n");
    for (int i = 0; i < rows; i++) {
        fprintf(fh, "%X ", i);
        for (int j = 0; j < cols; j++) {
            fprintf(fh, "%2d ", matrix[i][j]);
        }
        fprintf(fh, "\n");
    }
    fclose(fh);
}

void fprint_int_matrix(char* filename, int* matrix[], int rows, int cols) {
    FILE* fh = fopen(filename, "w");
    fprintf(fh, "   ");
    for (int i = 0; i < cols; i++) {
        fprintf(fh, "%X  ", i);
    }
    fprintf(fh, "\n");
    for (int i = 0; i < rows; i++) {
        fprintf(fh, "%X ", i);
        for (int j = 0; j < cols; j++) {
            fprintf(fh, "%2d ", matrix[i][j]);
        }
        fprintf(fh, "\n");
    }
    fclose(fh);
}

unsigned int addition(unsigned int x1, unsigned int x2) {
    unsigned int result = x1 ^ x2;
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

uint64_t* append_uint64(uint64_t* array, int* len, unsigned int value) {
    int l = *len;
    *len = *len + 1;
    uint64_t* new_array = malloc(sizeof(uint64_t) * (l + 1));
    for (int i = 0; i < l; i++) {
        new_array[i] = array[i];
    }
    new_array[l] = value;
    free(array);
    return new_array;
}

uint8_t** instantiate_matrix(uint8_t** matrix, int rows, int cols) {
    matrix = malloc(sizeof(uint8_t*) * rows);
    for (int i = 0; i < rows; i++) {
        matrix[i] = calloc(1, sizeof(uint8_t) * cols);
    }
    return matrix;
}

uint8_t shift_rotate8_right(uint8_t x, int rotation) {
    return (x >> rotation) | (x << (8 - rotation));
}

uint64_t shift_rotate8_left(uint64_t x, int rotation) {
    return (x << rotation) | (x >> (8 - rotation));
}

uint64_t shift_rotate64_right(uint64_t x, int rotation) {
    return (x >> rotation) | (x << (64 - rotation));
}

uint64_t shift_rotate64_left(uint64_t x, int rotation) {
    return (x << rotation) | (x >> (64 - rotation));
}