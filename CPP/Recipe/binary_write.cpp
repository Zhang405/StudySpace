#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

struct s{
    int x;
    int y;
    char p[5];
};
int binary_write(){
    s s1;
    s1.x = 9;
    s1.y = 10;
    memset(s1.p,1,5);
    int fd = open("./test",O_CREAT);
    write(fd,(char*)&s1,sizeof(s1));
    close(fd);
    return 1;
}

int main(){
    binary_write();
    return 0;
}