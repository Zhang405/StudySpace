struct A
{
    int ax;
    virtual void f0() {}
};

struct B
{
    int bx;
    virtual void f1() {}
};

struct C : public A, public B
{
    int cx;
    void f0() override {}
    void f1() override {}
};