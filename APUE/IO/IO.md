# **Liunx_C系统编程**




## **I/O**
>I/O是一切实现的基础  

### **两类I/O**
+ stdio:标准I/O
  
  >标准I/O依赖系统调用实现，但是对于不同的系统环境提供的不同系统调用，标准I/O进行统一，移植性好，且提供buff/cache提高速度，以及合并系统调用
+ sysio:系统调用I/O
  >kernel提供的系统调用，不同的平台提供的可能不同

  >errno线程私有化：

### **stdio**
FILE类型贯穿始终
+ fopen()

+ fclose()

+ fgetc()

+ fputc()

+ fgets()

+ fputs()

  > puts()函数用来向标准输出设备（屏幕）输出字符串并换行，具体为：把字符串输出到标准输出设备，将'\0'转换为回车换行。 其调用方式为，puts(s)；其中s为字符串字符（字符串数组名或字符串指针）。

+ fread()

+ fwrite()

+ prinf()

+ scanf()

+ fseek()

+ ftell()
  + 返回long，存在弊端
  + fseeko(),ftello()返回自定义的off_t，但不支持c99、c98标准
  
+ getline()

+ 临时文件
  > 不冲突的创建临时文件
  > 及时销毁(磁盘空间，冲突几率加大)
  + tmpnam()：并发时可能返回同样的名字,即拿到名字后还没来的及创建文件,后一个就调用tmpnam()
  + tmpfile():产生匿名文件(即不会冲突),占用磁盘返回FILE*,但不在目录中显示,即创建该文件后立刻unlink,取消硬连接,该文件会在文件关闭时自动销毁,而关闭的过程可以显示进行，也可以进程终止时自动进行

+ fflush()
	行缓冲：
    全缓冲：
    无缓冲：
	
	>标准的I/O函数（如fread,fwrite）会在内存建立缓冲，该函数刷新内存缓冲，将内容写入内核缓冲，要想将其写入磁盘，还需要调用fsync（先调用fflsuh然后再调用fsync，否则不起作用）；


### **sysio/文件IO**

#### 文件IO操作
+ 文件描述符`(fd/filedescriptor)`
  + 文件描述符实现原理：
  ![原理](https://i.loli.net/2021/09/26/c87rqe4luZRhjL2.png)
  0,1,2默认是标准输入、标准输出、标准错误，fd相当于`File dw scriptor table`中的下标，新打开的文件会使用可使用的下标中的最小的。
  [Linux文件描述符](https://www.cnblogs.com/zhangmingda/p/11715113.html)
  + 两种情景
    1. 两个`fd`指向`open File table`中的不同项，但指向同一个`inode`
    2. 两个`fd`指向`open file table`中相同项，关闭一个`fd`，另一个不受影响，有引用计数
+ open()
   `int open(const char *pathname, int flags)`其中`flag`是一个位图，权限信息按位与得到。

+ close()
+ read()
+ write()
+ lseek()

---
#### 区别文件IO和标准IO  
标准IO：缓冲，合并系统调用，在用户空间有缓存。
文件IO响应速度快
标准IO吞吐量大
转换：fileno()、fdopen()互相获取

不要混用系统IO和标准IO，标准IO有缓冲机制，与系统IO中的实际磁盘文件有出入
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    fputc('a',stdout);
    write(1,"b",1);

    fputc('a',stdout);
    write(1,"b",1);

    fputc('a',stdout);
    write(1,"b",1);
    exit(0);
}
```
>终端输出：bbbaaa    

这是由于fputc()是标准IO，有缓冲机制，用strace命令查看这个程序的系统调用情况：
```cpp
write(1, "b", 1b)                        = 1
write(1, "b", 1b)                        = 1
write(1, "b", 1b)                        = 1
write(1, "aaa", 3aaa)                    = 3
exit_group(0)                            = ?
```

可以看出，三次fputc()被合并为一次系统调用write()；

---

#### IO的效率问题 

#### 文件共享


#### 原子操作
	不可分割的操作，解决竞争和冲突
---

#### 程序中的重定向
+ dup()/dup2()
	`dup()`返回传入`fd`的副本，两个文件描述符指向同一个打开文件，如图中模式
	![](https://i.loli.net/2021/09/28/jzaRkxCwoOQUWlT.png)
	dup2原子操作：例如想要重用标准输出（默认fd=1）是之用来表示fd_temp的文件时，则需要close(1)，然后在dup(fd_temp)，这样的两步操作存在竞争问题，可能刚close()，另一线程要了一个fd，此时另一线程拿到的是最小的1，造成本线程dup()时返回的不是1.则可以采用dup2()
	
```c
  dup2(fd_temp,1);
```

---

#### 同步
>两个缓冲区：内核缓冲区，用户空间缓冲区
>Linux实现中在内核设有缓冲区高速缓存或页面高速缓存，大多数磁盘I/O都通过缓冲区进行。当我们向文件写数据时，内核通常先将数据复制到一个缓冲区中，如果该缓冲区尚未写满，则并不将其排入输出队列，而是等待写满或者内核需要重用该缓冲区以便存放其他数据时，才会将该缓冲区排入输出队列，然后等它到达队首时，才进行实际的I/O操作。这就是被称为 延迟写的输出方式。
>延迟写减少了磁盘读次数，但是却减低了文件内容跟新的速度。当系统发生故障时，延迟写的方式可能造成文件跟新丢失。为了应对此种情况，linux下提供了sysnc,fsync,fdatasync三个函数来保证实际文件系统与缓冲区中内容的一致。

+ sync()
	 该函数只是将所有修改过的块缓冲区排入写队列，然后就返回，他并不等待实际写磁盘操作结束

+ fsync()
	只对由文件描述符fd指定的一个文件起作用，并且等待写磁盘操作结束才返回。
	
+ fdatasync()
	 类似于fsync，但是它只影响文件的数据部分。而除数据外，fsync还会同步更新文件的属性。只刷新有效数据，不刷新亚数据

+ fcntl():文件相关
+ ioctl():设备相关

#### /dev/fd/
虚目录显示的是当前进程的文件描述符信息

-------------------------------