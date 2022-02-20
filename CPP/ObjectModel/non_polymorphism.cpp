#include <iostream>
#include <typeinfo>
using namespace std;

struct A{
	void foo(){}
};

int main(){
	A a;
	a.foo();
	cout<<typeid(a).name()<<endl;
	cout<< sizeof(a)<<endl;
	return 0;
}
