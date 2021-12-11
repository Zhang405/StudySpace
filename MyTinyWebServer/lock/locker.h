/*
 * @Description: 对posix的互斥锁，条件变量，信号量进行封装，实现RAII以及重复代码复用
 * @Author: XZMJX
 * @Date: 2021-12-05 14:06:18
 */
#pragma once
#include<exception>
#include<pthread.h>
#include<semaphore.h>

namespace xzmjx
{
    class Sem
    {
    public:
        Sem()
        {
            if(0 != sem_init(&_M_sem,0,0))
            {
                throw std::exception();
            }
        }
        Sem(int s)
        {
            if(0 != sem_init(&_M_sem,0,s))
            {
                throw std::exception();
            }
        }
        ~Sem()
        {
            sem_destroy(&_M_sem);
        }
        bool Wait()
        {
            return 0 == sem_wait(&_M_sem);
        }
        bool Post()
        {
            return 0 == sem_post(&_M_sem);  
        }
    private:
        sem_t _M_sem;
    };

    class Locker
    {

    public:
        Locker()
        {
            if(0 != pthread_mutex_init(&_M_mutex,NULL))
            {
                throw std::exception();
            }
        }
        ~Locker()
        {
            pthread_mutex_destroy(&_M_mutex);
        }
        bool Lock()
        {
            return 0 == pthread_mutex_lock(&_M_mutex);
        }
        bool UnLock()
        {
            return 0 == pthread_mutex_unlock(&_M_mutex);
        }

        pthread_mutex_t* GetMutex()
        {
            return &_M_mutex;
        }

    private:
        pthread_mutex_t _M_mutex;
    };    

    class Cond
    {
    public:
        Cond()
        {
            if(0 != pthread_cond_init(&_M_cond,NULL))
            {
                throw std::exception();
            }
        }
        ~Cond()
        {
            pthread_cond_destroy(&_M_cond);
        }

        bool Wait(pthread_mutex_t* mutex)
        {
            int rs;
            rs = pthread_cond_wait(&_M_cond,mutex);
            return rs == 0;
        }
        bool TimeWait(pthread_mutex_t* mutex,struct timespec t)
        {
            int rs;
            rs = pthread_cond_timedwait(&_M_cond,mutex,&t);
            return rs == 0;
        }
        bool Signal()
        {
            return 0 == pthread_cond_signal(&_M_cond);
        }
        bool Broadcast()
        {
            return 0 == pthread_cond_broadcast(&_M_cond);
        }
    private:
        pthread_cond_t _M_cond;
    };
}