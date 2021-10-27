#include "mypipe.h"
#include "stdlib.h"
#include "stdio.h"
#include <pthread.h>

static void* read_handle(void* p)
{
    
}

static void* write_handle(void* p)
{


}
static mypipe_t* p_MyPipe;
int main()
{
    p_MyPipe = mypipe_init();

    pthread_t tid_rd,tid_wr;
    pthread_create(&tid_rd,NULL,read_handle,NULL);
    pthread_create(&tid_wr,NULL,write_handle,NULL);

    void* ptr;
    pthread_join(tid_wr,ptr);
    pthread_join(tid_rd,ptr);

    mypipe_destory(p_MyPipe);
}