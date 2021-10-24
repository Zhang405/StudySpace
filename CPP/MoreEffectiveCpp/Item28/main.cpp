#include<iostream>
#include"MySmartPtr.h"

using namespace XZMJX;

int main()
{
    MySmartPtr<int> sp(new int(10));
    MySmartPtr<int> sp2(sp);
    MySmartPtr<int> sp3(new int(20));
    sp2 = sp3;
    std::cout << sp.Refer_Count() << std::endl;
    std::cout << sp3.Refer_Count() << std::endl;
    getchar();
    exit(0);
}