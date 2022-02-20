#include<stdio.h>
#include<unistd.h>
#include<iostream>
using namespace std;
int main()
{
cout<<"hehe";
int ret = fork();
if(ret==0)
{
//child
cout<<"I am child\n";
}
else if(ret<0)
{
printf("Error\n");
}
else
{
cout<<"I am father\n";
}

return 0;
}