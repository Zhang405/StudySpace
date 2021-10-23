#include<string>
#include<iostream>
#include<vector>
#include"LimitedObj.h"

using namespace XZMJX;
using namespace std;

int main()
{
	vector<LimitedObj*> TempObj;
    try
    {
        while (true)
        {
            LimitedObj* p = LimitedObj::CreateObject();
			TempObj.push_back(p);
            cout<<p->Object_Count()<<endl;
        }
    }
    catch(const LimitedObj::TooManyObject& e)
    {
		for (int i = 0; i != TempObj.size(); i++)
		{
			delete TempObj[i];
			TempObj[i] = nullptr; 
		}
        cout<<"MaxNum"<<endl;
		getchar();
        exit(1);
    }
    exit(0);
}