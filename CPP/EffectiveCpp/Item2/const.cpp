#include "../header/common_header.hpp"

class const_test{
private:
    int a_;
    int b_;
public:
    const_test(int a,int b):a_(a),b_(b){}
    ~const_test() = default;
    const_test() = default;
public:
    const int& GetA() const
    {
        return a_;
    }
    int GetA()
    {
        return a_;
    }
};
int main(){
    const_test x(3,4);
    std::cout<<x.GetA()<<std::endl;
    const const_test y(5,6);
    std::cout<<y.GetA()<<std::endl;
}