#include <iostream>
using namespace std;

void prinf()
{
}

template <typename T,typename... Types>
void prinf(const T& c, const Types&... args)
{
	cout<<c<<endl;
	prinf(args...);
}

template <typename... Types>
void match_test(const Types&... args)
{
	cout<<"This is The Generalized version"<<endl;
}

template <typename... Types>
void match_test(int i,const Types&... args)
{
	cout<<"This is The Specialized version"<<endl;
}



int main()
{
	//prinf('a','b','c','v','f','s');
	match_test(1,2,3,4,5,6,7);
	match_test("test","bala","hello");
	return 0;	
}

