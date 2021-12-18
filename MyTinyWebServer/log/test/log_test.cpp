#include"../log.h"
#include<string>
using namespace xzmjx;

int main()
{
    Log::GetInstance()->Init("./LogTest",0,8182,5000000,20);
    int _M_close_log = 0;
    for(int i = 0; i < 65553;++i)
    {
        LOG_DEBUG("This is Debug_%s",std::to_string(i).c_str());
        LOG_ERROR("This is ERROR_%s",std::to_string(i).c_str());
        LOG_INFO("This is INFO_%s",std::to_string(i).c_str());
        LOG_WARN("This is WARN_%s",std::to_string(i).c_str());
    }
    return 0;
}