#include <stdio.h>

int getMostSignificantBitPosition(__uint8_t x) {
    int r = 0;
    while (x >>=1) {
        r++;
    }
    return r;
}

unsigned shiftNTimes(unsigned x, int numberOfShifts, char typeOfShift) {
    if (typeOfShift == 'l') {
        while (numberOfShifts != 0) {
            x = x << 1;
            numberOfShifts--;
        }
        return x;
    } else if (typeOfShift == 'r') {
        while (numberOfShifts != 0) {
            x = x >> 1;
            numberOfShifts--;
        }
        return x;
    }
}


int main(int argc, char* argv) {
    __uint8_t x = 0b111;
    __uint8_t y = 0b10;
    int xLen = getMostSignificantBitPosition(x);
    int yLen = getMostSignificantBitPosition(y);
    unsigned result = shiftNTimes(x, yLen--, 'l');
    /*while (yLen != 0) {
        unsigned secondOperator;
        
    }*/
    printf("Hello world %i\n", (&y)[1]);
}
