#include<iostream>
#include <cassert>

class add_lambda;
class lambda_interface
{
    protected:
        virtual lambda_interface* clone() const = 0;
    public:
        virtual ~lambda_interface() {};
        virtual int operator() (const int, const int) const = 0;
        friend add_lambda;
};

class add_lambda : public lambda_interface
{
    private:
        lambda_interface *p1, *p2;
    public:
        virtual int operator() (const int a, const int b) const
        {
            return p1->operator()(a,b) + p2->operator()(a,b);
        }

        add_lambda(const lambda_interface& m1, const lambda_interface& m2): 
            p1(m1.clone()), p2(m2.clone())
            {}    

        add_lambda(const add_lambda& a) : p1(a.p1->clone()), p2(a.p2->clone()) {}

        virtual lambda_interface* clone() const
        {
            return new add_lambda(*this);
        }

        virtual ~add_lambda()
        {
            delete p1; delete p2;
        }
};

class placeholder1 : public lambda_interface
{
    public:
        virtual int operator() (const int a, const int b) const
        {
            return a;
            (void)(b);
        }

        virtual lambda_interface* clone() const
        {
            return new placeholder1(*this);
        }
        virtual ~placeholder1() {}
};
class placeholder2 : public lambda_interface
{
    public:
        int operator() (const int a, const int b) const
        {
            return b;
            (void)(a);
        }

        lambda_interface* clone() const
        {
            return new placeholder2(*this);
        }
        virtual ~placeholder2() {}
};

add_lambda operator+ (const lambda_interface& a, const lambda_interface& b)
{
    return add_lambda(a,b);
}

placeholder1 _1;
placeholder2 _2;

class constant: public lambda_interface
{
    private:
        int n_;
    public:
        constant(const int m): n_(m) {}
        constant(const constant& m) : n_(m.n_) {}
        virtual int operator() (const int, const int) const
        {
            return n_;
        }
        virtual lambda_interface* clone() const
        {
            return new constant(*this);
        }
        virtual ~constant() {}
};

#include <iostream>
using namespace std;
int main()
{
    for (int i=0; i<10; ++i)
    {
        auto l = _1 + _2;
        cout << l(-10,20) << endl;
    }
    auto l = _2 + constant(2) + _1;
    cout << l(2, 12) << endl;
}
