#include"binTree.h"
#include<iostream>
int main()
{
    string s = "12!3!#!#!4!5!#!#!6!#!#!";
    binNode* head = rebuildByPreString(s);

    std::cout<<seriaByPre(head)<<std::endl;
}