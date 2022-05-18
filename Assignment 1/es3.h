unsigned int addition(unsigned int, unsigned int);
unsigned int multiplication(unsigned int, unsigned int);
unsigned int reduction(unsigned int);

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