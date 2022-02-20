#define pchar char*
typedef char* ptype;

#include<iostream>
#include<typeinfo>
__attribute__((destructor)) void after_main(){
    //printf();
    std::cout<<"After Destructor\n";
}
class A{
public:
    A(){std::cout<<"A()\n";}
    ~A(){std::cout<<"~A()\n";}
};
class B{
public:
    B(){std::cout<<"B()\n";}
    ~B(){std::cout<<"~B()\n";}
};
A a;
B b;
int main(){
    pchar a,b;
    ptype c,d;
    std::cout<<typeid(a).name()<<std::endl;
    std::cout<<typeid(b).name()<<std::endl;
    std::cout<<typeid(c).name()<<std::endl;
    std::cout<<typeid(d).name()<<std::endl;


    int e[10];
    std::cout<<sizeof e<<std::endl;
    int*p = e;
    std::cout<<sizeof p<<std::endl;
    return 0;
}