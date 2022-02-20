#include <iostream>


auto add(auto a,auto b)
{
    return a+b;
}
// int main()
// {
//     //值类型，auto推断忽略const
//     const int a = 0;
//     auto acopy = a;
//     acopy = 1;
//     std::cout<<"a = "<<a<<"acopy = "<<acopy<<std::endl;

//     //引用类型，auto推断忽略const
//     int c = 0;
//     const int&c_ref =c;
//     auto c_ref_cp = c_ref; 
//     c_ref_cp = 8;
//     std::cout<<"c = "<<c<<"c_ref_cp = "<<c_ref_cp<<std::endl;

//     //指针类型，auto保留const
//     const int* b = &c;
//     auto bcopy = b;
//     std::cout<<"b = "<<*b<<"bcopy = "<<*bcopy<<std::endl;

//     //指针类型，auto推断为指针
//     int* p = &c;
//     auto p_cp = p;

//     //引用类型，auto推断为去掉引用
//     int& d = c;
//     auto d_ref = d;

//     std::cout<<add(1,2)<<std::endl;
// }

// template<typename _Tx,typename _Ty>
// auto mult(_Tx a,_Ty b)->decltype(a*b){
//     return a*b;
// }

template<typename _Tx,typename _Ty>
auto mult(_Tx a,_Ty b){
    return a*b;
}
#include<typeinfo>
#include<string>
int main(){
    std::cout<<mult(1,2)<<std::endl;
    return 0;
}


