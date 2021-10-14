# 进程


### PID
类型 pid_t
- ps axf 查看进程树
- ps axm
- ps ax -L
- ps -ef
### 进程的产生
- fork() 
        - 注意理解关键字 duplicating 意味着拷贝 克隆 一模一样
        - fork 后父子进程的区别 ： fork 的返回值不一样 pid不同 ppid也不同 未决信号与文🔓 不继承资源利用量清0
        - init进程 是所以进程的祖先进程 pid == 1
        - 调度器的调度策略来决定哪个进程先运行
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("%d start !\n",getpid());
    /****************************************************************
    刷新打开的流的缓冲区。此程序由于向标准输出中显示且末尾有换行，标准输出默认为行
    缓冲所以不会出现异常，但如果去除掉"\n"或者向其他打开的文件中写入，则会出现异常，
    其原因为父子进程的缓冲区各有一份start的缓冲
    ****************************************************************/
    fflush(NULL);//记得刷新 否则begin放到缓冲区 父子进程的缓冲区里各有一句begin

    pid_t pid = fork();

    if (pid == 0){
        printf("child %d\n",getpid());
    }else{
        printf("parent %d\n",getpid());
    }
    getchar();
    printf("pid %d end\n",getpid());
    return 0;
}

~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LEFT 2
#define RIGHT 200

int main()
{
    pid_t pid = 0;
    int i,j,mark;

    for (i = LEFT;i <= RIGHT;i++){
        pid = fork();
        if (pid == 0){
            mark = 1;
            for (j = 2;j < i/2;j++){
                if (i%j == 0){
                    mark = 0;
                    break;
                }
            }
            if (mark) {
                printf("%d is a primer\n",i);
            }
            exit(0);
        }
    }
    getchar();

    exit(0);
}

