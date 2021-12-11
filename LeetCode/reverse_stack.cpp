/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-04 19:50:04
 */
/**
 * @Description: 仅用递归和栈操作实现栈的倒置
 * @Author: XZMJX
 * @Date: 2021-12-04 20:10:29
 * @param {stack<int>} s
 * @return {*}
 */
#include<stack>
#include<iostream>

/**
 * @Description: 获取当前栈的最后一个元素并移出它
 * @Author: XZMJX
 * @Date: 2021-12-04 20:10:29
 * @param {stack<int>} s
 * @return {*}
 */
int getAndRemoveLastElement(std::stack<int>& s)
{
    int res = s.top();
    s.pop();
    if(s.empty())
    {
        return res;
    }
    else
    {
        int last = getAndRemoveLastElement(s);
        s.push(res);
        return last;
    }
}

/**
 * @Description: 逆序一个栈，每个递归实例中保存当前输入栈底元素，逆序返回时，推入栈中
 * @Author: XZMJX
 * @Date: 2021-12-04 20:26:00
 * @param {*}
 * @return {*}
 */
void reverse(std::stack<int>& s)
{
    if(s.empty())
        return;
    int i = getAndRemoveLastElement(s);
    reverse(s);
    s.push(i);
}
int main()
{
    std::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    std::cout<<"*************before reverse***************"<<std::endl;
    std::stack<int> s_copy = s;
    while(!s_copy.empty())
    {
        std::cout<<s_copy.top()<<" ";
        s_copy.pop();
    }
    std::cout<<std::endl;
    reverse(s);
     std::cout<<"*************after reverse***************"<<std::endl;
    while(!s.empty())
    {
        std::cout<<s.top()<<" ";
        s.pop();
    }
    std::cout<<std::endl;
    return 0;
}