#include<iostream> 

using namespace std; 
template<typename type1,typename type2>//函数模板 

auto Max(type1 a,type2 b) ->decltype(a > b ? a : b)
{ 
    std::cout<<&a<<endl;
    std::cout<<&b<<endl;
    return a > b ? a : b; 
} 

int main() 
{ 
    int a = 0;
    int b = 9;
    std::cout<<&a<<endl;
    std::cout<<&b<<endl;
    std::cout<<&Max(0,1)<<endl;
    std::cout<<a<<endl;
    std::cout<<b<<endl;
    cout<<"Max = "<<Max(5.5,'a')<<endl; 
} 