/*
 * @Description: 单例模式，
 * @Author: XZMJX
 * @Date: 2021-12-06 20:45:46
 */
#pragma once
#include<stdio.h>
#include<string>
#include"block_queue.h"
namespace xzmjx
{
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
        bool Init(const char* file_name,int close_log,int log_buf_size = 8192,int split_lines = 5000000,int max_queue_size = 0);

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
        /**
         * @Description: 异步写入日志，被异步写日志线程的处理函数调用
         * @Author: XZMJX
         * @Date: 2021-12-08 14:44:53
         * @param {*}
         * @return {*}
         */
        void* _async_write_log();
    private:
        char _M_dir_name[128];
        char _M_log_name[128];
        int  _M_max_line_count;
        int  _M_log_buff_size;
        long long _M_line_count;
        int _M_today;
        FILE* _M_fp;
        char* _M_buff;
        xzmjx::BlockQueue<std::string>* _M_log_queue;
        int _M_max_log_queue_count;
        bool _M_is_async;
        int _M_close_log;
        xzmjx::Locker _M_mutex;
    };

    
}