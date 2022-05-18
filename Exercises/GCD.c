#include <stdio.h>
#include <stdlib.h>

int homemadeGCD(int, int);
int modInverse(int, int);

// Driver Code
int main(int argc, char** argv)
{
    int a = atoi(argv[1]);
    int m = atoi(argv[2]);
 
    homemadeGCD(a, m);
    // Function call
    printf("Modular multiplicative inverse is %d\n",
           modInverse(a, m));
    return 0;
}

int homemadeGCD(int a, int m) {
    printf("a = %i, m = %i\n", a, m);
    while (m != 0) {
        printf("%i mod %i = %i\n", a, m, a % m);
        int temp = m;
        m = a % m; // a3
        a = temp;
    }
}


int modInverse(int a, int m)
{
    int m0 = m;
    int y = 0, x = 1;
 
    if (m == 1)
        return 0;
 
    while (a > 1) {
        // q is quotient
        int q = a / m;
        int t = m;
 
        printf("a = %i, m = %i\n", a, m);
        // m is remainder now, process same as
        // Euclid's algo
        m = a % m, a = t;
        t = y;
 
        // Update y and x
        y = x - q * y;
        x = t;
        printf("x = %i, y = %i\n", x, y);
    }
 
    // Make x positive
    if (x < 0)
        printf("Was neg\n");
        x += m0;
 
    return x;
}