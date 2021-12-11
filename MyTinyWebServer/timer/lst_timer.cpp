/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-09 13:03:06
 */
#include "lst_timer.h"


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