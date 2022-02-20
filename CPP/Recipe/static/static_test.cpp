#include<iostream>
extern int g_a;
extern int g_func();
const int a = 10;
int main(){
    std::cout<<g_a<<std::endl;
    std::cout<<g_func()<<std::endl;
    std::cout<<&a<<std::endl;
    return 0;
}