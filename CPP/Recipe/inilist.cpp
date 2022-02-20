#include <iostream>
using namespace std;
class A
{
public:
    A()
    {
        cout << "默认构造函数A()" << endl;
    }
    A(int a)
    {
        value = a;
        cout << "A(int "<<value<<")" << endl;
    }
    A(const A& a)
    {
        value = a.value;
        cout << "拷贝构造函数A(A& a):  "<<value << endl;
    }
    A& operator =(const A& rhs){
        this->value = rhs.value;
        cout << "assigncopy:  "<<value << endl;
        return *this; 
    }
    int value;
};

class B
{
public:
    B() 
    {
        b = A(2);
    }
    //A a;
    A b;
};
int main()
{
    B b;
}

