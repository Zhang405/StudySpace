#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>

#define THRNUM 20
#define LEFT 30000000
#define RIGHT 30000200

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

static void*handle(void* p)
{

    while(1)
    {
        pthread_mutex_lock(&mut_num);
        while (num == 0)
        {
            pthread_cond_wait(&cond_num,&mut_num);
        }
        
        if(num == -1)
        {
            pthread_mutex_unlock(&mut_num);
            break;
        }

        int task = num;
        num = 0;
        pthread_cond_broadcast(&cond_num);
        pthread_mutex_unlock(&mut_num);

        int mark = 1;
        for (int j = 2;j <= task/2;j++){
            if (task%j == 0){
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("[%2d] %d is a primer\n",*(int *)p,task);
        }
    }
    free(p);
    pthread_exit(NULL);
}

int main()
{
    pthread_t ptid[THRNUM];
    for(int i = 0 ; i!= THRNUM;i++)
    {
        int *num = malloc(sizeof(int));
        *num = i;
        int err = pthread_create(ptid+i,NULL,handle,num);
        if(err)
        {
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    for(int i = LEFT;i != RIGHT; i++)
    {
        pthread_mutex_lock(&mut_num);
        while(num != 0)
        {
            pthread_cond_wait(&cond_num,&mut_num);
        }
        num = i;
        pthread_cond_signal(&cond_num);
        pthread_mutex_unlock(&mut_num);
    }

    pthread_mutex_lock(&mut_num);
    while(num != 0)
    {
        pthread_cond_wait(&cond_num,&mut_num);
    }
    num =-1;
    pthread_cond_broadcast(&cond_num);
    pthread_mutex_unlock(&mut_num);

    for(int  i = 0; i!= THRNUM;i++)
    {
        pthread_join(ptid[i],NULL);
    }

    pthread_mutex_destroy(&mut_num);
    pthread_cond_destroy(&cond_num);

    exit(0);
}

