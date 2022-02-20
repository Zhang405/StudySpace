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

template <typename T, typename... Types>
void match_test(const T& a,const Types&... args)
{
	//相对于泛化版本而言较为特化
	cout<<"This is The Specialized version1"<<endl;
}



int main()
{
	//prinf('a','b','c','v','f','s');
	match_test(1,2,3,4,5,6,7);
	match_test("test","bala","hello");
	return 0;	
}

