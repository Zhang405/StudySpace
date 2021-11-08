#include <iostream>
#include <string>
#include <tuple>

using namespace std;

std::tuple<int,string> tuple_test()
{
    return std::make_tuple(5,"Hello World");
}
int main()
{
    int a;
    string b;
    std::tie(a,b) = tuple_test();

    std::cout<<"***********tie_test**********\n";
    std::cout<<a<<'\n'<<b<<'\n';
    std::cout<<"*********C++17_test**********\n";

    auto [x,y] = tuple_test();
    std::cout<<x<<'\n'<<y<<'\n';
    return 0;

}