#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int line1[4] = {1,0,1,0};
unsigned int line2[4] = {0,1,1,0};
unsigned int line3[4] = {1,0,0,0};
unsigned int line4[4] = {0,0,1,1};

unsigned int* fixed_matrix[4] = {line1, line2, line3, line4};

void print_matrix(unsigned int**);
unsigned int** MC(unsigned int**);
unsigned int** SR(unsigned int**);
unsigned int** SC(unsigned int**);
unsigned int** c_round(unsigned int**);
unsigned int** cipher(int, unsigned int**);
double probability_SBox(unsigned int**);
double permute_alpha_probability(unsigned int);
void all_matrix_permutation(int, int);
unsigned int** get_input_matrix();

#define ROUNDS 4
#define MATRIX_DIMENSION 4

double p = 1;
FILE* fh;

int main(int argc, char* argv[]) {
    fh = fopen("result.txt", "w");
    fprintf(fh, "\n");
    fclose(fh);

    fh = fopen("result.txt", "a");

    unsigned int** m = get_input_matrix();

    cipher(ROUNDS, m);
    fprintf(fh, "Total probability = %.12f\n", p);
    fclose(fh);
    //all_matrix_permutation(4, 6);
}

unsigned int matrix[4][4] = {
    {0,0,0,0},
    {0,0xc,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

unsigned int** get_input_matrix() {
    unsigned int** m = calloc(1, sizeof(unsigned int*) * MATRIX_DIMENSION);
    for (int i = 0; i < 4; i++) {
        m[i] = calloc(1, sizeof(unsigned int) * MATRIX_DIMENSION);
    }
    
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        for (int j = 0; j < MATRIX_DIMENSION; j++) {
            m[i][j] = matrix[i][j];
        }
    }
    return m;
}

// Val is the 64 bit value
void all_matrix_permutation(int d, int rounds) {
    unsigned long val = 0xc;
    FILE* fh = fopen("probs.txt", "w");
    fprintf(fh, "\n");
    fclose(fh);
    fh = fopen("probs.txt", "a");
    for (int i = 0; i < 16; i++) {
        unsigned int input_line1[4] = {(val & 0xF000000000000000) >> 60,(val & 0x0000F00000000000) >> 44,(val & 0x00000000F0000000) >> 28,(val & 0x000000000000F000) >> 12};
        unsigned int input_line2[4] = {(val & 0x0F00000000000000) >> 56,(val & 0x00000F0000000000) >> 40,(val & 0x000000000F000000) >> 24,(val & 0x0000000000000F00) >> 8};
        unsigned int input_line3[4] = {(val & 0x00F0000000000000) >> 52,(val & 0x000000F000000000) >> 36,(val & 0x0000000000F00000) >> 20,(val & 0x00000000000000F0) >> 4};
        unsigned int input_line4[4] = {(val & 0x000F000000000000) >> 48,(val & 0x0000000F00000000) >> 32,(val & 0x00000000000F0000) >> 16,(val & 0x000000000000000F)};
        unsigned int* input_matrix[4] = {input_line1, input_line2, input_line3, input_line4};
        //unsigned int input_line1[4] = {(val & 0xF000) >> 12,(val & 0x00F0) >> 4};
        //unsigned int input_line2[4] = {(val & 0x0F00) >> 8,(val & 0x000F)};
        //unsigned int* input_matrix[4] = {input_line1, input_line2};
        //print_matrix(input_matrix, d);
        //printf("\n");
        cipher(rounds, input_matrix);
        val = val << 4;
        fprintf(fh, "Total probability = %.12f\n", p);
        p = 1;
    }
    fclose(fh);
    
}

unsigned int** cipher(int rounds, unsigned int** m) {
    fprintf(fh, "Starting with matrix:\n");
    print_matrix(m);
    printf("\nStart..\n");
    for (int i = 0; i < rounds; i++) {
        m = c_round(m);
    }
}

unsigned int** c_round(unsigned int* m[]) {
    double probability = probability_SBox(m);
    p = probability * p;

    m = SC(m);
    fprintf(fh, "\nSC\n");
    print_matrix(m);
    fprintf(fh, "\nSR\n");
    m = SR(m);
    print_matrix(m);
    fprintf(fh, "\nMC\n");
    m = MC(m);
    print_matrix(m);
    fprintf(fh, "Round Ended\n");
    fprintf(fh, "Round probability = %f\n\n", probability);
    return m;
}

unsigned int** MC(unsigned int* input_matrix[]) {
    unsigned int** result = malloc(sizeof(unsigned int*) * 4);
    for (int i = 0; i < 4; i++)
    {
        result[i] = malloc(sizeof(unsigned int) * 4);
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result[i][j] = result[i][j] ^ (fixed_matrix[i][k] * input_matrix[k][j]);
            }
        }
    }
    return result;

}

unsigned int** SR(unsigned int* input_matrix[]) {
    unsigned int temp = input_matrix[1][0];
    input_matrix[1][0] = input_matrix[1][1];
    input_matrix[1][1] = input_matrix[1][2];
    input_matrix[1][2] = input_matrix[1][3];
    input_matrix[1][3] = temp;
    temp = input_matrix[2][0];
    unsigned int temp2 = input_matrix[2][1];
    input_matrix[2][0] = input_matrix[2][2];
    input_matrix[2][1] = input_matrix[2][3];
    input_matrix[2][2] = temp;
    input_matrix[2][3] = temp2;
    temp = input_matrix[3][0];
    temp2 = input_matrix[3][1];
    unsigned int temp3 = input_matrix[3][2];
    input_matrix[3][0] = input_matrix[3][3];
    input_matrix[3][1] = temp;
    input_matrix[3][2] = temp2;
    input_matrix[3][3] = temp3;

    return input_matrix;
}

void print_matrix(unsigned int* matrix[]) {
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        for (int j = 0; j < MATRIX_DIMENSION; j++) {
            fprintf(fh, "%X ", matrix[i][j]);
        }
        fprintf(fh, "\n");
    }
}

double probability_SBox(unsigned int* matrix[]) {
    double probability = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            probability = permute_alpha_probability(matrix[i][j]) * probability;
        }
    }
    return probability;
}

unsigned int permute_alpha_beta(unsigned int x) {
                        //[0,1,2,3,4,5,6,7,8,9,A,B,C, D ,E,F]
    int permutation[16] = {0,5,4,1,3,6,7,2,3,4,5,2,8,0xD,1,6};
    return permutation[x];
}
double permute_alpha_probability(unsigned int x) {
                            //[0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F]
    double probability[16] = {16,6,4,6,4,4,4,4,4,4,2,2,6,4,4,4};
    return probability[x] / 16;
}

unsigned int** SC(unsigned int* input_matrix[]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
                input_matrix[i][j] = permute_alpha_beta(input_matrix[i][j]);
        }
    }

    return input_matrix;
}