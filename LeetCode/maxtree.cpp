/*
 * @Description: 输入vector构造二叉树，对于每一个子树而言都是父节点为最大值
 * @Author: XZMJX
 * @Date: 2021-12-07 13:15:14
 */

//构建原则：每一个数的父节点是左右两侧最近的比他大的元素中的较小值，若左右没有，则为根节点

#include<vector>
#include<stack>
#include<unordered_map>
class node
{
public:
    int val;
    node* left;
    node* right;
public:
    node():val(-1),left(nullptr),right(nullptr)
    {

    }
    node(int data):val(data),left(nullptr),right(nullptr)    
    {

    }
    ~node();
};

void popStackSetMap(std::stack<node*>& s,std::unordered_map<node*,node*>& map);
node* getMaxTree(const std::vector<int>& arr)
{
    int size = arr.size();
    std::vector<node*> nodeArr;
    nodeArr.reserve(size);
    for(int i = 0; i < size; ++i)
    {
        nodeArr[i] = new node(arr[i]) ;
    }

    std::stack<node*> stack;
    std::unordered_map<node*,node*> lBigMap;
    std::unordered_map<node*,node*> rBigMap;

    for(int i = 0; i < size; ++i)
    {
        node* cur = nodeArr[i];
        while(!stack.empty()&&stack.top()->val<cur->val)
        {
            popStackSetMap(stack,lBigMap);
        }
        stack.push(cur);
    }
    while (!stack.empty())
    {
        popStackSetMap(stack,lBigMap);
    }

    for(int i = size - 1; i > -1; --i)
    {
        node* cur = nodeArr[i];
        while(!stack.empty()&&stack.top()->val<cur->val)
        {
            popStackSetMap(stack,rBigMap);
        }
        stack.push(cur);
    }
    while (!stack.empty())
    {
        popStackSetMap(stack,rBigMap);
    }

    node* pRoot = nullptr;
    for(int i = 0; i < size; ++i)
    {
        node* cur = nodeArr[i];
        node* lPa = lBigMap[cur];
        node* rPa = rBigMap[cur];

        if(lPa==nullptr&&rPa == nullptr)
        {
            pRoot = cur;
        }
        else if(lPa == nullptr)
        {
            //右侧第一个比它大的作为父节点，判断接入左/右孩子处
            if(rPa->left == nullptr)
                rPa->left = cur;
            else    
                rPa->right = cur;
        }
        else if(rPa == nullptr)
        {
            if(lPa->left == nullptr)
                lPa->left = cur;
            else    
                lPa->right = cur;
        }
        else
        {
            node* pPa = lPa->val<rPa->val?lPa:rPa;
            if(pPa->left == nullptr)
                pPa->left = cur;
            else    
                pPa->right = cur;
        }
    }
    return pRoot;
}
void popStackSetMap(std::stack<node*>& s,std::unordered_map<node*,node*>& map)
{
    if(s.empty())
        return;
    node* p = s.top();
    s.pop();
    if(s.empty())
    {
        map[p] = nullptr;
    }
    else
    {
        map[p] = s.top();
    }
}

int main()
{
    std::vector<int> a = {0,1,2,3,4,5,6};
    node* root = getMaxTree(a);
    return 0;
}