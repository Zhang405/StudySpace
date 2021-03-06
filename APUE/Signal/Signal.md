@[TOC](文章目录)
<hr>

# 信号

## 信号的概念
**信号是软件中断，横多比较重要的应用程序都需要处理信号，信号提供了一种处理异步时间的方法**
**信号的响应依赖于中断**
- kill -l
## signal()
- `void (*signal(int signum,void (*func)(int)))(int)`
![](https://i.loli.net/2021/10/14/mEgLuhU61bWIJpF.png)
~~~ c
void test(int sig){};

void (*signal(int sig,test))(int){};
//void (*res)(int) signal(int sig,test){};

~~~


**信号会打断阻塞的系统调用**

## 信号的不可靠
**信号的行为不可靠**
信号可能会丢失，但这里的不可靠指的是行为不可靠，而不是丢失这种情况。行为不可靠的原因在于信号处理函数的调用现场是内核布置的，不是像递归这样的是用户主动跳转的。假设一个信号处理函数还在进行中，另一个相同的信号也到达，这样可能会导致错误。

## 可重入函数
**解决信号的不可靠问题 第一次调用没结束 第二次调用开始了**
**第一次调用没结束 发生第二次调用 但不会出错**
**所有的系统调用都是可重入的 一部分库函数也是可重入的(如memcpy)**


>可重入是怎么实现的？
## 信号的响应过程(重要)
**信号从收到到响应有不可避免的延时**
标准信号的响应没有严格的顺序


![在这里插入图片描述](https://img-blog.csdnimg.cn/20210213182415635.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0ODA3Nzk2,size_16,color_FFFFFF,t_70#pic_center)



## 信号的常用函数
- kill() **不是用来杀死进程的，是用来发送信号的，只不过大多数信号有杀死进程的作用**
函数原型：int kill(pid_t pid, int sig);
参数pid_t分为一下几种情况：
1>pid为正数：向指定pid的进程发信号
2>pid等于0：向调用kill的进程所在组的所有进程发信号(组内广播)
3>pid等于-1：向kill的调用者进程所有有权限对其发信号的进程发送信号，但除了init进程（pid = 1）(全局广播)
4>pid小于-1：对pgid = abs（pid）的组内所有进程发送信号

参数sig：
sig = 0：没有信号，但是会检错，用来检测进程或进程组是否存在
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
    //int kill(pid_t pid, int sig)
    //pid > 0 发送给指定进程
    //pid == 0 发送给同组所有进程 即 组内广播
    //pid == -1 全局广播 init专用
    //pid < -1 发送给 ppid == |pid|的所有进程
    //当sig == 0时用于检测一个进程是否存在


    exit(0);
}
~~~

- raise() 等价与`kill(getpid(), sig);`
给当前进程或线程发信号
- **alarm()**
- pause 人为制造的阻塞系统调用 等待一个信号来打断它
- sleep:在某些环境下，sleep是有alarm+pause封装的，在此情况下，如果编码者写的程序中有alarm，可能会造成始终的紊乱而出现错误，故尽量少用sleep。一些使用nanosleep封装的则没有这种问题。
~~~ c
//5sec_sig.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//使用 信号 计算当前单核 累加 的速度

static /*volatile*/ int loop = 1;

static void handler(int sig){
    loop = 0;
}

int main()
{
    int64_t count = 0;

    alarm(5);
    signal(SIGALRM,handler);

    while(loop){
        count++;
    }

    printf("count %ld",count);

    exit(0);
}

~~~

~~~ bash
gcc 5sec_sig.c -S
~~~

~~~ x86asm
main:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	$0, -8(%rbp)
	movl	$5, %edihttps://github.com/impact-eintr/LinuxC
学习记录的笔记与代码，持续更新中
希望各位大佬给个star
	call	alarm@PLT
	leaq	handler(%rip), %rsi
	movl	$14, %edi
	call	signal@PLT
	jmp	.L3
.L4:
	addq	$1, -8(%rbp)
.L3:
	movl	loop(%rip), %eax ;执行循环
	testl	%eax, %eax
	jne	.L4
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
~~~


#### volatile 关键字
**去到这个变量真正的存储空间取数值，而不是根据局部结构判断取值**

**下面在视频中提到的由于优化产生的错误，在我的gcc9.3版本中并没有重现**
~~~ bash
gcc 5sec_sig.c -O1
./a.out         //不会有返回的 进入死循环

~~~

注意注释
~~~ x86asm
main:
.LFB23:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$5, %edi
	call	alarm@PLT
	leaq	handler(%rip), %rsi
	movl	$14, %edi
	call	signal@PLT
	cmpl	$0, loop(%rip)
	je	.L3
.L4:
	jmp	.L4   //未进行检查值
.L3:
	movl	$0, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.data
	.align 4
	.type	loop, @object
	.size	loop, 4
loop:
	.long	1                 ;将loop的值固定为1
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
~~~

原因在于这一句
~~~ c
//loop在局部循环中没有改变值 gcc 认为loop的值不会发生改变 永远为 1
while(loop){
    count++;
}
~~~

解决办法就是
~~~ c
static volatile int loop = 1;
~~~



#### 流控算法
实现流量控制的slowcat,通过漏桶实现,即无则等待,有则按给定量传输。
```c


int main()
{
    
}

```



#### 令牌桶(优化后的流控算法)
积攒token，有能力一次处理一部分数据的激增。
~~~ c
#ifndef MYTBF_H__
#define MYTBF_H__

#define MYTBF_MAX 1024

typedef void mytbf_t;

mytbf_t *mytbf_init(int cps,int burst);

//获取token
int mytbf_fetchtoken(mytbf_t *,int);
//归还token
int mytbf_returntoken(mytbf_t *,int);

int mytbf_destroy(mytbf_t *);

#endif
~~~

~~~ c
#include <asm-generic/errno-base.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "mytbf.h"

struct mytbf_st{
    int csp;
    int burst;
    int token;
    int pos;//任务列表的下标
};

static struct mytbf_st *job[MYTBF_MAX];
static volatile int inited = 0;
static void (*alarm_status)(int);

static int get_free_pos(){
    for (int i = 0;i < MYTBF_MAX;i++){
        if (job[i] == NULL)
          return  i;
    }
    return -1;
}


//信号处理函数
static void handler(int sig){
    alarm(1);
    for (int i = 0;i < MYTBF_MAX;i++){
        if (job[i] != NULL){
            job[i]->token += job[i]->csp;
            if (job[i]->token > job[i]->burst){
                job[i]->token = job[i]->burst;
            }
        }
    }
}

//装载信号处理模块
static void mod_load(){
    alarm_status = signal(SIGALRM,handler);//保存alarm信号处理函数原来的状态
    alarm(1);
}
//卸载信号处理模块 当发生异常退出时 可以将占用的资源释放 将alarm信号取消
static void mod_unload(){
    signal(SIGALRM,alarm_status);
    alarm(0);
    for (int i = 0;i < MYTBF_MAX;i++){
        free(job[i]);
    }
}

mytbf_t *mytbf_init(int cps,int burst){
    struct mytbf_st *tbf;

    if (!inited){
        mod_load();
    }

    //将新的tbf装载到任务组中
    int pos;
    pos = get_free_pos();
    if (pos == -1){
        return NULL;
    }

    tbf = malloc(sizeof(*tbf));
    if (tbf == NULL)
        return NULL;
    tbf->token = 0;
    tbf->csp = cps;
    tbf->burst = burst;
    tbf->pos = pos;
    
    job[pos] = tbf;

    return tbf;
}

//获取token ptr是一个 void * size是用户想要获取的token数
int mytbf_fetchtoken(mytbf_t *ptr,int size){
    struct mytbf_st *tbf = ptr;

    if (size <= 0){
        return -EINVAL;
    }
    
    //有token继续
    while (tbf->token <= 0)
      pause();
    
    int n =tbf->token<size?tbf->token:size;

    tbf->token -= n;
    //用户获取了 n 个token
    return n;
}

//归还token ptr是一个 void *
int mytbf_returntoken(mytbf_t *ptr,int size){
    struct mytbf_st *tbf = ptr;

    if (size <= 0){
        return -EINVAL;
    }
    
    tbf->token += size;
    if (tbf->token > tbf->burst)
        tbf->token = tbf->burst;

    return size;
}

int mytbf_destroy(mytbf_t *ptr){
    struct mytbf_st *tbf = ptr;
    job[tbf->pos] = NULL;
    free(tbf);
    return 0;
}
~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <math.h>

#include "mytbf.h"

static const int SIZE = 1024;
static const int CPS = 3;
static const int BURST = 100;//最大令牌数

static volatile int token = 0;//持有令牌数

int main(int argc,char** argv)
{
    if (argc < 2){
        fprintf(stdout,"Usage...");
        exit(1);
    }

    mytbf_t *tbf;

    tbf = mytbf_init(CPS,BURST);
    if (tbf == NULL){
        fprintf(stderr,"tbf init error");
        exit(1);
    }

    //打开文件
    int sfd,dfd = 0;
    do{
        sfd = open(argv[1],O_RDONLY);
        if (sfd < 0){
            if (errno == EINTR)
              continue;
            fprintf(stderr,"%s\n",strerror(errno));
            exit(1);
        }
    }while(sfd < 0);

    char buf[SIZE];
    
    while(1){
        
        int len,ret,pos = 0;
        int size = mytbf_fetchtoken(tbf,SIZE);
        
        int i = 0;
        while(i < 2){
            sleep(1);
            i++;
        }

        if (size < 0){
            fprintf(stderr,"mytbf_fetchtoken()%s\n",strerror(-size));
            exit(1);
        }

        len = read(sfd,buf,size);
        while (len < 0){
            if (errno == EINTR)
              continue;
            strerror(errno);
            break;
        }

        //读取结束
        if (len == 0){
            break;
        }

        //要是读到结尾没用完token
        if (size - len > 0){
            mytbf_returntoken(tbf,size-len);
        }  

        //以防写入不足
        while(len > 0){
            ret = write(dfd,buf+pos,len);
            while (ret < 0){
                if (errno == EINTR){
                  continue;
                }
                printf("%s\n",strerror(errno));
                exit(1);
            }

            pos += ret;
            len -= ret;
        }
    }

    close(sfd);
    mytbf_destroy(tbf);

    exit(0);
}

~~~

- setitimer() 更灵活 **而且误差不累积**，alarm()只能一秒为单位设置时钟
![](https://i.loli.net/2021/10/19/NIqAxPJuUwsRzKe.png)
当it_value的值衰减为0时，it_interval的值赋给it_value，这个赋值是原子的。

>使用setitimer()替换alarm()
~~~ cpp
//信号处理函数
static void handler(int sig){
    struct itimerval itv;

    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    if(setitimer(ITIMER_REAL,&itv,NULL) < 0){
        perror("setitimer()");
        exit(1);
    }
    for (int i = 0;i < MYTBF_MAX;i++){
        if (job[i] != NULL){
            job[i]->token += job[i]->csp;
            if (job[i]->token > job[i]->burst){
                job[i]->token = job[i]->burst;
            }
        }
    }
}

//装载信号处理模块
static void mod_load(){
    alarm_status = signal(SIGALRM,handler);//保存alarm信号处理函数原来的状态

    struct itimerval itv;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    if(setitimer(ITIMER_REAL,&itv,&old_itv) < 0){
        perror("setitimer()");
        exit(1);
    }
}
//卸载信号处理模块 当发生异常退出时 可以将占用的资源释放 将alarm信号取消
static void mod_unload(){
    signal(SIGALRM,alarm_status);
    setitimer(ITIMER_REAL,&old_itv,NULL);

    for (int i = 0;i < MYTBF_MAX;i++){
        free(job[i]);
    }
}

~~~
- abort()
认为制造异常，杀掉当前进程
- system()
**在有信号参与的程序当中，要阻塞住一个信号，要忽略调两个信号 这样system()才能正常使用**
![](https://i.loli.net/2021/10/19/eWFkdrhE5JxXiV8.png)

#### sleep的缺陷
**在某些平台，`sleep()`是使用`alarm`+`pause`封装的，而程序中出现多于1个的`alarm`alarm将失效**
可替换的函数：nanosleep()、usleep()

## 信号集
- sigset_t          信号集类型,位图的形式囊括当前支持的信号个数
- sigemptyset() 	将一个信号集置为空集
- sigfillset() 	    将一个信号集置为全集
- sigaddset() 	    将一个信号加入信号集
- sigdelset()	    将一个信号移除信号集
- sigismember()     测试一个信号是否在信号集中

### 信号屏蔽字/pending集的处理
**我们无法控制信号何时到来，但可以选择如何响应它**
- sigprocmask(int how,const sigset_t *set,sigset_t *oldset)
how的取值
1> SIG_BLOCK    当前信号屏蔽字和set的并集，set中包含希望被阻塞的信号
2> SIG_UNBLOCK  当前信号屏蔽字和set的补集的交集，set中包含希望被取消阻塞的信号
3> SIG_SETMASK  设置mask为传入set值

**在sigprocmask返回之前，如果进程存在未决的，非阻塞的信号，则在返回前至少有一个被递送。**

>star.c
~~~ cpp
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define N 5

void handler(int sig){
    write(1,"S",1);
}


int main()
{
    int i;

    sigset_t sigset,old_sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGINT);

    signal(SIGINT,handler);
    
    //保存进入该模块前的状态
    sigprocmask(SIG_UNBLOCK,&sigset,old_sigset);
    while(1){
         //屏蔽对信号的响应
        sigprocmask(SIG_BLOCK,&sigset,&old_sigset);
        for (i = 0;i < N;i++){
            write(1,"*",1);
            sleep(1);
        }
        write(1,"\n",1);
        //恢复对信号的响应
        sigprocmask(SIG_SETMASK,&old_sigset,NULL);
    }
    //恢复进入该模块前的状态
    sigprocmask(SIG_SETMASK,&old_sigset,NULL);

    exit(0);
}
~~~
ctrl+\ 退出

- sigpending():返回当前进程未决的，阻塞的一组信号

- sigsetjmp、siglongjmp
在信号处理函数当中，不能直接使用setjmp、longjmp进行跳转，在一些环境下会丢失mask的值（信号响应过程：处理完成后返回内核恢复mask），故引入以上两个函数
## 扩展
- sigsuspend() 信号驱动
pause()实现的不足：在pause()之前，信号就已经被响应
相当于
```cpp
sigprocmask(SIG_SETMASK,&old_sigset,&temp_sigset);
pause();
sigprocmask(SIG_SETMASK,&temp_sigset,&NULL);
```

>信号驱动实现，使用sigaction
~~~ cpp
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define N 5

static void handler(int sig){
    write(1,"S",1);
}

int main()
{
    int i;

    sigset_t sigset,old_sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGINT);

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT,&sa,NULL);

    //保存进入该模块前的状态
    sigprocmask(SIG_BLOCK,&sigset,&old_sigset);
    while(1){
        for (i = 0;i < N;i++){
            write(1,"*",1);
            sleep(1);
        }
        write(1,"\n",1);
        sigsuspend(&old_sigset);
    }
    //恢复进入该模块前的状态
    sigprocmask(SIG_SETMASK,&old_sigset,NULL);

    exit(0);
}
~~~

