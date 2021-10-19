#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include<signal.h>

#define N 5

void handler(int sig){
    write(1,"!",1);
}


int main()
{
    int i;
    sigset_t set,oldset;
    signal(SIGINT,handler);
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    while (1)
    {
        //在打印一行五个的期间，屏蔽SIGINT
        sigprocmask(SIG_BLOCK,&set,NULL);
        for (i = 0;i < N;i++)
        {
            write(1,"*",1);
            sleep(1);
        }
        write(1,"\n",1);
        //一行结束取消屏蔽
        sigprocmask(SIG_UNBLOCK,&set,NULL);
    }
    

    
    exit(0);
}
