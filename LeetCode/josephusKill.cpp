/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-10 16:07:45
 */
#include<iostream>
struct node
{
    int val;
    node* next;

    node():val(-1),next(nullptr)
    {}
    node(int a):val(a),next(nullptr)
    {}
};

node* josephusKill(node* phead,int count)
{
    node* cur = phead;
    node* pre = nullptr;
    while (cur->next != phead)
    {
        pre = cur;
        cur = cur->next;
    }
    int m = 0;
    cur = phead;
    while(cur != pre)
    {
        m++;
        if(m == count)
        {
            pre->next = cur->next;
            cur=cur->next;
            m = 0;
        }
        else
        {
            pre = pre->next;
            cur = cur->next;
        }
    }
    return cur;
}

int main()
{
    node* a = new node(1);
    a->next = new node(2);
    a->next->next = new node(3);
    a->next->next->next = new node(4);
    a->next->next->next->next = new node(5);
    a->next->next->next->next->next =a;
    node* p = josephusKill(a,2);
    std::cout<<p->val<<std::endl;
    if(p->next != p)
        std::cout<<"出错"<<std::endl;

}