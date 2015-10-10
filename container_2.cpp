#include <iostream>
#include <deque>
#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#if __cplusplus < 201103L
#error "this program is for CPP11+ only"
#endif

/***********************************************************
 * Define a type-safe container which could store any type
 * *********************************************************/

class Any
{
        private:

                class HolderBase
                {
                        public:
                                virtual ~HolderBase() {};
                                virtual HolderBase* clone() = 0;
                };

                template<typename T>
                        class Holder : public HolderBase //Force to be polymophic type
                {
                        public:
                                T content;

                                virtual ~Holder() {};
                                virtual HolderBase* clone()
                                {
                                        return new Holder<T>(content);
                                }
                                Holder(const T& arg) : content(arg) {};

                };

                HolderBase* ptr;
                std::type_index type_id;

        public:
                Any() : ptr(nullptr), type_id(typeid(nullptr)) {};

                template<typename T>
                        Any(const T& arg) :  ptr(new Holder<T>(arg)) ,type_id( std::type_index(typeid(T)) ) {}

                ~Any()
                {
                        delete ptr;
                }

                Any(const Any& other) :
                        ptr(other.ptr->clone()),
                        type_id(other.type_id)
        {}

                Any& operator = (const Any& other)
                {
                        if (this == &other) return *this;
                        type_id = other.type_id;
                        delete ptr;
                        ptr = other.ptr->clone();
                        return *this;
                }


                template<typename T>
                        T& get() const
                        {
                                if (ptr == nullptr)
                                      throw std::runtime_error("cannot get an object from a null object");
                                if (std::type_index(typeid(T)) != type_id)
                                      throw std::runtime_error(std::string("cannot get an object whose type is ")+typeid(T).name() +". Type dismatches. \
                                                              Original type is "+type_id.name() );
                                return static_cast< Holder<T>* >(ptr) -> content;
                        }

};

// End of Any


class Container : public std::deque<Any> // For convinience I make Container inherit most features from std::deque<Any>.
{
        private:

                template<typename T>
                        void addFront(const T& arg)
                        {
                                this->push_front(Any(arg));
                        }

        public:

                template<typename T>
                        void add(const T& arg)
                        {
                                this->push_back(Any(arg));
                        }

                template<typename T>
                        Container(const T& arg)
                        {
                                addFront(arg);
                        }
                template<typename T, typename ...Args  >
                        Container(const T& arg, const Args & ... args) : Container(args ... )
                        {
                                addFront(arg);
                        }

};


using namespace std;

int main()
{
        Any a(1.23);
        cout << a.get<double>() <<endl;
        //cout << a.get<int> () <<endl;

        Any b = std::deque<int>() ;
        deque<int> & bref = b.get<deque<int> >();
        bref.push_back(1);
        bref.push_front(2);

        cout << b.get< deque<int> >().size() <<endl;

        Any c(a);
        cout << c.get<double> () <<endl;

        Any d;
        d = b;
        cout << *d.get<deque<int> >().rbegin() <<endl;
        /* Test of Any ends */
        /* Test of Container begins */
        Container container(1,1.245, std::deque<int>() );
        cout << container[1].get<double>() << endl;
        auto & third = container[2].get<std::deque<int> >();
        third.push_back(2);
        third.push_back(3);
        third.push_back(4);

        for (const auto& c:third)
              cout << c << " " <<endl;

        container.push_back(true);
        cout << container.size() << endl;

        Container other_container { 2.34, false, 'a' };
        cout << other_container[2].get<char>() << endl;

}
