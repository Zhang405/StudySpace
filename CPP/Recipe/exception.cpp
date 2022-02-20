#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
    vector<int> v(10,0);
    vector<int> v_cp = v;
    for_each(v_cp.begin(),v_cp.end(),[](int&v1){v1++;});
    for_each(v_cp.begin(),v_cp.end(),[](int v1){cout<<v1<<endl;});
    for_each(v.begin(),v.end(),[](int&v1){cout<<v1<<endl;});
    return 0;
}