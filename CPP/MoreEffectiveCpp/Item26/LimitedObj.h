#pragma once
#include"Counted.h"

namespace XZMJX
{
    class LimitedObj:private Counted<LimitedObj>
    {
    private:
        LimitedObj();
        LimitedObj(const LimitedObj&) = delete;
        int m_TestMem;
    public:
        static LimitedObj* CreateObject();
        ~LimitedObj();
        using Counted<LimitedObj>::Object_Count;
        using Counted<LimitedObj>::TooManyObject;
    };
}