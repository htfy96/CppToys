#include <iostream>
#include <utility>
#include <type_traits>

typedef void(*FunT)();

template<typename T,  typename U>
class inplace
{
};

struct nullobj{};
template<typename T>
nullobj inplace2(T&&)
{
        T();
        return std::declval<nullobj>();
}

using namespace std;
int main()
{
        decltype(inplace2([]{cout<<"123" << endl; })) a;
}
