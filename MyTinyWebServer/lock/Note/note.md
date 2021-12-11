<!--
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-05 19:51:18
-->
## 调用signal/brodcast和解锁的先后次序
1.**`man`手册的说明**  

    > The pthread_cond_broadcast() or pthread_cond_signal() functions may be called by a thread whether or not it currently owns the mutex that threads calling pthread_cond_wait() or pthread_cond_timedwait() have associated with the condition variable during their waits; however, if predictable scheduling behavior is required, then that mutex shall be locked by thethread calling pthread_cond_broadcast() or pthread_cond_signal().
意思就是说，尽管既可以在持有锁的情况下调用`signal/broadcast`，也可以在解锁的情况下调用，但是如果需要调度行为是可预测的话，则应该在加锁的情况下调用`signal/broadcast`。

2.**加锁的情况下调用**
![](https://s2.loli.net/2021/12/05/hcrUvYCOXMamweG.png)  
考虑上图的场景：`T2`阻塞在条件变量上，`T1`在持有锁的情况下调用`signal`，接着上下文切换到`T2`，并且`T2`被唤醒，但是`T2`在从`pthread_cond_wait`返回时，需要重新加锁，然而此时锁还在`T1`手中。因此，`T2`只能继续阻塞（但是此时是阻塞在锁上），并且上下文又切换回`T1`。当`T1`解锁时，`T2`才得以继续运行。如果是调用`broadcast`唤醒等待条件变量的多个线程的话，那这种情形会变得更糟。
为了弥补这种缺陷，一些`Pthreads`的实现采用了一种叫做`waitmorphing`的优化措施，也就是当锁被持有时，直接将线程从条件变量队列移动到互斥锁队列，而无需上下文切换。
3. 解锁的情况下调用
解锁后调用`signal`有问题吗？首先，我们注意到，如果先进行`signal/broadcast`，则肯定会唤醒一个阻塞在条件变量上的线程；然而如果先解锁，则可能会唤醒一个阻塞在锁上的线程。
这种情形如何发生的呢？一个线程在锁上阻塞，是因为：
    a：它要检查条件，并最终会在条件变量上`wait`；
    b：它要改变条件，并最终通知那些等待条件变量的线程；
在a中，可能会发生唤醒截断的情况。重新考虑上图的场景，此时存在第三个线程`T3`阻塞在锁上。如果`T1`首先解锁，则上下文可能会切换到`T3`。现在`T3`检查到条件为真，进行处理，并在`T1`进行`signal/broadcast`之前，将条件重置。当`T1`进行`signal/broadcast`之后，`T2`被唤醒，而此时条件已经不再为真了。当然，在设计正确的应用中，这不是问题。因为`T2`必须考虑伪唤醒的情况。下面的代码模拟了这种场景：
```cpp
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
        rc = pthread_mutex_unlock(&mutex);
        PthreadCheck("pthread_mutex_unlock",rc);
        rc = pthread_cond_signal(&cv);
        PthreadCheck("pthread_cond_signal",rc);
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
    sleep(5);
    rc = pthread_mutex_lock(&mutex);
    PthreadCheck("pthread_mutex_lock",rc);
    printf("# wakeuo = %8lu\n# spurious = %8lu (%2.2f%%)\n",nwakeup,nspurious,(float)nspurious/nwakeup*100.0);
    rc = pthread_mutex_unlock(&mutex);
    PthreadCheck("pthread_mutex_unlock",rc);

    return EXIT_SUCCESS;
}
```
测试结果如下图，可见存在伪唤醒的情况（查的网上的实验结果伪唤醒高达40%，自己实现的并没有这么高），因为伪唤醒的存在，也说明了为什么
```cpp
    while(predicate == 0)
    {
        rc = pthread_cond_wait(&cv,&mutex);
        PthreadCheck("pthread_cond_wait",rc);
        nwakeup++;
        if(predicate == 0)
            nspurious++;
    }
```
处的`while()`不能写出`if()`;
![](https://s2.loli.net/2021/12/05/quEtXPhfswMRANW.png)  
另外，实测即使先`signal`再`unlock`，同样存在伪唤醒的情况。也不难理解，在`signal`之后唤醒一个等待在条件变量上的线程，在还未加锁之前，就调度到另外的线程，导致另一个**线程加锁-修改状态值-解锁完成**，从而造成前一个线程伪唤醒。

在b中，会推迟唤醒线程`T2`的时间。第三个线程`T3`阻塞在锁上，`T1`解锁后，`T3`得以继续执行。此时，只要`T1`不被调度，则它没有机会进行`signal/broadcast`，因此线程`T2`会一直阻塞。

4.**实时的情况**  

在实时性的程序中，线程的优先级反映了线程`deadline`的重要性。粗略的说，`deadline`越重要，则优先级应该越高。如果无法满足`deadline`的要求，则系统可能会失败、崩溃。
因此，你肯定希望高优先级的线程能尽可能早的获取`CPU`得以执行，然而，有可能会发生优先级反转的情况，也就是低优先级的线程阻碍了高优先级线程的执行。比如锁被低优先级的线程持有，使得高优先级的线程无法加锁。实际上，只要优先级反转的时间是有界且较短的话，这种情况不会造成太大问题。然而当反转时间变得无界时，这种情况就比较严重了，这会导致高优先级的线程无法满足其`deadline`。
当采用实时调度策略时，`signal/broadcast`会唤醒高优先级的线程。如果多个线程具有相同的优先级，则先在条件变量上阻塞的线程会被唤醒。
在线程进行`signal/broadcast`之前，也可能会发生优先级反转。继续考虑上图的场景：T1是个低优先级（P1）的线程，T2是高优先级（P2）的线程，T3的优先级（P3）介于T1和T2之间：P1 < P3 < P2。
如果T1先进行`unlock`，则其在`unlock`和`signal/broadcast`之间，T1可能被更高优先级的T3抢占，从而T1无法唤醒T2，因此低优先级的T3阻碍了高优先级的T2的运行，发生了优先级反转。
如果T1先进行`signal/broadcast`，假设锁使用了优先级天花板或继承协议（参考《Programming.With.Posix.Threads》第5.5.5.1节和5.5.5.2节），则可以保证T1在解锁后，T2会立即被调度。
因此，当持有锁时进行`signal/broadcast`更具优势。基于上面的讨论，在实时调度中，先`signal/broadcast`后`unlock`是必须的……。

5.**陷阱**  
如果先解锁，则可能会导致另一种问题：你必须保证解锁之后，用于`signal/broadcast`的条件变量依然有效。比如下面的代码：
```cpp
/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-05 23:20:42
 */
#include<pthread.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

#define COND_CHECK(func, cond, retv, errv) \
if ( (cond) ) \
{ \
   fprintf(stderr, "\n[CHECK FAILED at %s:%d]\n| %s(...)=%d (%s)\n\n",\
              __FILE__,__LINE__,func,retv,strerror(errv)); \
   exit(EXIT_FAILURE); \
}
 
#define ErrnoCheck(func,cond,retv)  COND_CHECK(func, cond, retv, errno)
#define PthreadCheck(func,rc) COND_CHECK(func,(rc!=0), rc, rc)
#define FOREVER for(;;) 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t *ptr_cv;
int predicate = 0;
int nthreads; 

/*****************************************************************************/
/* thread - tell the shutdown thread that we're done                        */
/*****************************************************************************/
void*
thread(void* ignore)
{
   int rc;

   // this thread now terminate 
   rc = pthread_mutex_lock(&mutex);
   PthreadCheck("pthread_mutex_lock", rc);

   nthreads--; // we have one thread less in the pool

   // note: we unlock first, and then signal 
   rc = pthread_mutex_unlock(&mutex);
   PthreadCheck("pthread_mutex_unlock", rc);
   rc = pthread_cond_signal(ptr_cv);
   PthreadCheck("pthread_cond_signal", rc);
   
   // Ok, time to retire
   pthread_exit(NULL);
}


/*****************************************************************************/
/* shutdown_thread- wait all threads in the pool to finish and clean-up      */
/* condvar                                                                   */
/*****************************************************************************/
void*
shutdown_thread(void* ignore)
{
   int rc;

   // wait as long as one thread in the pool is running
   rc = pthread_mutex_lock(&mutex);
   PthreadCheck("pthread_mutex_lock", rc);

   while (nthreads>0) {
      rc = pthread_cond_wait(ptr_cv, &mutex);
      PthreadCheck("pthread_cond_wait", rc);
   }

   // all thread stopped running: we can destroy the condvar
   rc = pthread_cond_destroy(ptr_cv);
   PthreadCheck("pthread_cond_destroy", rc);
   free(ptr_cv);

   // unlock mutex, and bye! 
   rc = pthread_mutex_unlock(&mutex);
   PthreadCheck("pthread_mutex_unlock", rc);
   pthread_exit(NULL);
}


/*****************************************************************************/
/* main- main thread                                                         */
/*****************************************************************************/

const int NTHREADS = 8; // # threads in the pool

int
main()
{
   pthread_t     pool[NTHREADS]; // threads pool
   pthread_t     tshd;           // shutdown thread
   unsigned long count=0;        // counter
   int           rc;             // return code

   FOREVER {

      // initialize condvar 
      nthreads=NTHREADS;
      ptr_cv = (pthread_cond_t*) malloc(sizeof(*ptr_cv));
      ErrnoCheck("malloc", (ptr_cv==NULL), 0);
      rc = pthread_cond_init(ptr_cv, NULL);
      PthreadCheck("pthread_cond_init", rc);

      // create shutdown thread
      rc = pthread_create(&tshd, NULL, shutdown_thread, NULL);
      PthreadCheck("pthread_create", rc);

      // create threads pool
      for (int i=0; i<NTHREADS; i++) {
         rc = pthread_create(pool+i, NULL, thread, NULL);
         PthreadCheck("pthread_create", rc);
         rc = pthread_detach(pool[i]);
         PthreadCheck("pthread_detach", rc);
      }

      // wait shutdown thread completion
      rc = pthread_join(tshd, NULL); 
      PthreadCheck("pthread_join", rc);

      // great... one more round
      ++count;
      printf("%lu\n", count); 
   }

   // should be never reached
   return EXIT_SUCCESS;

```
**为什么我没有出现segment fault**
十万多次后出现了，理论上原因是，条件变量失效而在这个条件变量的基础上`signal`。  
![](https://s2.loli.net/2021/12/05/9MGLcds13qp5rwB.png)