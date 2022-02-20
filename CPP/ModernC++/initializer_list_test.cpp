#include<iostream>
#include<initializer_list>
class MyClass
{
private:
    int a_;
    int b_;
    int c_;
public:
    MyClass(int a,int b,int c):a_(a),b_(b),c_(c){
        std::cout<<"Ctor is Called!"<<std::endl;
    }
    MyClass(std::initializer_list<int> a){
        std::cout<<"Initializer_list Ctor is Called!"<<std::endl;
    }
    ~MyClass(){}
};

int main(){
    MyClass a{1,2,3};
    MyClass b = {1,2,3};
    return 0;
}

