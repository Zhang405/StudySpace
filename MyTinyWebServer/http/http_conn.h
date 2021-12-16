#pragma once
#include<sys/socket.h>
#include<netinet/in.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<sys/stat.h>
#include<mysql/mysql.h>


namespace xzmjx
{
    class HttpConn
    {
    public:
        static const int FILENAME_LEN = 200;
        static const int READ_BUFFER_SIZE = 2048;
        static const int WRITE_BUFFER_SIZE = 1024;

        enum METHOD
        {
            GET = 0,
            POST,
            HEAD,
            PUT,
            DELETE,
            TRACE,
            OPTIONS,
            CONNECT,
            PATH
        };

        //主状态机状态
        enum CHECK_STATE
        {
            CHECK_STATE_REQUESTLINE = 0,
            CHECK_STATE_HEADER,
            CHECK_STATE_CONTENT
        };
        
        //从状态机状态
        enum LINE_STATUS
        {
            LINE_OK = 0,
            LINE_BAD,
            LINE_OPEN
        };

        //http应答代码
        enum HTTP_CODE
        {
            NO_REQUEST,
            GET_REQUEST,
            BAD_REQUEST,
            NO_REQUEST,
            FORBINDEN_REQUEST,
            INTERNAL_ERROR,
            CLOSED_CONNECTION
        };
    public:
        HttpConn();
        ~HttpConn();
    public:
        void Init(int sockfd,const sockaddr_in &addr,char*,int,int,std::string user,std::string passwd,std::string sqlname);
        void CloseConnection();
        void Process();
        bool Write();
        sockaddr_in* GetAddr()
        {
            return &_M_address;
        }

        //void IniMySQLResult(connection_pool *connPool);
        int timer_flag;
        int improv;
    private:
        void init();
        HTTP_CODE process_read();
        bool process_write(HTTP_CODE ret);
        HTTP_CODE parse_request_line(char *text);
        HTTP_CODE parse_headers(char *text);
        HTTP_CODE parse_content(char *text);
        HTTP_CODE do_request();
        char *get_line() { return _M_read_buf + _M_start_line; };
        LINE_STATUS parse_line();
        void unmap();
        bool add_response(const char *format, ...);
        bool add_content(const char *content);
        bool add_status_line(int status, const char *title);
        bool add_headers(int content_length);
        bool add_content_type();
        bool add_content_length(int content_length);
        bool add_linger();
        bool add_blank_line();
    public:
        static int m_epollfd;
        static int m_user_count;
        MYSQL *mysql;
        int m_state;  //读为0, 写为1
    private:
        int _M_sockfd;
        sockaddr_in _M_address;
        char _M_read_buf[READ_BUFFER_SIZE];
        int _M_read_idx;
        int _M_check_idx;
        int _M_start_line;
        char _M_write_buf[WRITE_BUFFER_SIZE];
        int _M_write_idx;
        CHECK_STATE _M_check_state;
        METHOD _M_method;
        char _M_real_file[FILENAME_LEN];
        char* _M_url;
        char* _M_version;
        char* _M_host;
        int _M_content_len;
        bool _M_linger;
        char* _M_file_address;
        struct stat _M_file_stat;
        struct iovec _M_ivp[2];
        int _M_iv_count;
        int cgi;
        char* _M_string;
        int bytes_to_send;
        int bytes_have_send;
        char* doc_root;

        std::map<std::string,std::string> _M_users;
        int _M_TRIGMode;
        int _M_close_log;
        char sql_user[100];
        char sql_passwd[100];
        char sql_name[100];
    };
}