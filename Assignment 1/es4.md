# Main ideas:

The program instantiates a matrix that will keep track of each pair (alpha, beta) that will occur keeping the number of them. Than given the permutation operation the program will calculate every beta by a given alpha and x as so.

$S(x \oplus alpha) + S(x) = beta$

Counted every couple the program returns a matrix that will represent how many couples for each alpha (as row) and beta (as column).
After that the program will ask if the user is interested in a specific case of alpha and beta. Given the 2 cases it will take the number in that position anche divide it by 8 ($2^{(n-1)}$) and return the differential probability.



# How does the program work

To make the program work firstly has to be compiled using gcc. Then it will return an a.out result and this one will receive no arguments. But at the end of the computation it will ask the user for 2 values (alpha and beta) and calculate the probability.

## The result
$$\begin{array}{cc}
  &  0 &  1 &  2 &  3 &  4 &  5 &  6 &  7 &  8 &  9 &  A &  B &  C &  D &  E &  F \\
0 & 16 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 \\
1 &  0 &  2 &  0 &  2 &  0 &  0 &  0 &  0 &  4 &  2 &  4 &  2 &  0 &  0 &  0 &  0 \\
2 &  0 &  2 &  0 &  2 &  0 &  4 &  4 &  0 &  0 &  2 &  0 &  2 &  0 &  0 &  0 &  0 \\
3 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  2 &  2 &  2 &  2 &  2 &  2 &  2 &  2 \\
4 &  0 &  0 &  4 &  2 &  2 &  0 &  2 &  2 &  0 &  0 &  0 &  2 &  0 &  2 &  0 &  0 \\
5 &  0 &  0 &  2 &  0 &  0 &  0 &  0 &  2 &  0 &  0 &  6 &  0 &  2 &  2 &  2 &  0 \\
6 &  0 &  2 &  2 &  2 &  2 &  0 &  2 &  2 &  2 &  0 &  0 &  0 &  0 &  2 &  0 &  0 \\
7 &  0 &  2 &  0 &  0 &  0 &  0 &  0 &  2 &  0 &  2 &  0 &  4 &  4 &  0 &  0 &  2 \\
8 &  0 &  0 &  0 &  0 &  2 &  2 &  0 &  0 &  0 &  0 &  0 &  0 &  4 &  4 &  2 &  2 \\
9 &  0 &  0 &  0 &  0 &  4 &  6 &  2 &  0 &  0 &  0 &  0 &  0 &  2 &  0 &  0 &  2 \\
A &  0 &  0 &  0 &  0 &  0 &  2 &  2 &  0 &  2 &  2 &  2 &  2 &  0 &  2 &  2 &  0 \\
B &  0 &  4 &  0 &  4 &  2 &  2 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  0 &  2 &  2 \\
C &  0 &  0 &  2 &  0 &  0 &  0 &  2 &  0 &  4 &  0 &  2 &  0 &  0 &  0 &  2 &  4 \\
D &  0 &  2 &  4 &  0 &  0 &  0 &  2 &  4 &  0 &  2 &  0 &  0 &  0 &  0 &  2 &  0 \\
E &  0 &  0 &  0 &  2 &  2 &  0 &  0 &  0 &  0 &  4 &  0 &  2 &  0 &  2 &  2 &  2 \\
F &  0 &  2 &  2 &  2 &  2 &  0 &  0 &  4 &  2 &  0 &  0 &  0 &  2 &  0 &  0 &  0
\end{array}$$
The probability for alpha = 15 and beta = 1 is 0.25 or 2/8.
