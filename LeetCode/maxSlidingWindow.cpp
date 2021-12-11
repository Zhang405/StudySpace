/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-07 10:36:09
 */
#include<iostream>
#include<vector>
#include<deque>
std::vector<int> getMaxValueInSlidingWindow(std::vector<int> in, int size)
{
	std::vector<int> res;
	std::deque<int> maxPosQ;
	for (int i = 0; i < in.size(); ++i)
	{
		if (maxPosQ.empty())
			maxPosQ.push_back(i);
		while (!maxPosQ.empty()&& in[maxPosQ.back()] <= in[i])
		{
			maxPosQ.pop_back();
		}
		maxPosQ.push_back(i);
		if (i - maxPosQ.front() >= size)
		{
			maxPosQ.pop_front();
		}
		if (i >= size-1)
			res.push_back(in[maxPosQ.front()]);
	}
	return res;
}
