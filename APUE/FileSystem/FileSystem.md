# **Liunx_C系统编程**

## 文件系统
### 目录和文件
#### 获取文件属性
>如何用touch创建一个-开头的文件
>![](https://i.loli.net/2021/09/28/YicCF1sbGHMJVoA.png)

+ stat()

  ![](https://i.loli.net/2021/10/01/qrgXsjOYRTcVaku.png)

+ fstat()

+ lstat()

+ 空洞文件
1. 空洞文件(hole file)：在Linux中，lseek的系统调用是可以改变在文件上面的偏移量的，而且还允许其超出文件的长度。偏移量一旦超出了文件的长度，下一次进行文件IO写入操作文件的时候便会延续偏移量的位置继续写入，进而在文件中间产生了空洞的部分，这部分会以”\0”填充，而从原来的文件结尾到新写入数据间的这段空间就被称为“文件空洞”。
在Linux中，EOF（文件结束符）并不是一个字符，而是在读取到文件末尾的时候返回的一个信号值，也就是-1。
文件空洞部分实际上是不会占用任何的物理空间的，直到在某个时刻对空洞部分进行写入文件内容的时候才会为它分配对应的空间。但是在空洞文件形成的时候，逻辑上面的文件大小是分配了空洞部分的大小的。
外部命令cp支持拷贝空洞文件
2. 像在我们平时使用迅雷下载的时候，刚开始下载但是本地的下载文件就已经好几百兆了，这就利用了空洞文件。为了能够并行下载，创建空洞文件可以让多线程在不同的seek上面开始写入文件，如果不是空洞文件就只能串行写入了。
	在创建虚拟机的时候，我们会使用img工具生成一个例如50GB大小的镜像文件，但是其实在安装完系统之后，镜像的大小可能只有4GB，也就是说img并不会马上就占用掉物理存储空间的50GB，而是在未来使用过程中不断增加的。
	

#### 文件访问权限
+ st_mode:一个十六位的位图，其实际含义如下，表示文件类型、权限信息、特殊权限位
![](https://i.loli.net/2021/09/29/ItWzLMh71OG4vJU.png)
![](https://i.loli.net/2021/09/29/B2NW4hliq8DHCvg.png)
#### umask
防止产生权限过松的文件
外部命令umask
系统调用函数umask()

#### 文件权限的更改/管理
外部命令chomd
系统调用chomd(),fchomd()

#### 粘住位
+ t位
	开始的设计是为了标记某个二进制文件，是指留在内存中下次装载会更快，现在有pagecache机制，舍弃不用，现该位主要用于给某个目录设置
#### 文件系统：FAT,UFS
+ FAT:静态单项链表实现，反向查询难，承载能力有限
![](https://i.loli.net/2021/09/29/A4Ya79isK5fqyc3.png)

+ UFS:
![](https://i.loli.net/2021/09/29/r1AFfXObKCja4nd.png)
#### 硬连接，符号链接
文件名以目录项的形式存在与目录文件中，例如/etc下有一个目录文件，里面有目录项记录每个目录和其对应的inode号，如图.../testdir/的结构，可以看出目录自身默认的.和..目录就相当与硬链接
![](https://i.loli.net/2021/09/29/uTCEHW47dSo1abc.png)
+ 硬链接：相当于一个目录项与另一个目录项指向相同的`inode`，文件在其所有硬链接都被删除即引用计数归零的时候才会被删除,不能跨分区建立，不能给目录建立
![](https://i.loli.net/2021/09/29/FWsVyDE3mqKBvkj.png)
+  符号链接：相当于一个快捷方式，是一个独立的文件，删除源文件，改链接文件失效，现被设计为不占磁盘空间，一些信息被记录在inode节点中
![](https://i.loli.net/2021/09/29/vRHMh2ZKbcP5Uwd.png)
- 跟随符号链接：即该函数处理符号链接时，是否由符号链接找到链接的文件，例如stat()，lstat()
+  link(),unlink():系统调用
	unlink(),删除一个名字，当该文件引用数为0时删除该文件
+  remove()：标准函数
+  rename():系统调用

> 为什么linux不能给一个目录建立硬链接？ 
> Linux 文件系统中的目录均隐藏了两个个特殊的目录当前目录（.）和父目录（..），如果允许对目录进行硬链接，则会有下面两种情况：
> 情况A：存在一个/usr/local/myapp的目录， 我们将他hard link 链接到/root/目录ln /usr/local/myapp /root/myapp 那么，myapp目录里的(..)访问父目录，是指的/usr/local呢，还是/root/呢？
> 情况B: 如果存在目录/root/test/a 和 目录/root/app/b 如果a是app目录的hard link, b是test 的hard link, 这时就形成了循环引用，假设可以这样设定，那么/root/test/a 既然是/root/app/目录，里面肯定有b, 这时可以访问 /root/test/a/b ，此时你发现b又是/root/test/的hard link 那么b里肯定有a，这时你课可以访问/root/test/a/b/a，以此循环/root/test/a/b/a/b/a/b/a/b/....
> 对于情况b，符号链接不是循环引用，而是相当于跳转，符号链接是一个单独的文件，逻辑上不出现环路。
> 个人理解：
> ![](https://i.loli.net/2021/09/29/K5qmfSdnNGx2eyz.png)

#### utime
更改access和modification时间
#### 目录的创建和销毁
+ mkdir():系统调用
+ rmdir():系统调用，只能删除空目录
#### 切换目录、更改当前工作目录
+ chdir()
+ fchdir()
切换当前进程的工作目录
>假根技术：

+ getcwd():系统调用，获取当前工作路径

#### 读取目录内容
+ glob(3):解析模式或者通配符
```c
#include <stdlib.h>
#include <stdio.h>
#include <glob.h>

#define PAT "/etc/a*.conf"
int main()
{
    glob_t globres;
    int err = glob(PAT,0,NULL,&globres);
    if(err)
    {
        fprintf(stderr,"Error");
    }
    for(int i = 0;i<globres.gl_pathc;i++)
    {
        puts(globres.gl_pathv[i]);
    }

    globfree(&globres);
    exit(0);
}
```
+ opendir(3)：返回一个目录的流指针，该指针指向当前目录的第一个词条(`the first entry in the directory`)
+ closedir(3)
+ readdir(3)：返回的数据是放在静态区的，记录了下一个目录条目的偏移。
使用示例：
```c
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>

#define PATH "/etc"
int main()
{
    DIR* dp = opendir(PATH);
    if(dp == NULL)
    {
    	//会设置errno
        perror("opendir()");
        exit(1);
    }
    while(1)
    {
        struct dirent* cur = readdir(dp);
        if(cur == NULL)
        {
            break; 
        }
        fprintf(stdout,"%s\n",cur->d_name);
    }
    closedir(dp);
}
```
+ rewinddir(3);
+ seekdir(3);
+ telldir(3);

---
** mydu的实现**
```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glob.h>
#include <string.h>

#define PATHSIZE 1024

static int path_noloop(const char *path) {
    char *pos;
    pos = strrchr(path,'/');
    if (pos == NULL) {
        exit(1);
    }

    if (strcmp(pos+1,".") == 0||strcmp(pos+1,"..")== 0) {
        return 0;
    }
    return 1;
}

static int64_t mydu(const char *path) {
	//尽量缩小递归中局部变量的数量以节约栈的资源
	//跨递归点使用的以局部变量实现，仅出现在递归点之前或之后的可放在静态区实现
    static struct stat statres;
    static char nextpath[PATHSIZE];
    glob_t globres;
    int64_t sum = 0;

    //非目录
    if (lstat(path,&statres) < 0) {
        perror("lstat()");
        exit(1);
    }

    if (!S_ISDIR(statres.st_mode)){
        fprintf(stdout,"%ld\t%s\n",statres.st_blocks / 2,path);
        return statres.st_blocks;
    }
    //目录
    //拼接路径
    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/*",PATHSIZE);
    if (glob(nextpath,0,NULL,&globres) < 0) {
        fprintf(stderr,"glob()");
        exit(1);
    }

    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/.*",PATHSIZE);
    if (glob(nextpath,GLOB_APPEND,NULL,&globres) < 0) {
        fprintf(stderr,"glob()");
        exit(1);
    }

    sum = statres.st_blocks;
    for (int i = 0;i < globres.gl_pathc;i++){
        if (path_noloop(globres.gl_pathv[i]))
            sum += mydu(globres.gl_pathv[i]);
    }
    
    globfree(&globres);//回收资源
    return sum;
        
}

int main(int argc,char **argv)
{   
    if (argc < 2) {
        fprintf(stderr,"%s\n","Usage...");
        exit(1);
    }
    
    printf("%ld\t%s\n",mydu(argv[1])/2,argv[1]);

    return 0;
}
```
### 系统数据文件和信息
####  /etc/passwd
各个平台实现不一样，所以不能直接打开这个文件区查询相关信息；
- getpwname(3)
- getpwuid(3)
> 根据name或uid查询用户信息

```c
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pwd.h>

int main(int argc,char** agrv)
{
    if(argc<2)
    {
        fprintf(stderr,"Usage...");
        exit(1);
    }

    struct passwd* pRes = getpwuid(atoi(agrv[1]));
    puts(pRes->pw_name);
    exit(0);
}
```
#### /etc/group
- getgrgid(3)
- getgrnam(3)
![](https://i.loli.net/2021/10/08/b4rOBwQxMhSvokz.png)
#### /etc/shadow
>哈希是用来混淆的而不是加密，原因在于哈希不可逆，并且如果原串一样，哈希之后的串也一样，存在风险

- getspnam(3)
![](https://i.loli.net/2021/10/08/mjnwhG98Y5zbugV.png)

- crypt(3):加密


#### 时间戳
![](https://i.loli.net/2021/10/08/v9Qo642ldehm3yN.png)
输出时间戳到文件中

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BUFSIZE 1024

int main()
{

    char fmttime[BUFSIZ];
    int count = 0;

    FILE *fptr = fopen("./log","a+");
    if (fptr == NULL) {
        perror("fopen()");
        exit(1);
    }

    char buf[BUFSIZE];

    while(fgets(buf,BUFSIZE,fptr) != NULL){
        count++;
    }

    char res[BUFSIZE];

    while (1){
        time_t stamp;
        stamp = time(NULL);

        struct tm *struct_tm;
        struct_tm = localtime(&stamp);

        strftime(fmttime,BUFSIZE,"%Y-%m-%d %H:%M:%S",struct_tm);
        fprintf(fptr,"%d %s\n",++count,fmttime);
        fflush(fptr);//此时`fprintf`是全缓冲的，故需要每写一行就刷新一次
        sleep(1);
    }

    fclose(fptr);

    exit(0);
}
```

使用`mktime`的特性完成时间的跳转
```c
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define TIMESTRSIZE 1024
int main()
{
    time_t timeStamp = time(NULL);

    struct tm* tm = localtime(&timeStamp);
    tm->tm_mday+=100;

    //mktime会自动调整不合理的时间，比如天数溢出会发生进位等
    (void)mktime(tm);
    char timestr[TIMESTRSIZE];
    strftime(timestr,TIMESTRSIZE,"100days later:%Y - %m - %d",tm);
    puts(timestr);
    exit(1);
}
```
### 进程环境
#### main()函数
#### 进程的终止
- 正常终止
  1.从main函数返回
  2.调用exit
  3.调用\_exit或\_Exit
  4.最后一个线程从其启动例程返回
  5.最后一个线程调用pthread_exit
- 异常终止
  1.调用abort
  2.接到一个信号并终止
  3.最后一个线程对其取消请求做出响应 


---
![20211009193845.png](https://i.loli.net/2021/10/09/KSXEyARoOdVJmFc.png)
```c
//atexit():钩子函数，在进程正常终止时会被调用，以当初注册时的逆序调用
#include <stdio.h>
#include <stdlib.h>

static void f1(void) {
puts("f1()");
}
static void f2(void) {
puts("f2()");
}
static void f3(void) {
puts("f3()");
}
int main()
{
puts("start!");
atexit(f1);
atexit(f2);
atexit(f3);
puts("end!");
exit(0);
}
```
>终端打印：
>start!
>end!
>f3()
>f2()
>f1()

---

---
![](https://i.loli.net/2021/10/09/sla2DBcZXfgNpGt.png)
调用`exit`会执行一系列操作后，再进入内核调用`_exit`,而`_exit`则直接结束进程，下图中如果一直`func`只会返回0,1,2，而`switch`却跳转到了`default`。则可能出现了脏数据比如越界写，导致一些错误，若此时调用`exit`，会导致错误扩散，比如文件写入脏数据等，故这里应该使用`_exit`，或者调用`abort`
![](https://i.loli.net/2021/10/09/btNWVknTASosFjM.png)

---


#### 命令行参数的分析
- getopt(3)
- getopt_long(3)

#### 环境变量
`environ`

#### c程序的存储空间布局
#### 库
#### 函数之间的跳转
#### 资源的获取和控制