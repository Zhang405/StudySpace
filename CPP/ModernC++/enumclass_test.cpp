#include <iostream>


enum class Enum_Type:int
{
    EType_a = 0,
    EType_b,
    EType_c,
    EType_d
};

//强类型的枚举
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value,std::ostream>::type& stream,const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}
int main()
{
    Enum_Type::EType_a;
    //std::cout<<static_cast<int>(Enum_Type::EType_b);
    std::cout<<Enum_Type::EType_b;
    return 0;
}