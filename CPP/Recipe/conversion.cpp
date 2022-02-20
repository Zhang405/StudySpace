#include<iostream>
using namespace std;
class A{
public:
    int a;
    A(){
        a = 0;
        cout<<"A():a = "<<a<<endl;
    }
    A(int i){
        a = i;
        cout<<"A(int i):a = "<<a<<endl;
    }
    ~A(){
        cout<<"~A():a = "<<a<<endl;
    }
    operator int(){
        cout<<"int():a = "<<a<<endl;
        return a;
    }
    int operator+(int i){
        cout<<"operator+():a = "<<a<<endl;
        return a+i;
    }
};

int main(){
    A a(1);
    cout<<a+1<<endl;
    cout<<1+a<<endl;
    return 0;
}