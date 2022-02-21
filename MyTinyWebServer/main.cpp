#include "config.h"
#include "webserver/webserver.h"


int main(int argc, char *argv[])
{
    //需要修改的数据库信息,登录名,密码,库名
    string user = "root";
    string passwd = "XZMJX";
    string databasename = "zmj";

    //命令行解析
    Config config;
    config.parse_arg(argc, argv);

    xzmjx::WebServer server;

    //初始化
    server.Init(config.PORT, user, passwd, databasename, config.LOGWrite, 
                config.OPT_LINGER, config.TRIGMode,  config.sql_num,  config.thread_num, 
                config.close_log, config.actor_model);
    

    //日志
    server.LogWrite();

    //数据库
    server.SqlPool();

    //线程池
    server.ThreadPool();

    //触发模式
    server.TrigMode();

    //监听
    server.EventListen();

    //运行
    server.EventLoop();

    return 0;
}