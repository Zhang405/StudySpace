#include <iostream>
#include <memory>
using namespace std;
class MyClass{
public:
    int* p_;
    MyClass(){
        p_ = new int(5);
    }
    
    ~MyClass(){
        delete p_;
    }
};


unique_ptr<MyClass> func(unique_ptr<MyClass> uniObj){
    return uniObj;
}

int main(){
    unique_ptr<MyClass> p(new MyClass());
    unique_ptr<MyClass> pRet = func(move(p));
    if(p == nullptr){
        cout<<"p is nullptr"<<endl;
    }
    cout<<pRet->p_<<endl;
    shared_ptr<MyClass> pShr(move(pRet));
    cout<<pShr->p_<<endl;

}
