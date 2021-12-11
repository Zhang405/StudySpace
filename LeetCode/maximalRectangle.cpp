/*
 * @Description: 输入矩阵，由0/1组成，求全1的矩形最大面积
 * @Author: XZMJX
 * @Date: 2021-12-07 20:09:23
 */
#include<stack>
#include<vector>
#include<algorithm>
using namespace std;
//切片叠起来，转变为求直方图最大面积，借助单调栈
int maximalRectangle(vector<vector<int>>& matrix)
{
    int m = matrix.size();
    int n = matrix[0].size();
    vector<int> v(n,0);
    int max = 0;
    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
           v[j] = matrix[i][j] == 0?0:v[j]+1;
        }
        max = std::max(maxRecFromBottom(v),max);
    }
    return max;
}

/**
 * @Description: m构成直方图，求直方图的最大矩形面积
 * @Author: XZMJX
 * @Date: 2021-12-07 20:21:06
 * @param {vector<int>} m
 * @return {*}
 */
int maxRecFromBottom(vector<int> m)
{
    stack<int> help;
    int max = 0;
    int size = m.size();
    for(int i = 0; i < size; ++i)
    {
        while(!help.empty()&&m[help.top()]>=m[i])
        {
            int j = help.top();
            help.pop();
            int k = help.empty()?-1:help.top();
            //从k到i-1
            int curArea = (i-1-k)*m[j];
            max = std::max(max,curArea);
        }
        help.push(i);
    }
     while(!help.empty())
    {
        int j = help.top();
        help.pop();
        int k = help.empty()?-1:help.top();
        //从k到size-1,
        int curArea = (size-1-k)*m[j];
        max = std::max(max,curArea);
    }
    return max;
}

int main()
{
    vector<vector<int>> v = {{1,0,1,0,0},{1,0,1,1,1},{1,1,1,1,1},{1,0,0,1,0}};
    maximalRectangle(v);
 }