#include <stdio.h>
#include <stdlib.h>

unsigned int permutation(unsigned int);
void differential_p(int, int**);

void populate_matrix(int**, int, int);
void print_matrix(int**, int);

int main(int argc, char** argv) {
    int** matrix = malloc(sizeof(int*) * 16);
    for (int i = 0; i < 16; i++) {
        matrix[i] = calloc(1, sizeof(int) * 16);
    }

    for (int i = 0; i < 16; i++) {
        differential_p(i, matrix);
    }
    
    print_matrix(matrix, 16);

    int alpha = -1, beta = -1;
    printf("Want a specific probability by given alpha and beta?\n");
    printf("Insert alpha: ");
    scanf("%i", &alpha);
    printf("Insert beta: ");
    scanf("%i", &beta);
    if (alpha != -1 && beta != -1) {
        printf("Probability of alpha = %i and beta = %i is\n", alpha, beta);
        double p = matrix[alpha][beta] / (double) 8;
        printf("%.2f or %i/%i\n", p, matrix[alpha][beta], 8);
    }

    for (int i = 0; i < 16; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}

void differential_p(int alpha, int** matrix) {
    //printf("alpha = %i\n", alpha);
    for (unsigned x = 0; x < 16; x++)
    {
        int b = permutation(x ^ alpha) ^ permutation(x);
        matrix[alpha][b]++;
        //printf("beta = %i for x = %i\n", b, x);
    }
    
}

void populate_matrix(int** matrix, int len, int x) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            matrix[i][j] = x;
        }
    }
}

void print_matrix(int* matrix[], int len) {
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F BETA\n");
    for (int i = 0; i < len; i++) {
        printf("%X ", i);
        for (int j = 0; j < len; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("ALPHA\n");
}

unsigned int permutation(unsigned int x) {
    unsigned int sBox[16] = {0xc,6,9,0,1,2,0xa,0xb,3,8,5,0xd,4,0xe,7,0xf};
    return sBox[x];
}
unsigned int permutation_assgnment_2(unsigned int x) {
    switch (x)
    {
    case 0:
        return 0xa;
    case 1:
        return 5;
    case 2:
        return 4;
    case 3:
        return 2;
    case 4:
        return 6;
    case 5:
        return 1;
    case 6:
        return 0xf;
    case 7:
        return 3;
    case 8:
        return 0xb;
    case 9:
        return 0xe;
    case 0xa:
        return 7;
    case 0xb:
        return 0;
    case 0xc:
        return 8;
    case 0xd:
        return 0xd;
    case 0xe:
        return 0xc;
    case 0xf:
        return 9;
    
    default:
        break;
    }
}