#include "lib.hpp"
#include <iostream>

int main()
{
        A a(42);
        std::cout << a.member << std::endl;
        a.setMember(21);
        std::cout << a.member << std::endl;
}
