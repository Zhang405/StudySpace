/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-08 22:36:35
 */
#include <stdio.h>
#include <unistd.h>
int main()//正确的
{
        char s[]="/home";
        chdir (s);
        char xxx[1024];//获取当前目录并打印
        getcwd(xxx,1024);
        printf("The current directory is : %s\n",xxx);
}