~~~ cpp
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define N 5

void handler(int sig){
    write(1,"S",1);
}


int main()
{
    int i;

    sigset_t sigset,old_sigset,sigset_status;
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGINT);

    signal(SIGINT,handler);
    
    //保存进入该模块前的状态
    sigprocmask(SIG_UNBLOCK,&sigset_status,NULL);
    sigprocmask(SIG_BLOCK,&sigset,&old_sigset);
    while(1){
        for (i = 0;i < N;i++){
            write(1,"*",1);
            sleep(1);
        }
        write(1,"\n",1);
        sigsuspend(&old_sigset);
    }
    //恢复进入该模块前的状态
    sigprocmask(SIG_SETMASK,&sigset_status,NULL);

    exit(0);
}
~~~

- sigaction() 用来替换signal() 还可以指定信号的来源以选择是否响应
signal()有重入的风险，如下图，在处理SIGINT的时候，被SIGQUIT打断。
![](https://i.loli.net/2021/10/20/xBOTNQ6Dg8FtMZE.png)
当然可以借助信号屏蔽字实现，在响应一个信号时，屏蔽其他信号。
~~~ cpp
//信号处理函数
static void handler(int sig,siginfo_t *infop,void *unused){
    struct itimerval itv;

    if (infop->si_code != SI_KERNEL){
        return ;
    }

    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    if(setitimer(ITIMER_REAL,&itv,NULL) < 0){
        perror("setitimer()");
        exit(1);
    }
    for (int i = 0;i < MYTBF_MAX;i++){
        if (job[i] != NULL){
            job[i]->token += job[i]->csp;
            if (job[i]->token > job[i]->burst){
                job[i]->token = job[i]->burst;
            }
        }
    }
}

//装载信号处理模块
static void mod_load(){
    struct sigaction sa;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGALRM,&sa,&old_sa) < 0){
        perror("sigaction()");
        exit(1);
    }


    struct itimerval itv;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    if(setitimer(ITIMER_REAL,&itv,&old_itv) < 0){
        perror("setitimer()");
        exit(1);
    }
}
//卸载信号处理模块 当发生异常退出时 可以将占用的资源释放 将alarm信号取消
static void mod_unload(){
   //signal(SIGALRM,alarm_status);
   sigaction(SIGALRM,&old_sa,NULL);
   
   //恢复时钟信号状态
    struct itimerval itv;
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 0;
    if(setitimer(ITIMER_REAL,&itv,&old_itv) < 0){
        perror("setitimer()");
        exit(1);
    }

    for (int i = 0;i < MYTBF_MAX;i++){
        free(job[i]);
    }
}
~~~

## 实时信号
不会丢失 有顺序
