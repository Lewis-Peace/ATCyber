#include <stdio.h>
#include <stdlib.h>
#include "es3.h"

unsigned int** matrix_multiplication(unsigned int**);
/**
 * @brief Receives from the stdin row by row the matrix
 * 
 * @return unsigned** The input matrix
 */
unsigned int** input_matrix();

/*
    1  0  2  3
    1  0  0  0
    0  1  2  0
    3  0  1  0
*/
unsigned int** static_matrix();
/**
 * @brief Prints the matrix
 * @param matrix The matrix to print
 * @param len The length of the matrix
 */
void print_matrix(unsigned int**, int);

int main() {
    unsigned int** state_matrix = input_matrix();
    unsigned int** result_matrix = matrix_multiplication(state_matrix);
    printf("\n\nResult\n");
    print_matrix(result_matrix, 4);
    
    for (int i = 0; i < 4; i++) {
        free(result_matrix[i]);
    }
    free(result_matrix);
}

unsigned int** matrix_multiplication(unsigned int** A) {
    unsigned int** result = malloc(sizeof(unsigned int*) * 4);
    for (int i = 0; i < 4; i++)
    {
        result[i] = malloc(sizeof(unsigned int) * 4);
    }

    unsigned int** fixed_matrix = static_matrix();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result[i][j] = result[i][j] ^ multiplication(fixed_matrix[i][k], A[k][j]);
                // If i misunderstood the factors order just use this one instead of the other one
                //result[i][j] = result[i][j] ^ multiplication(A[i][k], fixed_matrix[k][j]);
            }
            result[i][j] = reduction(result[i][j]);
        }
    }

    for (int i = 0; i < 4; i++) {
        free(fixed_matrix[i]);
    }
    free(fixed_matrix);
    for (int i = 0; i < 4; i++) {
        free(A[i]);
    }
    free(A);
    
    return result;
}

unsigned int** input_matrix() {
    printf("Input the matrix row by row\n");
    unsigned int a = 0, b = 0, c = 0, d = 0;
    unsigned int** input = malloc(sizeof(unsigned int*) * 4);
    for (int i = 0; i < 4; i++)
    {
        input[i] = calloc(1, sizeof(unsigned int) * 4);
    }
    for (int i = 0; i < 4; i++) {
        scanf("%X %X %X %X", &a, &b, &c, &d);
        input[i][0] = a;
        input[i][1] = b;
        input[i][2] = c;
        input[i][3] = d;
    }
    return input;
}

void print_matrix(unsigned int** matrix, int len) {
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            printf("%X ", matrix[i][j]);
        }
        printf("\n");
    }
    
}


unsigned int** static_matrix() {
    unsigned int** matrix = malloc(sizeof(unsigned int*) * 4);
    for (int i = 0; i < 4; i++)
    {
        matrix[i] = malloc(sizeof(unsigned int) * 4);
    }
    matrix[0][0] = 1;
    matrix[0][1] = 0;
    matrix[0][2] = 2;
    matrix[0][3] = 3;
    matrix[1][0] = 1;
    matrix[1][1] = 0;
    matrix[1][2] = 0;
    matrix[1][3] = 0;
    matrix[2][0] = 0;
    matrix[2][1] = 1;
    matrix[2][2] = 2;
    matrix[2][3] = 0;
    matrix[3][0] = 3;
    matrix[3][1] = 0;
    matrix[3][2] = 1;
    matrix[3][3] = 0;
    return matrix;
}