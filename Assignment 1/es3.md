# Main ideas:

## Addition

Concerning the addition the idea is simply the XOR of the two binary vaules. Thats because in the case of 2 x added together the modulo 2 will remove that value so if we have $x^2+x+1$ and $x^2+1$ the sum of those 2 will be $2x^2+x+2 \equiv x \mod 2$.

## Multiplication

Concerning the multiplication the operation will be a little bit more complex. The main idea is by keeping track of each 1 in the left operator and using those ones to shift the first operator equal times as the position of each 1. The different values obtained by the different shift will then be added together to represent the result (in the code i shift and add until the last bit cause was more compact instead of keeping in the memory each value of the shifts). An example is with the $(x^3+1)*(x^2+1)$ in binary 1001 0101. i will shift 1001 2 times because there are 2 ones in the second operator. so i will shift it by 2 (0100) and 0 cause (0001). So 100100 + 1001 = 1101.
Considering $(x^3+1)*(x^2+1)= x^5+x^3+x^2+1$ and considering the GF($2^4$) the $x^5$ will be removed keeping $x^3+x^2+1$ or 1101.

## Reduction

After the multiplication there always be a reduction that checks if the value has surpassed the irreducible polynomial and if so will apply the shift and xor operations to get his equivalent by modulo. As an example $(x^3+x^2+x)(x^2+x+1) = x^5+x^3+x$ and $x^5+x^3+x \div x^4+x+1 = x$ with a remainder of $x^3+x^2$. That remainder is the result of the operation and can be coded as 1100.

# How does the program work

To make the program work firstly has to be compiled using gcc. Then it will return an a.out result and this one will receive 3 arguments. The first one is the operation (+ or x). The second and third one are the polynomials in binary.

## Examples of commands

- ./a.out + 1000 1100
- ./a.out + 0010 1011
- ./a.out + 1110 1000
- ./a.out x 1010 0010
- ./a.out x 0110 1011
- ./a.out x 1000 1001