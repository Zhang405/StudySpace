/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-09 13:02:48
 */
#pragma once
#include <netinet/in.h>
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

    
    
}