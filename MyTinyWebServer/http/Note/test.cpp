#include<iostream>
#include<string>
using namespace std;
int main()
{
    string a = "333";
    cout<<a<<endl;
    return 0;
}



typedef struct dictEntry{
    void* key;
    union{
        void* val;
        uint64_t u64;
        int64_t s64;
    } v;
    struct dictEntry* next;
};
typedef struct dictht
{
    //哈希表数组
    dictEntry** table;

    unsigned long size;
    unsigned long sizemaske;

    unsigned long used;
};
