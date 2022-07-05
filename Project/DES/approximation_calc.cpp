#include <stdio.h>
#include <math.h>
#include <vector>

int DesExpansion[48] = {
    32,  1,  2,  3,  4,  5,  4,  5,
     6,  7,  8,  9,  8,  9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32,  1
}; 

const int Pbox[32] = {
    16,  25, 12, 11, 3, 20, 4, 15,
     31, 17, 9, 6,  27, 14, 1, 22,
     30,  24, 8, 18, 0, 5,  29,  23,
    13, 19, 2,  26, 10, 21,  28, 7
};

using std::vector;

int index_after_expansion(int index) {
    int cont = 0;
    for (int i = 0; i < 48; i++) {
        if (DesExpansion[47 - i] == index) {
            return cont;
        }
        cont++;
    }
    return -1;
}

int index_after_permutation(int index) {
    int cont = 0;
    for (int i = 0; i < 32; i++) {
        if (Pbox[31 - i] == index) {
            return cont;
        }
        cont++;
    }
    return -1;
}

vector<int> get_PH(int ind) {
    switch ((int) floor(ind / 6))
    {
    case 0:
        return {0,1,2,3};
    case 1:
        return {4,5,6,7};
    case 2:
        return {8,9,10,11};
    case 3:
        return {12,13,14,15};
    case 4:
        return {16,17,18,19};
    case 5:
        return {20,21,22,23};
    case 6:
        return {24,25,26,27};
    case 7:
        return {28,29,30,31};
    default:
        return {};
    }
}

void invert() {
    static int PC1[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,

    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};
    static char PC2[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};
    
    bool found = false;  
    for (int i = 0; i < 56; i++) {
        PC1[i] -= 1;
    }
    printf("PC1\n");
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 56; j++) {
            if (i == PC1[j]) {
                printf("%i, ", j);
                found = true;
            }
        }
        if (found == false) {
            printf("%i, ", -1);
        }
        found = false;
        (i + 1) % 8 == 0 ? printf("\n") : 0;
    }
    printf("\n");
  
    for (int i = 0; i < 56; i++) {
        PC2[i] -= 1;
    }
    printf("PC2\n");
    for (int i = 0; i < 56; i++) {
        for (int j = 0; j < 48; j++) {
            if (i == PC2[j]) {
                printf("%i, ", j);
                found = true;
            }
        }
        if (found == false) {
            printf("%i, ", -1);
        }
        found = false;
        (i + 1) % 8 == 0 ? printf("\n") : 0;
    }
    printf("\n");
    
}

int main() {
    for (int i = 0; i < 48; i++) {
        DesExpansion[i] = 32 - DesExpansion[i]; 
    }

    //vector<int> PL = {3, 7, 11, 19, 23, 27, 31};
    //for (int j = 0; j < PL.size(); j++)
    //{
    //    printf("PL[%i]\n", PL[j]);
    //    int ind = index_after_expansion(PL[j]);
    //    printf("K[%i]\n", ind);
    //    vector<int> PH = get_PH(ind);
    //        printf("PH[");
    //    for (int i = 0; i < PH.size(); i++) {
    //        ind = index_after_permutation(PH[i]);
    //        printf("%i ", ind);
    //    }
    //    printf("]\n\n");
    //}
    
    invert();
    
    return 0;
}