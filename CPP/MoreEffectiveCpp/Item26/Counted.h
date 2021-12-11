/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-11-30 19:22:55
 */
#pragma once
#include<glob.h>
namespace XZMJX
{
    template<typename BeingCounted> 
    class Counted
    {
    public:
        class TooManyObject{};
        static size_t Object_Count(){return m_NowObjNum;}
    protected:
        Counted();
        Counted(const Counted&);
        ~Counted(){--m_NowObjNum;}
    private:
        static size_t m_NowObjNum;
        static const size_t m_MaxObjNum;

        void init();
    };
    template<typename BeingCounted>
    size_t Counted<BeingCounted>::m_NowObjNum = 0;

    
    template<typename BeingCounted>
    Counted<BeingCounted>::Counted()
    {
        init();
    }

    template<typename BeingCounted>
    Counted<BeingCounted>::Counted(const Counted<BeingCounted>&rhs)
    {
        init();
    }

    template<typename BeingCounted>
    void Counted<BeingCounted>::init()
    {
        if(m_NowObjNum >= m_MaxObjNum)
        {
            throw TooManyObject();
        }
        ++m_NowObjNum;
    }
}