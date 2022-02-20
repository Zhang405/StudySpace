#include <iostream>

struct A{
public:
	int ax_;
	A(){std::cout<<"Ctor is Called\n";}
	~A(){std::cout<<"Dtor is Called\n";}
};


int main(){
	A* pa = new A();
	//pa->A::A();
	pa->A::~A();
	return 0;
}



