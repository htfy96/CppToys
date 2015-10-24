#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <string>
#include <cstring>
#include <sstream>

namespace ht
{
        template<typename T>
                std::string operator %(const std::string& str, const T& arg)
                {
                        auto stream_  =  std::ostringstream(str, std::ios_base::app);
                        stream_ << arg;
                        return stream_.str();
                }

}

using namespace std;

int main()
{
        using namespace ht;
        std::string s = std::string("2")%15%true;
        cout << s <<endl;
}
