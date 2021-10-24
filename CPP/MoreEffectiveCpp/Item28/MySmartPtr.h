#pragma once

#include<exception>
namespace XZMJX
{
    template <typename T>
    class MySmartPtr
    {
    private:
        T* m_pDumbPtr;
        size_t* m_ReferCount;
    public:
        MySmartPtr(T* ptr = nullptr);
        MySmartPtr(const MySmartPtr& rhs);
        ~MySmartPtr();

        MySmartPtr& operator = (const MySmartPtr& rhs);
        T& operator*();
        T* operator->();
        size_t Refer_Count();
    };

    template<typename T>
    MySmartPtr<T>::MySmartPtr(T* ptr):m_pDumbPtr(ptr)
    {
        if(m_pDumbPtr)
            m_ReferCount = new size_t(1);
        else
            m_ReferCount = new size_t(0);
    }

    template<typename T>
    MySmartPtr<T>::MySmartPtr(const MySmartPtr&rhs)
    {
        if(rhs.m_pDumbPtr == this->m_pDumbPtr)
            return;
        
        this->m_pDumbPtr = rhs.m_pDumbPtr;
        this->m_ReferCount = rhs.m_ReferCount;
        (*this->m_ReferCount)++;
    }

    template<typename T>
    MySmartPtr<T>::~MySmartPtr()
    {
        (*this->m_ReferCount)--;
        if(this->m_ReferCount == 0)
        {
            delete this->m_pDumbPtr;
            delete this->m_ReferCount;
        }
    }

    template<typename T>
    MySmartPtr<T>& MySmartPtr<T>::operator =(const MySmartPtr& rhs)
    {
        if(this->m_pDumbPtr == rhs.m_pDumbPtr)
            return *this;
        
        if(this->m_pDumbPtr)
        {
            (*this->m_ReferCount)--;
            if(this->m_ReferCount == 0)
            {
                delete this->m_pDumbPtr;
                delete this->m_ReferCount;
            }
        }
        this->m_pDumbPtr = rhs.m_pDumbPtr;
        this->m_ReferCount = rhs.m_ReferCount;
        (*this->m_ReferCount)++;
        return *this;
    }

    template<typename T>
    T* MySmartPtr<T>::operator->()
    {
        if(this->m_pDumbPtr == nullptr)
        {
            throw std::exception("null pointer");
        }
        return this->m_pDumbPtr;
    }

    template<typename T>
    T& MySmartPtr<T>::operator*()
    {
        if(this->m_pDumbPtr == nullptr)
        {
            throw std::exception("null pointer");
        }
        return *(this->m_pDumbPtr);
    }
    template<typename T>
    size_t MySmartPtr<T>::Refer_Count()
    {
        return *this->m_ReferCount;
    }



    
}