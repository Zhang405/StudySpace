#include <iostream>
#include <typeinfo>
struct A{
	void fooA(){}
};

struct B:public A{
	void fooB(){}
};

using namespace std;
int main(){
	A* aHandle_bObj = new B();
        cout<<typeid(aHandle_bObj).name()<<endl;
	cout<<sizeof(*aHandle_bObj)<<endl;
 	cout<<typeid(decltype(aHandle_bObj)).name()<<endl;
	/*try{
		B* p = dynamic_cast<B*>(aHandle_bObj);
		if(p == nullptr){
			cout<<"dynamic_cast gone wrong with non-pointer return"<<endl;
		}
	}	
	catch(...){
		cout<<"dynamic_cast gone wrong with throw a exception"<<endl;
	}*/
	return 0;
}
