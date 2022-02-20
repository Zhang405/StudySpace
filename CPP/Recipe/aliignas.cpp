struct NotAlign{
    char a_;
    int b_;
}__attribute__((packed));

#include<iostream>
int main(){
    std::cout<<sizeof(NotAlign)<<std::endl;
    NotAlign a;
    std::cout<<sizeof a<<std::endl;
    return 0;
}
