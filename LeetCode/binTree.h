class binNode
{
public:
    int val;
    binNode* left;
    binNode* right;

public:
    binNode(int value):val(value),left(nullptr),right(nullptr)
    {
    }
};

#include<string>
#include<vector>
#include<queue>
using namespace std;

string seriaByPre(binNode* head)
{
    if(head == nullptr)
        return "#!";
    string res = to_string(head->val) + "!";
    res+=seriaByPre(head->left);
    res+=seriaByPre(head->right);
    return res;
}

void split(const string& s,vector<string>&tokens,const string& delimiters = " ")
{
    string::size_type lastPos = s.find_first_not_of(delimiters,0);
    string::size_type pos = s.find_first_of(delimiters,lastPos);
    while(string::npos != pos ||string::npos != lastPos)
    {
        tokens.push_back(s.substr(lastPos,pos - lastPos));
        lastPos = s.find_first_not_of(delimiters,pos);
        pos = s.find_first_of(delimiters,lastPos);
    }
}

binNode* rebuild(queue<string>& q)
{
    string val = q.front();
    q.pop();
    if(val == "#")
    {
        return nullptr;
    }
    binNode* head = new binNode(atoi(val.c_str()));
    head->left = rebuild(q);
    head->right = rebuild(q);
    return head;
}


binNode* rebuildByPreString(string pre)
{
    vector<string> vstrSplit;
    split(pre,vstrSplit,"!");
    queue<string> q;
    for(auto&& it:vstrSplit)
    {
        q.push(it);
    }
    return rebuild(q);
}