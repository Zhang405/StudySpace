#include<iostream>
using namespace std;
void print(const char*s){
    while (*s)
    {
        cout<<*s++;
    }
    
}

template<typename T,typename... Types>
void print(char* s,const T v,const Types&... args){
    while(*s){
        if(*s == '%'&&*(++s) != '%'){
            cout<<v;
            print(++s,args...);
            return;
        }
        cout<<*s++;
    }
}
int main(){
    print("%sddd%d\n","string",5);
    return 0;
}