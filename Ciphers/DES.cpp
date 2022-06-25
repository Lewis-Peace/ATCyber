#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>

class DES {
private:
    /* data */

    uint32_t* round(uint32_t round_input[2]) {
        uint32_t left_part = round_input[0];
        uint32_t right_part = round_input[1];
    }

    void expansion_permutation(uint32_t expansion_input, uint64_t result) {
        
    }

public:
    DES(/* args */);
    ~DES();
};

DES::DES(/* args */)
{
}

DES::~DES()
{
}
