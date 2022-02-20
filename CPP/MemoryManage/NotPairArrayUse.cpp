#include <iostream>

class Trivial_Dtor{
	int ax_;
public:
	Trivial_Dtor()=default;
	~Trivial_Dtor()=default;
};

class Non_Trivial_Dtor{
	int bx_;
public:
	Non_Trivial_Dtor()=default;
	~Non_Trivial_Dtor(){std::cout<<"Do Something"<<std::endl;}
};
class Pointer_Inside{
	int* pint_;
public:
	Pointer_Inside(){pint_ = new int(5);}
	~Pointer_Inside(){delete pint_;}
};
int main(){
	Pointer_Inside* pint = new Pointer_Inside[10];
	delete pint;
	return 0;
}
