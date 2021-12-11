/*
 * @Description: 最大值减去最小值小于等于num的子数组数量
 * @Author: XZMJX
 * @Date: 2021-12-08 11:30:00
 */
#include<vector>
#include<deque>
#include<iostream>
using namespace std;
int getNum(const vector<int>& arr,int num)
{
    deque<int> qmin;
    deque<int> qmax;
    int res = 0;
    int i = 0;
    int j = 0;
    int size = arr.size();
    while(i < size)
    {
        //以i开头的子数组，找到第一个不符合条件的j即退出，j-i则是以i开头的子数组个数
        while(j < size)
        {
            while(!qmin.empty()&&arr[j]<= arr[qmin.back()])
                qmin.pop_back();
            qmin.push_back(j);
            while(!qmax.empty()&&arr[j]>=arr[qmax.back()])
                qmax.pop_back();
            qmax.push_back(j);
            if(arr[qmax.front()] - arr[qmin.front()] > num)
                break;
            ++j;
        }
        if(qmin.front() == i)
            qmin.pop_front();
        if(qmax.front() == i)
            qmax.pop_front();
        res+=(j-i);
        ++i;
    }
    return res;
}

int main()
{
    vector<int> a = {1,2,3,4,5,6};
    cout<<getNum(a,1)<<endl;
}