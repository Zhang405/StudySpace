/*
 * @Description: 单例模式，
 * @Author: XZMJX
 * @Date: 2021-12-06 20:45:46
 */
#pragma once
#include<stdio.h>
#include<string>
    class Log
    {
    public:
        enum class ELogLevel:int
        {
            Log_Debug = 0,
            Log_Info,
            Log_Warn,
            Log_Error
        };
    public:
        /**
         * @Description: 懒汉式单例，c++11保证静态局部变量的初始化过程是线程安全的。
         * @Author: XZMJX
         * @Date: 2021-12-08 14:36:19
         * @param {*}
         * @return {*}
         */
        static Log* GetInstance()
        {
            static Log instance;
            return &instance;
        }

        /**
         * @Description: 异步写日志线程主函数
         * @Author: XZMJX
         * @Date: 2021-12-08 14:46:50
         * @param {void*} ignore
         * @return {*}
         */
        static void* AsyncWriteLogThread(void* ignore);
        /**
         * @Description: 初始化Log参数
         * @Author: XZMJX
         * @Date: 2021-12-08 14:40:32
         * @param {char*} file_name：   log文件全名称
         * @param {int} close_log：     是否关闭日志
         * @param {int} log_buf_size：  缓冲区大小
         * @param {int} split_lines：   最大行数
         * @param {int} max_queue_size：阻塞队列大小
         * @return {*}
         */
        bool Init(const char* file_name,int close_log,int log_buf_size = 8192,int split_lines = 5000000);

        /**
         * @Description: 写日志主函数
         * @Author: XZMJX
         * @Date: 2021-12-08 14:41:52
         * @param {int} level   ：日志级别
         * @param {char*} format：内容格式
         * @return {*}
         */
        void WriteLog(ELogLevel level,const char* format,...);

        /**
         * @Description: 强制刷新至文件中（fputs至内核缓冲区，调用flush刷新到磁盘文件）
         * @Author: XZMJX
         * @Date: 2021-12-08 14:43:33
         * @param {*}
         * @return {*}
         */
        void Flush(void);

    private:
        ~Log();
        Log();
    private:
        char _M_dir_name[128];
        char _M_log_name[128];
        int  _M_max_line_count;
        int  _M_log_buff_size;
        long long _M_line_count;
        int _M_today;
        FILE* _M_fp;
        char* _M_buff;
    };
//##__VA_ARGS__前面的##是为了当可变参数...没有时，去掉前面多余的，
#define LOG_DEBUG(format, ...)  {Log::GetInstance()->WriteLog(Log::ELogLevel::Log_Debug, format, ##__VA_ARGS__); Log::GetInstance()->Flush();}
#define LOG_INFO(format, ...)   {Log::GetInstance()->WriteLog(Log::ELogLevel::Log_Info, format, ##__VA_ARGS__); Log::GetInstance()->Flush();}
#define LOG_WARN(format, ...)   {Log::GetInstance()->WriteLog(Log::ELogLevel::Log_Warn, format, ##__VA_ARGS__); Log::GetInstance()->Flush();}
#define LOG_ERROR(format, ...)  {Log::GetInstance()->WriteLog(Log::ELogLevel::Log_Error, format, ##__VA_ARGS__); Log::GetInstance()->Flush();}
