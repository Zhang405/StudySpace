#include"sql_connection_pool.h"
#include"../log/log.h"
namespace xzmjx
{
    MYSQL* SQLConnPool::GetConnection() 
    {
        MYSQL* res = NULL;
        _M_SemToReserveConn.Wait();
        _M_Locker.Lock();
        con = _M_ConnList.pop_front();
        ++_M_ConnNumOccupy;
        --_M_freeConnNum;
        _M_Locker.UnLock();
        return con;
    }
    
    bool SQLConnPool::ReleseConnection(MYSQL* con) 
    {
        if(NULL == conn)
            return false;
        _M_Locker.Lock();
        _M_ConnList.push_back(con);
        ++_M_freeConnNum;
        --_M_ConnNumOccupy;
        _M_Locker.UnLock();
        _M_SemToReserveConn.Post();
    }
    
    int SQLConnPool::GetFreeConnNum() 
    {
        return _M_freeConnNum;
    }
    
    void SQLConnPool::DestroyConnPool() 
    {
        _M_Locker.Lock();
        if(_M_ConnList.size()>0)
        {
            for(auto it = _M_ConnList.begin();it != _M_ConnList.end(); ++it)
            {
                MYSQL* conn = *it;
                mysql_close(conn);
            }
            _M_ConnNumOccupy = 0;
            _M_freeConnNum = 0;
            _M_ConnList.clear();
        }
        _M_Locker.UnLock();
    }
    
    SQLConnPool* SQLConnPool::GetConnPool() 
    {
        static SQLConnPool pool;
        return &pool;
    }
    
    void SQLConnPool::Init(std::string url,std::string user,std::string passwd,std::string database,int port,int maxconn,int closeLog) 
    {
        _M_url = url;
        _M_user = user;
        _M_passwd = passwd;
        _M_DatabaseName = database;
        _M_port = port;
        _M_maxConnNum = maxconn;
        _M_closeLog = closeLog;

        for(int i = 0; i < _M_maxConnNum;++i)
        {
            MYSQL* con = NULL;
            con = mysql_init(con);
            if(con == NULL)
            {
                LOG_ERROR("MySQL ERROR");
                exit(1);
            }
            con = mysql_real_connect(con,url.c_str(),user.c_str(),passwd.c_str(),database.c_str(),port,NULL,0);
            if(con == NULL)
            {
                LOG_ERROR("MySQL ERROR");
                exit(1);
            }
            _M_ConnList.push_back(con);
            ++_M_freeConnNum;
        }
        _M_SemToReserveConn = Sem(_M_freeConnNum);
        _M_maxConnNum = _M_freeConnNum;
    }
    
    SQLConnPool::SQLConnPool() :_M_maxConnNum(100),
    _M_ConnNumOccupy(0),
    _M_freeConnNum(100),
    _M_closeLog(0)
    {
        
    }
    
    SQLConnPool::~SQLConnPool() 
    {
        DestroyConnPool();
    }
    
    SQLConnRAII::SQLConnRAII(SQLConnPool* pool) 
    {
        _M_ConnPool = pool;
        _M_conn = _M_ConnPool->GetConnection();
    }
    
    SQLConnRAII::~SQLConnRAII() 
    {
        _M_ConnPool->ReleseConnection(_M_conn);
    }
}