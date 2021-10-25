#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

void* func(void*p)
{
    int i;
    printf("%ld\n",(long)(&i));
    pause();
    pthread_exit(NULL);
}
int main()
{
    int i =0;
    int err;
    pthread_t tid;

    for(int i;;i++)
    {
        err = pthread_create(&tid,NULL,func,NULL);
        if(err)
        {
            printf("%d\n",i);
            exit(0);
        }
    }
    exit(0);
}