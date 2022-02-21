#include "webserver.h"
#include <string.h>
#include "../log/log.h"
xzmjx::WebServer::WebServer()
{
    _M_users = new HttpConn[MAX_FD];

    //root文件夹路径
    char server_path[200];
    getcwd(server_path, 200);
    char root[6] = "/root";
    _M_root = (char *)malloc(strlen(server_path) + strlen(root) + 1);
    strcpy(_M_root, server_path);
    strcat(_M_root, root);

    //定时器,直接用下标索引客户端数据
    _M_users_timer = new client_data[MAX_FD];
}

xzmjx::WebServer::~WebServer()
{
    close(_M_epollfd);
    close(_M_listenfd);
    close(_M_pipefd[1]);
    close(_M_pipefd[0]);
    delete[] _M_users;
    delete[] _M_users_timer;
    delete _M_pool;
}

void xzmjx::WebServer::Init(int port, std::string user, std::string passWord, std::string databaseName, int log_write, 
                     int opt_linger, int trigmode, int sql_num, int thread_num, int close_log, int actor_model)
{
    _M_port = port;
    _M_user = user;
    _M_passWord = passWord;
    _M_databaseName = databaseName;
    _M_sql_num = sql_num;
    _M_thread_num = thread_num;
    _M_log_write = log_write;
    _M_OPT_LINGER = opt_linger;
    _M_TRIGMode = trigmode;
    _M_close_log = close_log;
    _M_actormodel = actor_model;
}

void xzmjx::WebServer::TrigMode()
{
    //LT + LT
    if (0 == _M_TRIGMode)
    {
        _M_LISTENTrigmode = 0;
        _M_CONNTrigmode = 0;
    }
    //LT + ET
    else if (1 == _M_TRIGMode)
    {
        _M_LISTENTrigmode = 0;
        _M_CONNTrigmode = 1;
    }
    //ET + LT
    else if (2 == _M_TRIGMode)
    {
        _M_LISTENTrigmode = 1;
        _M_CONNTrigmode = 0;
    }
    //ET + ET
    else if (3 == _M_TRIGMode)
    {
        _M_LISTENTrigmode = 1;
        _M_CONNTrigmode = 1;
    }
}

void xzmjx::WebServer::LogWrite()
{
    if (0 == _M_close_log)
    {
        //初始化日志，分同步异步写入
        if (1 == _M_log_write)
            Log::GetInstance()->Init("./ServerLog", _M_close_log, 2000, 800000, 800);
        else
            Log::GetInstance()->Init("./ServerLog", _M_close_log, 2000, 800000, 0);
    }
}

void xzmjx::WebServer::SqlPool()
{
    _M_connPool = SQLConnPool::GetConnPool();
    _M_connPool->Init("localhost", _M_user, _M_passWord, _M_databaseName, 3306, _M_sql_num, _M_close_log);
    _M_users->IniMySQLResult(_M_connPool);
}

void xzmjx::WebServer::ThreadPool()
{
    _M_pool = new threadpool<HttpConn>(_M_actormodel, _M_connPool, _M_thread_num);
}

void xzmjx::WebServer::EventListen()
{
    _M_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(_M_listenfd >= 0);
    if (0 == _M_OPT_LINGER)
    {
        struct linger tmp = {0, 1};
        setsockopt(_M_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }
    else if (1 == _M_OPT_LINGER)
    {
        struct linger tmp = {1, 1};
        setsockopt(_M_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
    }

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(_M_port);

    int flag = 1;
    setsockopt(_M_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    ret = bind(_M_listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret >= 0);
    ret = listen(_M_listenfd, 5);
    assert(ret >= 0);
    
    //设置定时器时间间隔
    _M_utils.Init(TIMESLOT);

    epoll_event events[MAX_EVENT_NUMBER];
    _M_epollfd = epoll_create(5);
    assert(_M_epollfd != -1);

    _M_utils.Addfd(_M_epollfd, _M_listenfd, false, _M_LISTENTrigmode);
    HttpConn::m_epollfd = _M_epollfd;

    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, _M_pipefd);
    assert(ret != -1);
    _M_utils.Setnonblocking(_M_pipefd[1]);
    _M_utils.Addfd(_M_epollfd, _M_pipefd[0], false, 0);

    _M_utils.Addsig(SIGPIPE, SIG_IGN);
    _M_utils.Addsig(SIGALRM, _M_utils.sig_handler, false);
    _M_utils.Addsig(SIGTERM, _M_utils.sig_handler, false);

    alarm(TIMESLOT);

    //工具类,信号和描述符基础操作
    Utils::u_pipefd = _M_pipefd;
    Utils::u_epollfd = _M_epollfd;
}

void xzmjx::WebServer::Timer(int connfd, struct sockaddr_in client_address)
{
    _M_users[connfd].Init(connfd, client_address, _M_root, _M_CONNTrigmode, _M_close_log, _M_user, _M_passWord, _M_databaseName);

    //初始化client_data数据
    //创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
    _M_users_timer[connfd].st_sockaddr = client_address;
    _M_users_timer[connfd].st_sockfd = connfd;
    xzmjx::Timer *timer = new xzmjx::Timer;
    timer->_M_data = &_M_users_timer[connfd];
    timer->cb_func = cb_func;
    time_t cur = time(NULL);
    timer->_M_expire = cur + 3 * TIMESLOT;
    _M_users_timer[connfd].st_pTimer = timer;
    _M_utils._M_timer_lst.AddTimer(timer);
}

//定时保活
void xzmjx::WebServer::AdjustTimer(xzmjx::Timer *timer)
{
    time_t cur = time(NULL);
    timer->_M_expire = cur + 3 * TIMESLOT;
    _M_utils._M_timer_lst.AdjustTimer(timer);

    LOG_INFO("%s", "adjust timer once");
}

void xzmjx::WebServer::DealTimer(xzmjx::Timer *timer, int sockfd)
{
    timer->cb_func(&_M_users_timer[sockfd]);
    if (timer)
    {
        _M_utils._M_timer_lst.DeleteTimer(timer);
    }

    LOG_INFO("close fd %d", _M_users_timer[sockfd].st_sockfd);
}

bool xzmjx::WebServer::DealClient()
{
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    if (0 == _M_LISTENTrigmode)
    {
        int connfd = accept(_M_listenfd, (struct sockaddr *)&client_address, &client_addrlength);
        if (connfd < 0)
        {
            LOG_ERROR("%s:errno is:%d", "accept error", errno);
            return false;
        }
        if (HttpConn::m_user_count >= MAX_FD)
        {
            _M_utils.ShowError(connfd, "Internal server busy");
            LOG_ERROR("%s", "Internal server busy");
            return false;
        }
        Timer(connfd, client_address);
    }

    else
    {
        while (1)
        {
            int connfd = accept(_M_listenfd, (struct sockaddr *)&client_address, &client_addrlength);
            if (connfd < 0)
            {
                LOG_ERROR("%s:errno is:%d", "accept error", errno);
                break;
            }
            if (HttpConn::m_user_count >= MAX_FD)
            {
                _M_utils.ShowError(connfd, "Internal server busy");
                LOG_ERROR("%s", "Internal server busy");
                break;
            }
            Timer(connfd, client_address);
        }
        return false;
    }
    return true;
}

bool xzmjx::WebServer::DealSignal(bool &timeout, bool &stop_server)
{
    int ret = 0;
    int sig;
    char signals[1024];
    ret = recv(_M_pipefd[0], signals, sizeof(signals), 0);
    if (ret == -1)
    {
        return false;
    }
    else if (ret == 0)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < ret; ++i)
        {
            switch (signals[i])
            {
            case SIGALRM:
            {
                timeout = true;
                break;
            }
            case SIGTERM:
            {
                stop_server = true;
                break;
            }
            }
        }
    }
    return true;
}

