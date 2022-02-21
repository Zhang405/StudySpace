/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-09 13:03:06
 */
#include "lst_timer.h"
#include "../http/http_conn.h"


xzmjx::TimerList::TimerList()
{
    _M_head = nullptr;
    _M_tail = nullptr;
}
xzmjx::TimerList::~TimerList()
{
    while (_M_head)
    {
        Timer* temp = _M_head->_M_next;
        delete _M_head;
        _M_head = temp;
    }
    
}
void xzmjx::TimerList::AddTimer(Timer* ipTimer)
{
    if(_M_head == nullptr)
    {
        _M_head = _M_tail = ipTimer;
    }
    else
    {
        addTimer(ipTimer,_M_head);
    }
}
void xzmjx::TimerList::AdjustTimer(Timer* ipTimer)
{
    if(ipTimer == nullptr)
        return;
    Timer* temp = ipTimer->_M_next;
    if(temp == nullptr || ipTimer->_M_expire<temp->_M_expire)
    {
        return;
    }
    else if(ipTimer == _M_head)
    {
        _M_head = ipTimer->_M_next;
        _M_head->_M_prev = nullptr;
        ipTimer->_M_next = nullptr;
        addTimer(ipTimer,_M_head);
    }
    else
    {
        Timer* prev = ipTimer->_M_prev;
        prev->_M_next = temp;
        temp->_M_prev = prev;
        ipTimer->_M_next = nullptr;
        ipTimer->_M_prev = nullptr;
        addTimer(ipTimer,_M_head);
    }
}
void xzmjx::TimerList::DeleteTimer(Timer* ipTimer)
{
    if(ipTimer == nullptr)
        return;
    else if(ipTimer == _M_head&&ipTimer==_M_tail)
    {
        _M_tail=_M_head = nullptr;
        delete ipTimer;
    }
    else if(ipTimer == _M_head)
    {
        _M_head=ipTimer->_M_next;
        _M_head->_M_prev = nullptr;
        delete ipTimer;
        ipTimer = nullptr;
    }
    else if(ipTimer == _M_tail)
    {
        _M_tail=ipTimer->_M_prev;
        _M_tail->_M_next = nullptr;
        delete ipTimer;
        ipTimer = nullptr;
    }
    else
    {
        ipTimer->_M_prev->_M_next = ipTimer->_M_next;
        delete ipTimer;
        ipTimer = nullptr;
    }
}
void xzmjx::TimerList::Tick()
{
    if(!_M_head)
        return;
    time_t now = time(NULL);
    Timer* cur = _M_head;
    while(cur)
    {
        if(cur->_M_expire>=now)
        {
            cur->cb_func(cur->_M_data);
            _M_head = cur->_M_next;
            if(_M_head)
            {
                _M_head->_M_prev = nullptr;
            }
            delete cur;
            cur = _M_head;
        }
        else
            break;
    }
}
void xzmjx::TimerList::addTimer(Timer* ipTimer,Timer* iphead)
{
    Timer* pre = iphead;
    Timer* cur = iphead->_M_next;
    while(cur)
    {
        if(ipTimer->_M_expire>cur->_M_expire)
        {
            pre->_M_next = ipTimer;
            ipTimer->_M_prev = pre;
            ipTimer->_M_next = cur;
            cur->_M_prev = ipTimer;
        }
        pre = cur;
        cur = cur->_M_next;
    }
    if(cur == nullptr)
    {
        _M_tail->_M_next = ipTimer;
        ipTimer->_M_prev = _M_tail;
        _M_tail= ipTimer;
        _M_tail->_M_next = nullptr;
    }
}

void xzmjx::Utils::Init(int timeslot)
{
    _M_TIMESLOT = timeslot;
}

//对文件描述符设置非阻塞
int xzmjx::Utils::Setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

//将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
void xzmjx::Utils::Addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
{
    epoll_event event;
    event.data.fd = fd;

    if (1 == TRIGMode)
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else
        event.events = EPOLLIN | EPOLLRDHUP;

    if (one_shot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    Setnonblocking(fd);
}

//信号处理函数
void xzmjx::Utils::sig_handler(int sig)
{
    //为保证函数的可重入性，保留原来的errno
    int save_errno = errno;
    int msg = sig;
    send(u_pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

//设置信号函数
void xzmjx::Utils::Addsig(int sig, void(handler)(int), bool restart)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if (restart)
        sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

//定时处理任务，重新定时以不断触发SIGALRM信号
void xzmjx::Utils::timer_handler()
{
    _M_timer_lst.Tick();
    alarm(_M_TIMESLOT);
}

void xzmjx::Utils::ShowError(int connfd, const char *info)
{
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int *xzmjx::Utils::u_pipefd = 0;
int xzmjx::Utils::u_epollfd = 0;

class Utils;
void xzmjx::cb_func(client_data *user_data)
{
    epoll_ctl(Utils::u_epollfd, EPOLL_CTL_DEL, user_data->st_sockfd, 0);
    assert(user_data);
    close(user_data->st_sockfd);
    xzmjx::HttpConn::m_user_count--;
}