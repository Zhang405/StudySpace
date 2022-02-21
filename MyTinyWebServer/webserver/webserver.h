#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>

#include "../threadpool/threadpool.h"
#include "../http/http_conn.h"
#include"../timer/lst_timer.h"


const int MAX_FD = 65536;           
const int MAX_EVENT_NUMBER = 10000; 
const int TIMESLOT = 5;             

namespace xzmjx{
    class WebServer
    {
    public:
        WebServer();
        ~WebServer();

        void Init(int port , std::string user, std::string passWord, std::string databaseName,
                int log_write , int opt_linger, int trigmode, int sql_num,
                int thread_num, int close_log, int actor_model);

        void ThreadPool();
        void SqlPool();
        void LogWrite();
        void TrigMode();
        void EventListen();
        void EventLoop();
        void Timer(int connfd, struct sockaddr_in client_address);
        void AdjustTimer(xzmjx::Timer *timer);
        void DealTimer(xzmjx::Timer *timer, int sockfd);
        bool DealClient();
        bool DealSignal(bool& timeout, bool& stop_server);
        void DealRead(int sockfd);
        void DealWrite(int sockfd);

    public:
        int _M_port;
        char *_M_root;
        int _M_log_write;
        int _M_close_log;
        int _M_actormodel;
        int _M_pipefd[2];
        int _M_epollfd;
        HttpConn *_M_users;
        SQLConnPool *_M_connPool;
        std::string _M_user;         
        std::string _M_passWord;    
        std::string _M_databaseName; 
        int _M_sql_num;
        threadpool<HttpConn> *_M_pool;
        int _M_thread_num;
        epoll_event _M_events[MAX_EVENT_NUMBER];
        int _M_listenfd;
        int _M_OPT_LINGER;
        int _M_TRIGMode;
        int _M_LISTENTrigmode;
        int _M_CONNTrigmode;
        xzmjx::client_data *_M_users_timer;
        xzmjx::Utils _M_utils;
    };
    
}
