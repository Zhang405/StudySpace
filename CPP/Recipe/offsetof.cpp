#include <iostream>
#include <stddef.h>
using namespace std;

struct  S
{
    int x;
    char y;
    int z;
    double a;

    void* operator new(size_t size){
        cout<<"S"<<endl;
        return ::operator new(size);
    }
};

void fun(int(*p)(int,int))
{
    cout<<p(1,1)<<endl;
}
int main()
{
    cout << offsetof(S, x) << endl; // 0
    cout << offsetof(S, y) << endl; // 4
    cout << offsetof(S, z) << endl; // 8
    cout << offsetof(S, a) << endl; // 12
    new S;
    printf("%p\n", &S::x);
    printf("%p\n", &S::y);
    printf("%p\n", &S::z);
    printf("%p\n", &S::a);
    int (*pi)(int,int) = [](int a,int b){return a+b;};
    fun(pi);
    return 0;
}