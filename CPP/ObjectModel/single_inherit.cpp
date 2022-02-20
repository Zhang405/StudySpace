struct A
{
    int ax; // 成员变量
    virtual void f0() {}
    virtual void f1() {}
};

struct B : public A
{
    int bx; // 成员变量
    void f0() override {}; // 重写f0
};








