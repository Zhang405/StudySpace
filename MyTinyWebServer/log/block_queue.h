/*
 * @Description: 将生产者、消费者模型封装为一个循环阻塞队列(循环队列由数组实现)
 * @Author: XZMJX
 * @Date: 2021-12-06 20:46:12
 */
#pragma once
#include"../lock/locker.h"
#include<time.h>
namespace xzmjx
{
    template <typename T>
    class BlockQueue
    {
    public:
        BlockQueue(int max_size = 1000)
        {
            if(max_size<0)
            {
                throw std::exception();
            }
            _M_array = new T[max_size];
            _M_size = 0;
            _M_max_size = max_size;
            _M_front = -1;
            _M_back = -1;
        }
        ~BlockQueue()
        {
            _M_mutex.Lock();
            if(_M_array != nullptr)
            {
                delete[] _M_array;
                _M_array = nullptr;
                _M_size = 0;
                _M_max_size = 1000;
                _M_front = -1;
                _M_back = -1;
            }
            _M_mutex.UnLock();
        }
        /**
         * @Description: 清空缓冲队列
         * @Author: XZMJX
         * @Date: 2021-12-06 21:48:05
         * @param {*}
         * @return {*}
         */
        void Clear()
        {
            _M_mutex.Lock();
            _M_size  =  0;
            _M_front = -1;
            _M_back  = -1;
            _M_mutex.UnLock();
        }
        /**
         * @Description: 判断当前队列是否已满
         * @Author: XZMJX
         * @Date: 2021-12-06 21:48:22
         * @param {*}
         * @return {*}
         */
        bool Full()
        {
            bool f = false;
            _M_mutex.Lock();
            if(_M_size >= _M_max_size)
                f = true;
            _M_mutex.UnLock();
            return f;
        }
        /**
         * @Description: 判断当前队列是否为空
         * @Author: XZMJX
         * @Date: 2021-12-06 21:51:44
         * @param {*}
         * @return {*}
         */
        bool Empty()
        {
            bool f = false;
            _M_mutex.Lock();
            if(0 == _M_size)
                f = true;
            _M_mutex.UnLock();
            return f;
        }
        /**
         * @Description: 返回当前队首元素，若当前队列size == 0，返回false
         * @Author: XZMJX
         * @Date: 2021-12-06 21:51:58
         * @param {T&} val
         * @return {*}
         */
        bool Front(T& val)
        {
            bool f = true;
            _M_mutex.Lock();
            if(0 == _M_size)
                f = false;
            val = _M_array[_M_front];
            _M_mutex.UnLock();
            return f;
        }
        /**
         * @Description: 返回当前队列队尾元素，若当前队列size == 0，返回false
         * @Author: XZMJX
         * @Date: 2021-12-06 21:52:45
         * @param {T&} val
         * @return {*}
         */
        bool Back(T& val)
        {
            bool f = true;
            _M_mutex.Lock();
            if(0 == _M_size)
                f = false;
            val = _M_array[_M_back];
            _M_mutex.UnLock();
            return f;
        }
        /**
         * @Description: 返回当前缓冲队列item个数
         * @Author: XZMJX
         * @Date: 2021-12-06 21:55:06
         * @param {*}
         * @return {*}
         */
        int Size()
        {
            int size = 0;
            _M_mutex.Lock();
            size = _M_size;
            _M_mutex.UnLock();
            return size;
        }

        /**
         * @Description: 返回缓冲队列最大容量
         * @Author: XZMJX
         * @Date: 2021-12-06 21:55:25
         * @param {*}
         * @return {*}
         */
        int MaxSize()
        {
            int max_size = 0;
            _M_mutex.Lock();
            max_size = _M_max_size;
            _M_mutex.UnLock();
            return max_size;
        }

        /**
         * @Description: 向缓冲队列加入item
         * @Author: XZMJX
         * @Date: 2021-12-06 21:55:49
         * @param {T&} item
         * @return {*}
         */
        bool Push(const T& item)
        {
            bool f = true;
            _M_mutex.Lock();
            if(_M_size>=_M_max_size)
            {
                f = false;
            }
            else
            {
                _M_back = (_M_back+1)%_M_max_size;
                _M_array[_M_back] = item;
                ++_M_size;
            }
            //缓冲队列有值，若有等待的消费者，则唤醒
            _M_cond.Broadcast();
            _M_mutex.UnLock();
            return f;
        }
        /**
         * @Description: 从缓冲队列取item
         * @Author: XZMJX
         * @Date: 2021-12-06 21:56:04
         * @param {T&} item
         * @return {*}
         */
        bool Pop(T& item)
        {
            bool f = true;
            _M_mutex.Lock();
            //while(...)防止伪唤醒
            while(_M_size <=0)
            {
                //缓冲队列没有值，阻塞等
                f = _M_cond.Wait(_M_mutex.GetMutex());
            }
            if(f)
            {
                //_M_front初始为-1，实际位置是当前队列第一个元素的前一个位置，故先加一后取
                _M_front = (_M_front+1)%_M_max_size;
                item = _M_array[_M_front];
                --_M_size;
            }
            _M_mutex.UnLock();
            return f;
        }
        /**
         * @Description: 同上，加入超时处理
         * @Author: XZMJX
         * @Date: 2021-12-06 21:56:17
         * @param {T&} item
         * @param {int} ms_timeout
         * @return {*}
         */
        bool Pop(T& item,int ms_timeout)
        {
            bool f = true;
            struct timespec t;
            clock_gettime(CLOCK_REALTIME,&t);
            _M_mutex.Lock();
            if(_M_size<=0)
            {
                t.tv_sec += ms_timeout/1000;
                t.tv_nsec+=((ms_timeout%1000)*1000);
                f = _M_cond.TimeWait(_M_mutex.GetMutex(),t);
            }
            if(_M_size<=0)
            {
                f = false;
            }
            if(f)
            {
                _M_front = (_M_front+1)%_M_max_size;
                item = _M_array[_M_front];
                --_M_size;
            }
            _M_mutex.UnLock();
            return f;
        }
    private:
        xzmjx::Locker _M_mutex;     
        xzmjx::Cond   _M_cond;      //用于生产者消费者之间的同步

        T*            _M_array;     //缓冲区
        int           _M_size;      //当前缓冲区元素个数 
        int           _M_max_size;  //缓冲区最大容量
        int           _M_front;     //头部，消费者取资源的前一个位置
        int           _M_back;      //尾部，生产者放资源的前一个位置
    };
}