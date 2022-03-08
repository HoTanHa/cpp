#include <iostream>
#include "test.h"
#include <cstring>

int main()
{
    memcpy(&abc_tesst.array_abc[1], a, sizeof(a));
    memcpy(&abc_tesst.array_abc[2], b, sizeof(b));

    std::cout << abc_array[1].a << ".." << abc_array[1].b << ".." << abc_array[1].c << std::endl;
    std::cout << abc_array[2].a << ".." << abc_array[2].b << ".." << abc_array[2].c << std::endl;
    std::cout << abc_array[3].a << ".." << abc_array[3].b << ".." << abc_array[3].c << std::endl;
    return 0;
    

}