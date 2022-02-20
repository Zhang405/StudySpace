#include <iostream>
#include <typeinfo>
using namespace std;

struct A{
       virtual void foo(){}
};

struct B:public A{
};

int main(){
	B b;
	A& b_ = b;
	b_.foo();
	cout<<typeid(b_).name()<<endl;
	cout<< sizeof(b_)<<endl;
	return 0;
}
