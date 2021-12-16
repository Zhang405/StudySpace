#include"http_conn.h"

//定义http响应的一些状态信息
const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form = "You do not have permission to get file form this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form = "There was an unusual problem serving the request file.\n";
namespace xzmjx
{
    HttpConn::HttpConn() 
    {
        
    }

    HttpConn::~HttpConn() 
    {
        
    }
    
    void HttpConn::Init(int sockfd,const sockaddr_in &addr,char*,int,int,std::string user,std::string passwd,std::string sqlname) 
    {
        
    }
    
    
    void HttpConn::CloseConnection() 
    {
        
    }
    
    void HttpConn::Process() 
    {
        
    }
    
    bool HttpConn::Write() 
    {
        
    }
    void HttpConn::init()
    {

    }
    HttpConn::HTTP_CODE HttpConn::process_read()
    {

    }
    bool HttpConn::process_write(HttpConn::HTTP_CODE ret)
    {

    }
    HttpConn::HTTP_CODE HttpConn::parse_request_line(char *text)
    {

    }
    HttpConn::HTTP_CODE HttpConn::parse_headers(char *text)
    {

    }
    HttpConn::HTTP_CODE HttpConn::parse_content(char *text)
    {

    }
    HttpConn::HTTP_CODE HttpConn::do_request()
    {

    }
    HttpConn::LINE_STATUS HttpConn::parse_line()
    {

    }
    void HttpConn::unmap()
    {

    }
    bool HttpConn::add_response(const char *format, ...)
    {

    }
    bool HttpConn::add_content(const char *content)
    {

    }
    bool HttpConn::add_status_line(int status, const char *title)
    {

    }
    bool HttpConn::add_headers(int content_length)
    {

    }
    bool HttpConn::add_content_type()
    {

    }
    bool HttpConn::add_content_length(int content_length)
    {

    }
    bool HttpConn::add_linger()
    {

    }
    bool HttpConn::add_blank_line()
    {

    }
}