~~~
>只fork，子进程仍然会接着for循环fork...
>查看进程树表现如下:一共2的n次方个进程
>![](https://i.loli.net/2021/10/11/4jCEVrwaginOFyR.png)
- vfork()
vfork()只有执行exec函数族才是被定义的，其余动作未定义，现在的fork采用了写时拷贝(Copy-On-Write)技术，此函数逐渐被废弃。

### 进程的消亡以及释放资源
- wait()
- waitpid()
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define N 3
#define LEFT 100000002
#define RIGHT 100000200

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LEFT  3000000
#define RIGHT 3000200
int main()
{
	int pid = 0;
	int mark = 1;
	for(int i = LEFT ;i != RIGHT; i++ )
	{
		pid = fork();
		if (pid < 0){
            perror("fork");
            exit(1);
        }
		if(pid == 0)
		{
			mark = 1;
			for (int j = 2;j <= i/2;j++){
				if (i%j == 0){
					mark = 0;
					break;
				}
			}
			if (mark) {
				printf("%d is a primer\n",i);
			}
			//sleep(10000);
			exit(0);
		}
	}
	//sleep(10000);
	exit(0);
}
```
>子进程退出后，未收尸，让父进程sleep，使用ps命令查看进程树结果如下
>![](https://i.loli.net/2021/10/11/Eyp3QuqU2gLeBnw.png)
>子进程处于僵尸状态，未被收尸
>
>---
>若使每个子进程计算完成后sleep，而父进程提前退出，子进程会处于孤儿状态，被init接管
>ps查看进程树如下
>
>![](https://i.loli.net/2021/10/11/m8Ry2M7auGkxiDV.png)

可以看出，虽然进程调用了exit，但并未释放对应资源，比如pid。因此，遵循**谁申请，谁释放；谁打开，谁关闭**的原则，需要在父进程中释放对应资源。
- wait();
- waitpid()
优化程序如下
```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define N 3
#define LEFT 100000002
#define RIGHT 100000200

//交叉算法计算 池类算法涉及到竞争，分块计算会负载不均衡，此处交叉其实也不合适
//可以发现第一个线程接到的计算任务永远都是3的倍数，因此不会有质数的输出。
int main()
{
    printf("[%d] start !\n",getpid());
    fflush(NULL);//记得刷新 否则begin放到缓冲区 父子进程的缓冲区里各有一句begin
    pid_t pid = 0;
    int i,j,mark;

    for (int n = 0;n < N;n++){
        pid = fork();
        if (pid < 0){
            perror("fork");
            for (int k = 0;k < n;k++){
                wait(NULL);
            }
            exit(1);
        }

        if (pid == 0){
            for (i = LEFT+n;i <= RIGHT;i+=N){
                mark = 1;
                for (j = 2;j <= i/2;j++){
                    if (i%j == 0){
                        mark = 0;
                        break;
                    }
                }
                if (mark) {
                    printf("%d is a primer\n",i);
                }
            }
            printf("[%d] exit\n",n);
            exit(0);
        }

    }

    int st,n;
    for (n =0 ;n < N;n++){
        wait(&st);
        printf("%d end\n",st);
    }

    exit(0);
}

~~~

### exec函数族
**exec 替换 当前进程映像**
---
>默认情况下，由exec()的调用程序所打开的所有文件描述符在exec()的执行过程中会保持打开状态，且在新程序中依然有效。这通常很实用，因为调用程序可能会以特定的描述打开文件，而在新程序中这些文件将自动有效，无需再去了解文件名或是把它们重新打开。shell利用这一特性为其所执行的程序处理IO重定向，例如下面的shell命令。
>```
>$ ls /tmp > dir.txt
>```
>shell运行该命令时，执行了以下步骤。
>1.调用fork()创建子进程，子进程会也运行shell的一份拷贝（因此命令行也有一份拷贝）。
>2.子shell以描述符1（标准输出）打开文件dir.txt用于输出。可能会采取以下任一方式。
>	2.1. 子shell关闭描述符1（STDOUT_FILENO）后，随即打开文件 dir.txt。因为 open()在为描述符取值时总是取最小值，而标准输入（描述符0）又仍处于打开状态，所以会以描述符1来打开文件。
>	2.2.shell打开文件dir.txt，获取一个新的文件描述符。之后，如果该文件描述符不是标准输出，那么shell会使用dup2()强制将标准输出复制为新描述符的副本，并将此时已然无用的新描述符关闭。（这种方法较之前者更为安全，因为它并不依赖于打开文件描述符的低值取数原则。）
>3.子shell执行程序ls。ls将其结果输出到标准输出，即文件dir.txt中。
>上面是这个特性的作用，然而在某些场景，应避免使用这个特性，这时可以考虑执行时关闭（close-on-exec）标志（FD_CLOEXEC），如下文。
>在执行exec()之前，程序有时需要确保关闭某些特定的文件描述符。尤其是在特权进程中调用exec()来启动一个未知程序时（并非自己编写），或是启动程序并不需要使用这些已打开的文件描述符时，从安全编程的角度出发，应当在加载新程序之前确保关闭那些不必要的文件描述符。对所有此类描述符施以close()调用就可达到这一目的，然而这一做法存在如下局限性。
>	- 某些描述符可能是由库函数打开的。但库函数无法使主程序在执行exec()之前关闭相应的文件描述符。作为基本原则库函数应总是为其打开的文件设置执行时关闭（close-on-exec）标志。
>	- 如果exec()因某种原因而调用失败可能还需要使描述符保持打开状态。如果这些描述符已然关闭，将它们重新打开并指向相同文件的难度很大，基本上不太可能。
>	为此，内核为每个文件描述符提供了执行时关闭标志。如果设置了这一标志，那么在成功执行exec()时，会自动关闭该文件描述符，如果调用exec()失败，文件描述符则会保持打开状态。可以通过系统调用fcntl()来访问执行时关闭标志。fcntl()的F_GETFD操作可以获得文件描述符标志的一份拷贝。获取这些标志之后，可以对FD_CLOEXEC位进行修改，再调用fcntl()的F_SETFD操作使其生效。

---
- `extern char **environ`
- execl
- execlp
- execle
- execv
- execvpa
###### 一个小shell
~~~ c
//lhq yyds
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <glob.h>
#include <string.h>

#define BUFSIZE 1024
#define DELIMS " \t\n"

extern char **environ;

static int cd(char *path){
    int ret = chdir(path);
    if (ret == -1){
        perror("chdir");
    }
    return ret;
}

static void readrc(char *name){
    FILE *fp;
    fp = fopen(name,"r+");
    //处理文件内容
    fclose(fp);
}

static void prompt()
{
    char pwd[BUFSIZE];
    char name[BUFSIZE];
    getcwd(pwd,BUFSIZE);
    getlogin_r(name,BUFSIZE);
    printf("%s %s $ ",name,pwd);
}

static int parse(char *linebuf,glob_t *globres){
    char *tok;
    int flag = 0;
    

    while (1){
        tok = strsep(&linebuf,DELIMS);
        if (tok == NULL){
            break;
            return -1;
        }else if(strcmp(tok,"cd") == 0){
            char *path = strsep(&linebuf,DELIMS);
            return cd(path);
        }else if(tok[0] == '\0'){
            continue;
        }

        glob(tok,GLOB_NOCHECK|GLOB_APPEND*flag,NULL,globres);//第一次不能append glob_argv中是随机值 GLOB_NOCHECK | (GLOB_APPEND*flag)==0 第一次不append
        flag = 1;
    }
    return 1;
}


//之后记得 将 ctrl+c 转为 stdout:\n 将ctrl+d 转为 退出+再见标语
int main()
{
    printf("This is YSHELL\n");

    pid_t pid;
    char *linebuf = NULL;
    size_t lienbuf_size = 0;
    glob_t globres;//解析命令行

    //读取配置文件
    char *yshrc = "/home/yixingwei/.yshrc";//填一个绝对路径
    readrc(yshrc);

    while(1){
        prompt();


        //获取命令
        getline(&linebuf,&lienbuf_size,stdin);
        //解析命令
        int ret = parse(linebuf,&globres);
        
        if (ret == -1){
            
        }else if (ret == 0){//内部命令
            
        }else if (ret == 1){//外部命令
            fflush(NULL);
            pid = fork();
            if (pid < 0){
                perror("fork()");
                exit(1);
            }else if(pid == 0){
                execvp(globres.gl_pathv[0],globres.gl_pathv);
                perror("execl()");
                exit(1);
            }
        }
        waitpid(pid,NULL,0);
    } 
    
    exit(0);
}

~~~

### 用户权限以及组权限
- u+s 当其他用户调用该可执行文件时，会切换成当前可执行文件的user的身份来执行
- g+s

- uid/gid
    - r real
    - e effective

函数
- getuid 返回 real
- geteuid 返回 effective
~~~ c
// mysudo
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc,char **argv)
{
    if (argc < 3){
        fprintf(stderr,"Useage");
        exit(1);
    }

    pid_t pid;

    pid = fork();
    if (pid == 0){
        setuid(atoi(argv[1]));
        execvp(argv[2],argv+2);
        perror("execvp()");
        exit(1);

    }else {
        wait(NULL);
    }

    exit(0);
}

