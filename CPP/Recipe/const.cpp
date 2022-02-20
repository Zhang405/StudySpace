#include<iostream>
#include<vector>
using namespace std;

void func(int& a){
    cout<<"int&"<<endl;
}

void func(const int& a){
    cout<<"const int&"<<endl;
}
int main(){
    //const int a = 0;

    vector<int> a = {1,2,3};
    //func(a);
    return 0;
}