#include "inc/common.hpp"



int main()
{
    int num = 0;

    std::vector<int*> vec;

    int& ref = num;

    vec.push_back(&ref);

    *vec[0] = 5;

    std::cout << num << std::endl;
}