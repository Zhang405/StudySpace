#include<iostream>
#include<stdio.h>
using namespace std;

int i = 1;
void func(){
    char a[1024];
    printf("第%d次申请栈空间，本次申请了1K的空间，共申请了%dK的空间\n",i,i);
    i++;
    func();
}
int main(){
    func();
    return 0;
}