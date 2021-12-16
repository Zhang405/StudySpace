#pragma once
#include<list>
#include<mysql/mysql.h>
#include"../lock/locker.h"
#include<string>
namespace xzmjx
{
    class SQLConnPool
    {
    public:
        MYSQL* GetConnection();
        bool ReleseConnection(MYSQL* con);
        int GetFreeConnNum();
        void DestroyConnPool();
        static SQLConnPool* GetConnPool();
        void Init(std::string url,std::string user,std::string passwd,std::string database,int port,int maxconn,int closeLog);
    private:
        SQLConnPool();
        ~SQLConnPool();
    private:
        int _M_maxConnNum;
        int _M_ConnNumOccupy;
        int _M_freeConnNum;
        Locker _M_Locker;
        std::list<MYSQL*> _M_ConnList;
        Sem _M_SemToReserveConn;

    public:
        std::string _M_url;
        std::string _M_port;
        std::string _M_user;
        std::string _M_passwd;
        std::string _M_DatabaseName;
        int _M_close_log;
    };

    class SQLConnRAII
    {
    public:
        SQLConnRAII(SQLConnPool* pool);
        ~SQLConnRAII();

        MYSQL* SQLConn()
        {
            return _M_conn;
        }
    private:
        SQLConnPool* _M_ConnPool;
        MYSQL* _M_conn;
    };
    
}