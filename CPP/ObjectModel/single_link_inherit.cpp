struct A
{
    int ax;
    virtual void f0() {}
};

struct B : public A
{
    int bx;
    virtual void f1() {}
};

struct C : public B
{
    int cx;
    void f0() override {}
    virtual void f2() {}
};