#include "../header/common_header.hpp"
class T{
    static const double i;
    static const std::string x; 
public:
    int a[5];
    std::string s = "char";
};
const std::string T::x = "char";
const double T::i = 5.0;
int main()
{
    T s;
    return 0;
}
