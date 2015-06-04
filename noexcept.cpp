/* ***************************************
 * Another application of noexcept as
 * exception indication
 *
 * Author: lz96@foxmail.com
 * June 4th, 2015
 * ***************************************/
#include <iostream>
#include <algorithm>
#include <stdexcept>
using namespace std;

template<typename T>
void f(T& x, T& y) 
        noexcept(
            noexcept(throw invalid_argument("x can't == y")) && 
            noexcept(swap(x,y))
            )
{
    if (x==y)
      throw invalid_argument("x can't == y");
    swap(x,y);
}

int main()
{

    int a=1, b=2;
    cout<< noexcept( f(a,b) )<<endl;
    f(a,b);
    cout<<a<<' '<<b<<endl;
    return 0;
}


