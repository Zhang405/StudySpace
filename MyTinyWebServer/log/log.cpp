/*
 * @Description: 
 * @Author: XZMJX
 * @Date: 2021-12-06 20:45:55
 */
#include"log.h"
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdarg.h>
#include<sys/time.h>
xzmjx::Log::Log()
{
    _M_line_count = 0;
    _M_is_async = false;

}
xzmjx::Log::~Log()
{

}
void* xzmjx::Log::AsyncWriteLogThread(void* ignore)
{
    xzmjx::Log::GetInstance()->_async_write_log();
}

void xzmjx::Log::Flush(void)
{
    _M_mutex.Lock();
    fflush(_M_fp);
    _M_mutex.UnLock();
}

bool xzmjx::Log::Init(const char* file_name,int close_log,int log_buf_size = 8192,int split_lines = 5000000,int max_queue_size = 0)
{
    if(_M_max_log_queue_count>=1)
    {
        //异步写入日志，多个生产者，一个消费者线程写日志
        _M_is_async = true;
        _M_log_queue = new BlockQueue<std::string>(_M_max_log_queue_count);
        pthread_t tid;
        pthread_create(&tid,NULL,AsyncWriteLogThread,NULL);
    }
    _M_close_log = close_log;
    _M_log_buff_size = log_buf_size;
    _M_buff = new char[_M_log_buff_size];
    memset(_M_buff,'\0',_M_log_buff_size);
    _M_max_line_count = split_lines;

    time_t stamp = time(NULL);
    struct tm* st_tm = localtime(&stamp);
    char fmttime[128];
    strftime(fmttime,128,"%Y_%m_%d",st_tm);

    const char *pos = strrchr(file_name,'/');
    char log_full_name[256];
    if(pos == NULL)
    {
        //example:2020_12_08WebServer
        strcpy(_M_log_name,file_name);
        snprintf(log_full_name,255,"%s_%s",fmttime,file_name);
    }
    else
    {
        //example:./2020_12_08WebServer
        strcpy(_M_log_name,pos+1);
        strncpy(_M_dir_name,file_name,pos-file_name+1);
        snprintf(log_full_name,255,"%s%s_%s",_M_dir_name,fmttime,_M_log_name);
    }
    _M_today = st_tm->tm_mday;
    _M_fp = fopen(log_full_name,"a");
    if(_M_fp == NULL)
        return false;
    return true;
}

void xzmjx::Log::WriteLog(ELogLevel level,const char* format,...)
{
    struct timeval now;
    gettimeofday(&now,NULL);
    time_t t = time(NULL);
    struct tm*st_tm = localtime(&t);
    struct tm my_tm = *st_tm;

    //打印日志等级
    char s[16];
    switch (level)
    {
    case ELogLevel::Log_Debug:
        strcpy(s,"[Debug]:");
        break;
    case ELogLevel::Log_Info:
        strcpy(s,"[Info]:");
        break;
    case ELogLevel::Log_Error:
        strcpy(s,"[Error]:");
        break;
    case ELogLevel::Log_Warn:
        strcpy(s,"[Warn]:");
        break;
    default:
        strcpy(s,"[Infor]:");
        break;
    }
    _M_mutex.Lock();
    _M_line_count++;
    //过了一天或者当前日志文件以达到最大行数
    if(_M_today != my_tm.tm_mday||_M_line_count%_M_max_line_count == 0)
    {
        char new_log[256];
        fflush(_M_fp);
        fclose(_M_fp);
        char tail[16];
        strftime(tail,16,"%Y_%m_%d",&my_tm);

        if(_M_today != my_tm.tm_mday)
        {
            snprintf(new_log,255,"%s%s%s",_M_dir_name,tail,_M_log_name);
        }
        else
        {
            snprintf(new_log,255,"%s%s%s.%lld",_M_dir_name,tail,_M_log_name,_M_line_count/_M_max_line_count);
        }
        _M_fp = fopen(new_log,"a");
    }
    _M_mutex.UnLock();

    //组装log打印信息
    va_list valist;
    va_start(valist,format);
    std::string log_message;

    _M_mutex.Lock();
    int n = snprintf(_M_buff,48,"%d-%02d-%02d %02d:%02d:%02d.%06ld %s",
                     my_tm.tm_year+1900,my_tm.tm_mon+1,my_tm.tm_mday,
                     my_tm.tm_hour,my_tm.tm_min,my_tm.tm_sec,now.tv_usec,s);
    int m = vsnprintf(_M_buff+n,_M_log_buff_size-1,format,valist);
    _M_buff[n+m] = '\n';
    _M_buff[n+m+1] = '\0';
    log_message = _M_buff;
    _M_mutex.UnLock();

    //分同步/异步写入文件
    if(_M_is_async&&!_M_log_queue->Full())
    {
        _M_log_queue->Push(log_message);
    }
    else
    {
        _M_mutex.Lock();
        fputs(log_message.c_str(),_M_fp);
        _M_mutex.UnLock();
    }
    va_end(valist);
}

void* xzmjx::Log::_async_write_log()
{
    std::string single_log;
    while (_M_log_queue->Pop(single_log))
    {
        _M_mutex.Lock();
        fflush(_M_fp);
        _M_mutex.UnLock();
    }
}

