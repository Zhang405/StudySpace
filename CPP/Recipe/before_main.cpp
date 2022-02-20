#include<string>
#include<string.h>


__attribute((constructor))void before(){
    printf("before main\n");
}

int main(){
    printf("main start\n");
    return 0;
}