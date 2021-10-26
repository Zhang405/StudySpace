#include "mypipe.h"
#include "stdlib.h"
#include "stdio.h"
#include <pthread.h>


static mypipe_t* p_MyPipe;
int main()
{
    p_MyPipe = mypipe_init();

    pthread_t tid_rd,tid_wr;
    pthread_create()

}