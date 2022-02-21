/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-09 13:02:48
 */
#pragma once
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>

#include <time.h>
namespace xzmjx
{
    class Timer;
    struct client_data
    {
        sockaddr_in st_sockaddr;
        int st_sockfd;
        Timer* st_pTimer;
    };
    

    class Timer
    {
    public:
        Timer():_M_prev(nullptr),_M_next(nullptr)
        {

        }
        ~Timer() = default;
    public:
        Timer* _M_prev;
        Timer* _M_next;
        time_t _M_expire;
        client_data* _M_data;
        void(*cb_func)(client_data*);
    };
    
    class TimerList
    {
    public:
        TimerList();
        ~TimerList();

        void AddTimer(Timer* ipTimer);
        void AdjustTimer(Timer* ipTimer);
        void DeleteTimer(Timer* ipTimer);
        void Tick();

    private:
        void addTimer(Timer* ipTimer,Timer* ipHead);
    private:
        Timer* _M_head;
        Timer* _M_tail;

    };

    class Utils
    {
    public:
        Utils() {}
        ~Utils() {}

        void Init(int timeslot);

        //对文件描述符设置非阻塞
        int Setnonblocking(int fd);

        //将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
        void Addfd(int epollfd, int fd, bool one_shot, int TRIGMode);

        //信号处理函数
        static void sig_handler(int sig);

        //设置信号函数
        void Addsig(int sig, void(handler)(int), bool restart = true);

        //定时处理任务，重新定时以不断触发SIGALRM信号
        void timer_handler();

        void ShowError(int connfd, const char *info);

    public:
        static int *u_pipefd;
        TimerList _M_timer_lst;
        static int u_epollfd;
        int _M_TIMESLOT;
    };

void cb_func(client_data *user_data);
    
}