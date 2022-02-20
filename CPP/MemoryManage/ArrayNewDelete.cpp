#include <iostream>

struct A{
public:
	int id_;
	A(){std::cout<<"default ctor is called.this = "<<this<<" id = "<<id_<<std::endl;}
	A(int id):id_(id){std::cout<<"ctor is called.this = "<<this<<" id = "<<id_<<std::endl;}
	~A(){std::cout<<"dtor is called.this = "<<this<<" id = "<<id_<<std::endl;}	
};
int main(){
	A* buf = new A[3];
	A* tmp = buf;
	std::cout<<"buf = "<<buf<<" tmp = "<<tmp<<std::endl;
	for(int i = 0;i<3;++i){
		new(tmp++)A(i);
	}
	std::cout<<"buf = "<<buf<<" tmp = "<<tmp<<std::endl;
	delete[] buf;
	return 0;
}

