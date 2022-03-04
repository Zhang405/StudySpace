#include<list>
#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;


class LRUCache{
    typedef int keytype;
    typedef int valuetype;
    typedef pair<keytype,valuetype> item;
    typedef list<item> listtype;
    typedef unordered_map<keytype,typename listtype::iterator> maptype;
public:
    maptype map_;
    listtype list_;
    int capacity_;

public:
    void set(keytype k,valuetype v){
        auto iter = map_.find(k);
        if(iter == map_.end()){
            if(list_.size()>= capacity_){
                map_.erase(list_.back().first);
                list_.pop_back();
            }
            list_.push_front(std::make_pair(k,v));
            map_.insert(make_pair(k,list_.begin()));
        }
        else{
            iter->second->second = v;
            list_.splice(list_.begin(),list_,iter->second);
            return;
        }
    }

    int get(keytype k){
        auto iter = map_.find(k);
        if(iter == map_.end()){
            return -1;
        }
        else{
            list_.splice(list_.begin(),list_,iter->second);
            return iter->second->second;
        }
    }

};

int main(){
    LRUCache cache;
    cache.set(1,1);
    cache.set(2,2);
    cache.set(3,3);
    cache.set(4,4);
    return 0;
}