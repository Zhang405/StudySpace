#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    int pid;
    if(pipe(fd)<0)
    {
        perror("pipe():");
        exit(1);
    }

    pid = fork();
    if(pid < 0)
    {
        perror("fork():");
        exit(1);
    }

    if(pid == 0)  //子
    {
        close(fd[0]);
        write(fd[1],"Hello World\n",12);
        close(fd[1]);
        exit(0);
    }
    else
    {
        close(fd[1]);
        char line[1024];
        int n = read(fd[0],line,1024);
        close(fd[0]);
        write(1,line,n);
        wait(NULL);
    }
    exit(0);
}