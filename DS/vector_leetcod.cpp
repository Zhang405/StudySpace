#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

#define TESTONEARG(fun,v,arg)\
        fun(v,arg)\

template <typename T>
void printvector(const vector<T>&v){
    cout<<"(";
    for(int i = 0; i<v.size();i++){
        cout<<v[i];
        if(i != v.size()-1){
            cout<<",";
        }
    }
    cout<<")";
    cout<<endl;
}


int binarySearch(const vector<int>& v,int t){
    int left = 0;
    int right = v.size();
    while(left<right){
        int mid = left+(right-left)/2;
        if(v[mid]>t){
            right = mid;
        }
        else{
            left = mid+1;
        }
    }
    return --left;
}

void remove(vector<int>& v,int tar){
    int i = 0;
    int j = 0;
    while(j<=v.size()-1){
        if(v[j] != tar){
            v[i++] = v[j];
        }
        j++;
    }
    for(int k = i;i<v.size();i++){
        v[i] = 0;
    }
}

vector<int>sortedSquares(vector<int> v,void* ign){
    int i = 0;
    int j = v.size()-1;
    int index = v.size()-1;
    vector<int> res(v.size(),0);
    while(i<=j){
        if(abs(v[i])<abs(v[j])){
            res[index--] = v[j]*v[j];
            j--;
        }
        else{
            res[index--] = v[i]*v[i];
            i++;
        }
    }
    return res;
}

int minSubArrayLen(const vector<int>& v,int t){
    int min_len = INT_MAX;
    int total = 0;
}


int main(){
    vector<int> v ={-4,-1,4,5,6,7,8,9};
    printvector(TESTONEARG(sortedSquares,v,nullptr));
    return 0;
}