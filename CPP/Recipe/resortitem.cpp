#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Item
{
    int key;
    int val;
};

void orderItem(vector<Item>& v){
    int last = v.size();
    int start = 0;
    while(start != last){
        if(v[start].key == 0){
            for(int i = start+1;i<=v.size()-1;i++){
                swap(v[i-1],v[i]);
            }
            last--;
        }else{
            start++;
        }
    }
}

void sort(vector<Item>& v,int l,int r){
    if(l>=r) return;
    int i = l;
    int j = r;
    while(i<j){
        while(i<j&&v[j].val>=v[l].val){
            j--;
        }
        while(i<j&&v[i].val<=v[l].val){
            i++;
        }
        swap(v[i],v[j]);
    }
    swap(v[i],v[l]);
    sort(v,l,i);
    sort(v,i+1,r);
}
int main(){
    vector<Item> v = {{1,2},{0,2},{4,5},{0,3},{8,0},{0,4},{0,5}};
    orderItem(v);
    for(auto&& it:v){
        cout<<"("<<it.key<<","<<it.val<<") ";
    }
    cout<<endl;
    sort(v,0,v.size()-1);
    for(auto&& it:v){
        cout<<"("<<it.key<<","<<it.val<<") ";
    }
    cout<<endl;
    return 0;
}