void xzmjx::WebServer::DealRead(int sockfd)
{
    xzmjx::Timer *timer = _M_users_timer[sockfd].st_pTimer;

    if (1 == _M_actormodel)
    {
        if (timer)
        {
            AdjustTimer(timer);
        }
        _M_pool->append(_M_users + sockfd, 0);

        while (true)
        {
            if (1 == _M_users[sockfd].improv)
            {
                if (1 == _M_users[sockfd].timer_flag)
                {
                    DealTimer(timer, sockfd);
                    _M_users[sockfd].timer_flag = 0;
                }
                _M_users[sockfd].improv = 0;
                break;
            }
        }
    }
    else
    {
        if (_M_users[sockfd].ReadOnce())
        {
            LOG_INFO("deal with the client(%s)", inet_ntoa(_M_users[sockfd].GetAddr()->sin_addr));
            _M_pool->append_p(_M_users + sockfd);

            if (timer)
            {
                AdjustTimer(timer);
            }
        }
        else
        {
            DealTimer(timer, sockfd);
        }
    }
}

void xzmjx::WebServer::DealWrite(int sockfd)
{
    xzmjx::Timer *timer = _M_users_timer[sockfd].st_pTimer;
    //reactor
    if (1 == _M_actormodel)
    {
        if (timer)
        {
            AdjustTimer(timer);
        }

        _M_pool->append(_M_users + sockfd, 1);

        while (true)
        {
            if (1 == _M_users[sockfd].improv)
            {
                if (1 == _M_users[sockfd].timer_flag)
                {
                    DealTimer(timer, sockfd);
                    _M_users[sockfd].timer_flag = 0;
                }
                _M_users[sockfd].improv = 0;
                break;
            }
        }
    }
    else
    {
        //proactor
        if (_M_users[sockfd].Write())
        {
            LOG_INFO("send data to the client(%s)", inet_ntoa(_M_users[sockfd].GetAddr()->sin_addr));

            if (timer)
            {
                AdjustTimer(timer);
            }
        }
        else
        {
            DealTimer(timer, sockfd);
        }
    }
}

void xzmjx::WebServer::EventLoop()
{
    bool timeout = false;
    bool stop_server = false;
    //事件循环
    while (!stop_server)
    {
        int number = epoll_wait(_M_epollfd, _M_events, MAX_EVENT_NUMBER, -1);
        if (number < 0 && errno != EINTR)
        {
            LOG_ERROR("%s", "epoll failure");
            break;
        }

        for (int i = 0; i < number; i++)
        {
            int sockfd = _M_events[i].data.fd;

            if (sockfd == _M_listenfd)
            {
                bool flag = DealClient();
                if (false == flag)
                    continue;
            }
            else if (_M_events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                //移除连接
                xzmjx::Timer *timer = _M_users_timer[sockfd].st_pTimer;
                DealTimer(timer, sockfd);
            }
            //统一事件源
            else if ((sockfd == _M_pipefd[0]) && (_M_events[i].events & EPOLLIN))
            {
                bool flag = DealSignal(timeout, stop_server);
                if (false == flag)
                    LOG_ERROR("%s", "dealclientdata failure");
            }
            else if (_M_events[i].events & EPOLLIN)
            {
                DealRead(sockfd);
            }
            else if (_M_events[i].events & EPOLLOUT)
            {
                DealWrite(sockfd);
            }
        }
        if (timeout)
        {
            _M_utils.timer_handler();

            LOG_INFO("%s", "timer tick");

            timeout = false;
        }
    }
}