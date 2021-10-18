#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

#define CPS      10
#define BUFFSIZE CPS

static volatile int loop = 0;
static void alrm_handler(int i)
{
    alarm(1);
    loop = 1;
}

int main(int argc,char** argv)
{
    int sfd,dfd = 1;
    char buf[BUFFSIZE];
    int len,ret,pos;

    if(argc < 2)
    {
        fprintf(stderr,"Usage....\n");
        exit(1);
    }

    signal(SIGALRM,alrm_handler);
    alarm(1);
    do{
        sfd = open(argv[1],O_RDONLY);
        if(sfd < 0&&errno != EINTR)
        {
            break;
        }
    }while(sfd<0);
    

    while (1)
    {
        int flag = 0;
        if(!loop)
        {
            pause();
        }
        loop =0;
        
        do{
            len = read(sfd,buf,BUFFSIZE);
            if(len < 0)
            {
                if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                    flag = 1;
                    perror("read()");
                    break;
                }

            }
            if(len == 0)
            {
                flag = 1;
                break;
            }
            if(flag) break;
        }while(len < 0);
        pos = 0;
        while(len > 0)
        {
            ret = write(dfd,buf+pos,BUFFSIZE);
            if (ret < 0)
            {
                if(errno == EINTR)
                    continue;
                printf("%s\n",strerror(errno));
                exit(1);
            }
            pos += ret;
            len -= ret;
        }
    }
    close(sfd);
    
}