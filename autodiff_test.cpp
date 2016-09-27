#include "autodiff.hpp"

#include <iostream>
#include <type_traits>
#include <typeinfo>
using namespace std;

int main()
{
    using namespace adiff;
    using p1 = Product<Plus< Number<3>, Plus< Var<VarName::x>, Number<42> > > , Number<15> >;
    using p2 = ConstantFold<p1>::type;
    cout << typeid(p2).name() << endl;
}