~~~

~~~ bash
$ su
# chown root ./mysudo
# chmod u+s ./mysudo

$ ./mysudo 0 /etc/shadow
~~~
- getgid
- getegid
- setuid 设置effective
- setgid 设置effective
- setreuid 交换 r e //是原子的交换
- setregid 交换 r e

### system()
可以理解成 fork+exec+wait 封装

### 进程会计
- acct()

### 进程时间
- times()

### 守护进程
- sesion 标识是`sid`
- 终端
- setsid()
- getpgrp()
- getpgid()
- setpgid()


### 系统日志
- syslogd 服务
- openlog
- syslog
- closelog

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>

#define FNAME "/tmp/out"

static int deamonize(){
    int fd;
    pid_t pid;
    pid = fork();

    if (pid < 0){
        return -1;
    }

    if (pid > 0){
        exit(0);
    }

    fd = open("/dev/null",O_RDWR);//输出都忽略
    if (fd < 0){
        return -1;
    }
    if (pid == 0){
        printf("test");
        fflush(NULL);
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        if (fd > 2){
            close(fd);
        }
        setsid();//脱离终端
        //umask();
        chdir("/");
    }
    return 0;
}

int main()
{
    FILE* fp;

    //开启日志服务
    openlog("print i",LOG_PID,LOG_DAEMON);

    if (deamonize()){
        syslog(LOG_ERR,"init failed!");
    }else{
        syslog(LOG_INFO,"successded!");
    }

    fp = fopen(FNAME,"w+");
    if (fp == NULL){
        syslog(LOG_ERR,"write file failed!");
        exit(1);
    }

    syslog(LOG_INFO,"%s opened",FNAME);

    for(int i = 0; ;i++){
        fprintf(fp,"%d\n",i);
        fflush(NULL);
        syslog(LOG_DEBUG,"%d 写入",i);
        sleep(1);
    }

    closelog();
    fclose(fp);
    exit(0);
}

~~~

~~~ bash
 journalctl _PID=XXX

~~~

@[TOC](文章目录)
