# Main ideas:

MixColumn consists in pre-multiply a pre-defined and invertible matrix to a matrix of GF($2^4$) elements. The program asks the user for a matrix (row by row) and multiplies that with the static one. The multiplication operation is the same as the exercise 3 (with also the same irreducible polynomial). So after each multiplication and sum there will be a reduciton to the polynomial (with the irreducible one) until it is of grade 3.

# How does the program work

To make the program work firstly has to be compiled using gcc. Then it will ask for the matrix. The matrix has to be given row by row. So has to be inserted 4 values (in the hex form) divided by a space, and after that pressing enter to insert the next row.

## The result
$$\begin{equation}
    \begin{array}{cc}
    1 & 0 & 2 & 3 \\
    1 & 0 & 0 & 0 \\
    0 & 1 & 2 & 0 \\
    3 & 0 & 1 & 0 
    \end{array}*
    \begin{array}{cc}
    1 & f & c & 2 \\
    0 & 0 & 1 & 0 \\
    f & 0 & 0 & 2 \\
    3 & c & 0 & 0 
    \end{array}=
    \begin{array}{cc}
    9 & 8 & C & 6 \\ 
    1 & F & C & 2 \\ 
    D & 0 & 1 & 4 \\ 
    C & 2 & 7 & 4
    \end{array}

\end{equation}$$