#include"LimitedObj.h"

using namespace XZMJX;

LimitedObj::LimitedObj()
{
    m_TestMem = Object_Count();
}
LimitedObj::~LimitedObj()
{

}
LimitedObj* LimitedObj::CreateObject()
{
    return new LimitedObj();
}

const size_t Counted<LimitedObj>::m_MaxObjNum = 10;