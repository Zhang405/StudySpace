/*
 * @Description: 测试文件，测试解锁和signal先后顺序的影响（伪唤醒问题）
 * @Author: XZMJX
 * @Date: 2021-12-05 20:19:01
 */
#include<pthread.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define COND_CHECK(func,cond,retv,errv)\
if((cond))\
{\
    fprintf(stderr,"\n[CHECK FAILED at %s:%d]\n|%s(...)=%d (%s)\n\n",\
                __FILE__,__LINE__,func,retv,strerror(errv));\
    exit(EXIT_FAILURE);\
}
#define ErrnoCheck(fun,cond,retv) COND_CHECK(func,cond,retv,errno)
#define PthreadCheck(fun,rc) COND_CHECK(fun,(rc!=0),rc,rc)
#define FOREVER for(;;)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
int predicate = 0;
unsigned long nwakeup = 0;   //总唤醒次数
unsigned long nspurious = 0; //总伪唤醒次数
const int NTHREADS = 8;

/**
 * @Description: 等待条件变量唤醒，并重置条件变量，重置predicate参数，统计总唤醒次数和伪唤醒次数
 * @Author: XZMJX
 * @Date: 2021-12-05 22:27:22
 * @param {void*} ignore
 * @return {*}
 */
void* thread(void* ignore)
{
    int rc;
    FOREVER{
        rc = pthread_mutex_lock(&mutex);
        PthreadCheck("pthread_mutex_lock",rc);
        while(predicate == 0)
        {
            rc = pthread_cond_wait(&cv,&mutex);
            PthreadCheck("pthread_cond_wait",rc);
            nwakeup++;
            if(predicate == 0)
                nspurious++;
        }
        predicate = 0;
        rc = pthread_mutex_unlock(&mutex);
        PthreadCheck("pthread_mutex_unlock",rc);
    }
    pthread_exit(NULL);
}
/**
 * @Description: 用来准备条件变量，并唤醒线程
 * @Author: XZMJX
 * @Date: 2021-12-05 22:40:33
 * @param {void*} ignore
 * @return {*}
 */
void* signal_thread(void* ignore)
{
    int rc;
    FOREVER{
        rc = pthread_mutex_lock(&mutex);
        PthreadCheck("pthread_mutex_lock",rc);
        predicate = 1;

        rc = pthread_cond_signal(&cv);
        PthreadCheck("pthread_cond_signal",rc);
        rc = pthread_mutex_unlock(&mutex);
        PthreadCheck("pthread_mutex_unlock",rc);
    }
    pthread_exit(NULL);
}
/**
 * @Description: main线程
 * @Author: XZMJX
 * @Date: 2021-12-05 22:48:59
 * @param {*}
 * @return {*}
 */
int main()
{
    pthread_t tid[NTHREADS];
    pthread_t tsig;
    int rc;

    for(int i = 0; i < NTHREADS; ++i)
    {
        rc = pthread_create(tid+i,NULL,thread,NULL);
        PthreadCheck("pthread_create",rc);
    }
    rc = pthread_create(&tsig,NULL,signal_thread,NULL);
    PthreadCheck("pthread_create",rc);
    sleep(10);
    rc = pthread_mutex_lock(&mutex);
    PthreadCheck("pthread_mutex_lock",rc);
    printf("# wakeuo = %8lu\n# spurious = %8lu (%2.2f%%)\n",nwakeup,nspurious,(float)nspurious/nwakeup*100.0);
    rc = pthread_mutex_unlock(&mutex);
    PthreadCheck("pthread_mutex_unlock",rc);

    return EXIT_SUCCESS;
}