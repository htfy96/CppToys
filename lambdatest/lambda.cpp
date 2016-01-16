#include <iostream>


class constant;

template<typename T>
struct replace
{
        typedef T type;
};

template<>
struct replace<int>
{
        typedef constant type;
};

template<typename T1, typename T2>
class lambda_add;

template<typename T1, typename T2>
class lambda_less;

template<typename T1, typename T2>
lambda_add<typename replace<T1>::type, typename replace<T2>::type>
operator+ (const T1& a1, const T2& a2);

template<typename T1, typename T2>
lambda_less<typename replace<T1>::type, typename replace<T2>::type>
operator< (const T1& a1, const T2& a2);

template<typename T1, typename T2>
class lambda_base
{
        protected:
                T1 a1_;
                T2 a2_;
        public:
                lambda_base(const T1& a1, const T2& a2) : a1_(a1), a2_(a2) {}
};

template<typename T1, typename T2>
class lambda_add : protected lambda_base<T1, T2>
{
        public:
                lambda_add(const T1& a1, const T2& a2) : lambda_base<T1, T2>(a1, a2) {}
                int operator() (int arg1, int arg2)
                {
                        return lambda_base<T1, T2>::a1_(arg1, arg2) + lambda_base<T1, T2>::a2_(arg1, arg2);
                }
                friend lambda_add<typename replace<T1>::type, typename replace<T2>::type>
                        operator+<> (const T1&, const T2&);
                friend lambda_less<typename replace<T1>::type, typename replace<T2>::type>
                        operator< <> (const T1&, const T2&);
};


template<typename T1, typename T2>
class lambda_less : protected lambda_base<T1, T2>
{
        public:
                lambda_less(const T1& a1, const T2& a2) : lambda_base<T1, T2>(a1, a2) {}
                int operator() (int arg1, int arg2)
                {
                        return lambda_base<T1, T2>::a1_(arg1, arg2) < lambda_base<T1, T2>::a2_(arg1, arg2);
                }
                friend lambda_add<typename replace<T1>::type, typename replace<T2>::type>
                        operator+<> (const T1&, const T2&);
                friend lambda_less<typename replace<T1>::type, typename replace<T2>::type>
                        operator< <> (const T1&, const T2&);
};

template<typename T1, typename T2>
lambda_add<typename replace<T1>::type, typename replace<T2>::type>
operator+ (const T1& a1, const T2& a2)
{
        return lambda_add<typename replace<T1>::type, typename replace<T2>::type>(a1, a2);
}

template<typename T1, typename T2>
lambda_less<typename replace<T1>::type, typename replace<T2>::type>
operator< (const T1& a1, const T2& a2)
{
        return lambda_less<typename replace<T1>::type, typename replace<T2>::type>(a1, a2);
}

class constant
{
        private:
                int c_;
        public:
                constant(const int c) : c_(c) {}
                int operator() (int arg1, int arg2)
                {
                        return c_;
                        (void)(arg1); (void)(arg2); //eliminate warning
                }
};

class placeholder1
{
        public:
                int operator() (int arg1, int arg2)
                {
                        return arg1;
                        (void)(arg2);
                }
};

class placeholder2
{
        public:
                int operator() (int arg1, int arg2)
                {
                        return arg2;
                        (void)(arg1);
                }
};

placeholder1 _1;
placeholder2 _2;


int main()
{
        auto l = 2 + _1 + _2;
        auto lnew = _1 + 2 + 3 + _2;
        auto l3 = (_1 + 2) < _2;
        std::cout<< l(2,-43) << std::endl;
        std::cout << lnew(3,4) << std::endl;
        std::cout << l3(0,1) << " "<< l3(-2, 1) << std::endl;
}

