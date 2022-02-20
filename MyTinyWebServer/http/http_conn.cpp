#include"http_conn.h"
#include"../lock/locker.h"
#include"../log/log.h"
#include<unistd.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<string.h>
#include<sys/mman.h>
#include<sys/uio.h>
#include<errno.h>

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

xzmjx::Locker m_lock;
std::map<std::string, std::string> users;
namespace xzmjx
{
    //对文件描述符设置非阻塞
    int setnonblocking(int fd)
    {
        int old_option = fcntl(fd, F_GETFL);
        int new_option = old_option | O_NONBLOCK;
        fcntl(fd, F_SETFL, new_option);
        return old_option;
    }

    //将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
    {
        epoll_event event;
        event.data.fd = fd;

        if (1 == TRIGMode)
            event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
        else
            event.events = EPOLLIN | EPOLLRDHUP;

        if (one_shot)
            event.events |= EPOLLONESHOT;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
        setnonblocking(fd);
    }

    //从内核时间表删除描述符
    void removefd(int epollfd, int fd)
    {
        epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
        close(fd);
    }

    //将事件重置为EPOLLONESHOT
    void modfd(int epollfd, int fd, int ev, int TRIGMode)
    {
        epoll_event event;
        event.data.fd = fd;

        if (1 == TRIGMode)
            event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
        else
            event.events = ev | EPOLLONESHOT | EPOLLRDHUP;

        epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
    }

    int HttpConn::m_user_count = 0;
    int HttpConn::m_epollfd = -1;

    HttpConn::HttpConn() 
    {
        
    }

    HttpConn::~HttpConn() 
    {
        
    }
    
    void HttpConn::Init(int sockfd,const sockaddr_in &addr,char*root,int TRIGMode,int close_log,std::string user,std::string passwd,std::string sqlname) 
    {
        _M_sockfd = sockfd;
        _M_address = addr;

        addfd(m_epollfd,sockfd,true,_M_TRIGMode);
        m_user_count++;
        doc_root = root;
        _M_TRIGMode = TRIGMode;
        _M_close_log = close_log;

        strcpy(sql_user, user.c_str());
        strcpy(sql_passwd, passwd.c_str());
        strcpy(sql_name, sqlname.c_str());

        Init();
    }
    void HttpConn::Init()
    {
        mysql = NULL;
        bytes_to_send = 0;
        bytes_have_send = 0;
        _M_check_state = CHECK_STATE_REQUESTLINE;
        _M_longer = false;
        _M_method = GET;
        _M_url = 0;
        _M_version = 0;
        _M_content_len = 0;
        _M_host = 0;
        _M_start_line = 0;
        _M_check_idx = 0;
        _M_read_idx = 0;
        _M_write_idx = 0;
        cgi = 0;
        m_state = 0;
        timer_flag = 0;
        improv = 0;

        memset(_M_read_buf, '\0', READ_BUFFER_SIZE);
        memset(_M_write_buf, '\0', WRITE_BUFFER_SIZE);
        memset(_M_real_file, '\0', FILENAME_LEN);
    }
    HttpConn::HTTP_CODE HttpConn::ProcessRead()
    {
        LINE_STATUS line_status = LINE_OK;
        HTTP_CODE ret = NO_REQUEST;
        char* text = 0;

        while((_M_check_state == CHECK_STATE_CONTENT&&line_status == LINE_OK)||((line_status = ParseLine())) == LINE_OK)
        {
            text = GetLine();               //读出已解析的一行
            _M_start_line = _M_check_idx;   //新一行的起点
            LOG_INFO("%s",text);
            switch (_M_check_state)
            {
                case CHECK_STATE_REQUESTLINE:
                {
                    ret = ParseRequestLine(text);
                    if(ret == BAD_REQUEST)
                    {
                        return BAD_REQUEST;
                    }
                    break;
                }
                case CHECK_STATE_HEADER:
                {
                    ret = ParseHeaders(text);
                    if(ret == BAD_REQUEST)
                    {
                        return BAD_REQUEST;
                    }
                    else if(ret == GET_REQUEST)
                    {
                        return DoRequest();
                    }
                    break;
                }  
                case CHECK_STATE_CONTENT:
                {
                    ret = ParseContent(text);
                    if(ret == BAD_REQUEST)
                    {
                        return BAD_REQUEST;
                    }
                    else if(ret == GET_REQUEST)
                    {
                        return DoRequest();
                    }
                    break;
                }  
                default:
                    return INTERNAL_ERROR;            
            }
        }
        return NO_REQUEST;
    }
    bool HttpConn::ProcessWrite(HTTP_CODE ret)
    {
        switch (ret)
        {
        case HTTP_CODE::INTERNAL_ERROR:
            {
                AddStatusLine(500,error_500_title);
                AddHeaders(strlen(error_500_form));
                if(!AddContent(error_500_form))
                {
                    return false;
                }
                break;
            }
        case HTTP_CODE::BAD_REQUEST:
            {
                AddStatusLine(404,error_404_title);
                AddHeaders(strlen(error_404_title));
                if(!AddContent(error_500_form))
                {
                    return false;
                }
                break;
            }
        case HTTP_CODE::FORBINDEN_REQUEST:
            {
                AddStatusLine(403, error_403_title);
                AddHeaders(strlen(error_403_form));
                if (!AddContent(error_403_form))
                    return false;
                break;
            }
        case HTTP_CODE::FILE_REQUEST:
            {
                AddStatusLine(200, ok_200_title);
                if (_M_file_stat.st_size != 0)
                {
                    AddHeaders(_M_file_stat.st_size);
                    _M_ivp[0].iov_base = _M_write_buf;
                    _M_ivp[0].iov_len = _M_write_idx;
                    _M_ivp[1].iov_base = _M_file_address;
                    _M_ivp[1].iov_len = _M_file_stat.st_size;
                    _M_iv_count = 2;
                    bytes_to_send = _M_write_idx + _M_file_stat.st_size;
                    return true;
                }
                else
                {
                    const char *ok_string = "<html><body></body></html>";
                    AddHeaders(strlen(ok_string));
                    if (!AddContent(ok_string))
                        return false;
                }
            }
        default:
            break;
        }
        _M_ivp[0].iov_base = _M_write_buf;
        _M_ivp[0].iov_len = _M_write_idx;
        _M_iv_count = 1;
        bytes_to_send = _M_write_idx;
        return true;
    }
    HttpConn::HTTP_CODE HttpConn::ParseRequestLine(char *text)
    {
        /*************************************************
         GET /hello.txt HTTP/1.1
         User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
         Host: www.example.com
         Accept-Language: en, mi
        **************************************************/

        /*********************解析请求行信息开始********************/
        _M_url = strpbrk(text," \t"); //查找首次出现空格或者制表符的地方
        if(!_M_url)
        {
            return BAD_REQUEST;
        }
        // GET'/0'/hello.txt HTTP/1.1
        //|method|^_M_url
        *_M_url++ = '\0';
        char* method = text;
        if(0 == strcasecmp(method,"GET"))
        {
            _M_method = METHOD::GET;
        }
        else if(0 == strcasecmp(method,"POST"))
        {
            _M_method = METHOD::POST;
            cgi = 1;//需要查询数据库
        }
        else
            return BAD_REQUEST;
        // GET'/0'/hello.txt HTTP/1.1
        //|method|^_M_url
        _M_url += strspn(_M_url," \t");
        // GET'/0'/hello.txt HTTP/1.1
        //|method| ^_M_url  ^_M_version
        _M_version = strpbrk(_M_url," \t");
        if(!_M_version)
        {
            return BAD_REQUEST;
        }
        // GET'/0'/hello.txt'/0'HTTP/1.1
        //|method|^_M_url       ^_M_version
        *_M_version++ = '\0';  
        _M_version += strspn(_M_version, " \t");
        if(strcasecmp(_M_version,"HTTP/1.1")!=0)
        {
            return BAD_REQUEST;
        }
        if(strncasecmp(_M_url,"http://",7) == 0)
        {
            _M_url+=7;
            _M_url = strchr(_M_url,'/');
        }
        if(strncasecmp(_M_url,"https://",8) == 0)
        {
            _M_url+=8;
            _M_url = strchr(_M_url,'/');
        }
        if(!_M_url||_M_url[0] != '/')
        {
            return BAD_REQUEST;
        }
        if(strlen(_M_url) == 1)
        {
            strcat(_M_url,"judge.html");//为根目录时的文件
        }
        _M_check_state = CHECK_STATE_HEADER;
        return NO_REQUEST;
    }
    HttpConn::HTTP_CODE HttpConn::ParseHeaders(char *text)
    {
        if(text[0] == '\0')
        {
            //空行
            if(_M_content_len != 0)
            {
                _M_check_state = CHECK_STATE_CONTENT;
                //POST方法，还有body信息尚未完全读入，不必开始处理
                return NO_REQUEST;
            }
            //GET方法，已完整读入请求，可以开始处理
            return GET_REQUEST;
        }
        else if(strncasecmp(text,"Connection:",11) == 0)
        {
            text += 11;
            text += strspn(text," \t");
            if(strcasecmp(text,"keep-alive") == 0)
            {
                _M_longer = true;
            }
        }
        else if(strncasecmp(text,"Content-length",15) == 0)
        {
            text += 15;
            text += strspn(text," \t");
            _M_content_len = atol(text);
        }
        else if(strncasecmp(text,"Host:",5) == 0)
        {
            text += 5;
            text += strspn(text," \t");
            _M_host = text;
        }
        else
        {
            LOG_INFO("oops!unknown header: %s",text);
        }
        return NO_REQUEST;
    }
    HttpConn::HTTP_CODE HttpConn::ParseContent(char *text)
    {
        if(_M_read_idx >= (_M_content_len+=_M_check_idx))
        {
            //请求内容已完整读入
            text[_M_content_len] = '\0';
            _M_string = text;
            return GET_REQUEST;
        }
        return NO_REQUEST;
    }
    HttpConn::HTTP_CODE HttpConn::DoRequest()
    {
        strcpy(_M_real_file,doc_root);
        int len = strlen(doc_root);
        const char* p = strrchr(_M_url,'/');

        if(cgi == 1&&(*(p+1))=='2'||(*p+1) == '3')
        {
            //根据标志判断是登录检测还是注册检测
            char flag = _M_url[1];

            char *m_url_real = (char *)malloc(sizeof(char) * 200);
            strcpy(m_url_real, "/");
            strcat(m_url_real, _M_url + 2);
            strncpy(_M_real_file + len, m_url_real, FILENAME_LEN - len - 1);
            free(m_url_real);

            //将用户名和密码提取出来
            //user=123&passwd=123
            char name[100], password[100];
            int i;
            for (i = 5; _M_string[i] != '&'; ++i)
                name[i - 5] = _M_string[i];
            name[i - 5] = '\0';

            int j = 0;
            for (i = i + 10; _M_string[i] != '\0'; ++i, ++j)
                password[j] = _M_string[i];
            password[j] = '\0';

            if (*(p + 1) == '3')
            {
                //如果是注册，先检测数据库中是否有重名的
                //没有重名的，进行增加数据
                char *sql_insert = (char *)malloc(sizeof(char) * 200);
                strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
                strcat(sql_insert, "'");
                strcat(sql_insert, name);
                strcat(sql_insert, "', '");
                strcat(sql_insert, password);
                strcat(sql_insert, "')");

                if (users.find(name) == users.end())
                {
                    //添加要加锁
                    m_lock.Lock();
                    int res = mysql_query(mysql, sql_insert);
                    users.insert(std::pair<std::string, std::string>(name, password));
                    m_lock.UnLock();

                    if (!res)
                        strcpy(_M_url, "/log.html");
                    else
                        strcpy(_M_url, "/registerError.html");
                }
                else
                    strcpy(_M_url, "/registerError.html");
            }
            //如果是登录，直接判断
            //若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0
            else if (*(p + 1) == '2')
            {
                //读不必加锁
                if (users.find(name) != users.end() && users[name] == password)
                    strcpy(_M_url, "/welcome.html");
                else
                    strcpy(_M_url, "/logError.html");
            }
        }

        if (*(p + 1) == '0')
        {
            char *m_url_real = (char *)malloc(sizeof(char) * 200);
            strcpy(m_url_real, "/register.html");
            strncpy(_M_real_file + len, m_url_real, strlen(m_url_real));

            free(m_url_real);
        }
        else if (*(p + 1) == '1')
        {
            char *m_url_real = (char *)malloc(sizeof(char) * 200);
            strcpy(m_url_real, "/log.html");
            strncpy(_M_real_file + len, m_url_real, strlen(m_url_real));

            free(m_url_real);
        }
        else if (*(p + 1) == '5')
        {
            char *m_url_real = (char *)malloc(sizeof(char) * 200);
            strcpy(m_url_real, "/picture.html");
            strncpy(_M_real_file + len, m_url_real, strlen(m_url_real));

            free(m_url_real);
        }
        else if (*(p + 1) == '6')
        {
            char *m_url_real = (char *)malloc(sizeof(char) * 200);
            strcpy(m_url_real, "/video.html");
            strncpy(_M_real_file + len, m_url_real, strlen(m_url_real));

            free(m_url_real);
        }
        else if (*(p + 1) == '7')
        {
            char *m_url_real = (char *)malloc(sizeof(char) * 200);
            strcpy(m_url_real, "/fans.html");
            strncpy(_M_real_file + len, m_url_real, strlen(m_url_real));

            free(m_url_real);
        }
        else
            strncpy(_M_real_file + len, _M_url, FILENAME_LEN - len - 1);

        if (stat(_M_real_file, &_M_file_stat) < 0)
            return NO_RESOURCE;

        if (!(_M_file_stat.st_mode & S_IROTH))
            return HTTP_CODE::FORBINDEN_REQUEST;

        if (S_ISDIR(_M_file_stat.st_mode))
            return HTTP_CODE::BAD_REQUEST;

        int fd = open(_M_real_file, O_RDONLY);
        _M_file_address = (char *)mmap(0, _M_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);
        return HTTP_CODE::FILE_REQUEST;

    }
    HttpConn::LINE_STATUS HttpConn::ParseLine()
    {
        /*******************************************
         __|__|__|__|__|__|__|__|__|__|__|__|__|__|
                 ^write_idx          ^read_idx
         *******************************************/
        char temp;
        for(;_M_check_idx<_M_read_idx;++_M_check_idx)
        {
            temp = _M_read_buf[_M_check_idx];
            if(temp == '\r')
            {
                if(_M_check_idx+1 == _M_read_idx)//'\r'后无内容，尚未读入一个完整的行
                    return LINE_OPEN;
                else if(_M_read_buf[_M_check_idx+1] == '\n')//'\r'后为一个'\n'，已解析一个完整行
                {
                    //'\r'和'\n'替换为'\0'
                    _M_read_buf[_M_check_idx++] = '\0';
                    _M_read_buf[_M_check_idx++] = '\0';
                }
                return LINE_BAD;//语法存在问题
            }
            else if(temp == '\n')
            {   
                //组成'\r''\n'，形成完整行，这个判断是必须的不会跟上一个中的冲突，
                //因为可能上一次读入'\r'后为LINE_OPEN退出，接受客户端信息更新后，发现为\n
                if(_M_read_buf[_M_check_idx-1] == '\r')
                {
                    //'\r'和'\n'替换为'\0'
                    _M_read_buf[_M_check_idx-1] = '\0';
                    _M_read_buf[_M_check_idx++] = '\0';
                    return LINE_OK;
                }
                return LINE_BAD;
            }
        }
    }
    void HttpConn::Unmap()
    {
        if(_M_file_address)
        {
            munmap(_M_file_address,_M_file_stat.st_size);
            _M_file_address = NULL;
        }
    }
    bool HttpConn::AddResponse(const char *format, ...)
    {
        if(_M_write_idx >= WRITE_BUFFER_SIZE)
        {
            return false;
        }
        va_list arg_list;
        va_start(arg_list,format);

        //写入的字符串超出的部分会被舍弃，但是返回值的len仍然计数
        int len = vsnprintf(_M_write_buf+_M_write_idx,WRITE_BUFFER_SIZE-1-_M_write_idx,format,arg_list);
        if(len >= WRITE_BUFFER_SIZE-1-_M_write_idx)
        {
            va_end(arg_list);
            return false;
        }
        _M_write_idx+=len;
        va_end(arg_list);
        LOG_INFO("request:%s",_M_write_buf);
        return true;
    }
    bool HttpConn::AddContent(const char *content)
    {
        return AddResponse("%s", content);
    }
    bool HttpConn::AddStatusLine(int status, const char *title)
    {
        return AddResponse("%s %d %s\r\n","HTTP/1.1",status,title);
    }
    bool HttpConn::AddHeaders(int content_length)
    {
        return AddContentLength(content_length) && AddLinger() &&AddBlankLine();
    }
    bool HttpConn::AddContentType()
    {
        return AddResponse("Content-Type:%s\r\n", "text/html");
    }
    bool HttpConn::AddContentLength(int content_length)
    {
        return AddResponse("Content-Length:%d\r\n", content_length);
    }
    bool HttpConn::AddLinger()
    {
        return AddResponse("Connection:%s\r\n", (_M_longer == true) ? "keep-alive" : "close");
    }
    bool HttpConn::AddBlankLine()
    {
        return AddResponse("%s", "\r\n");
    }
    void HttpConn::IniMySQLResult(SQLConnPool *connPool)
    {
        SQLConnRAII con(connPool);
        MYSQL* mysql = con.SQLConn();

        if(mysql_query(mysql,"SELECT username,passwd FROM user"))
        {
            LOG_ERROR("SELECT error:%s\n",mysql_error(mysql));
        }

        MYSQL_RES* res = mysql_store_result(mysql);
        while(MYSQL_ROW row = mysql_fetch_row(res))
        {
            users[row[0]] = row[1];
        }
    }
    void HttpConn::CloseConnection(bool realClose) 
    {
        if(realClose&&_M_sockfd != -1)
        {
            removefd(m_epollfd,_M_sockfd);
            _M_sockfd = -1;
            m_user_count--;
        }
    }
    
    void HttpConn::Process() 
    {
        HTTP_CODE read_ret = ProcessRead();
        if(HTTP_CODE::NO_REQUEST == read_ret)
        {
            //没有接受到完整的请求，继续关注sockfd上的读事件
            modfd(m_epollfd,_M_sockfd,EPOLLIN,_M_TRIGMode);
            return;//继续等待接受客户端数据
        }
        bool write_ret = ProcessWrite(read_ret);
        if(!write_ret)
        {
            CloseConnection();
        }
        //处理完成，可以开始写数据，关注何时可以开始，即可写事件
        modfd(m_epollfd,_M_sockfd,EPOLLOUT,_M_TRIGMode);
    }
    
    bool HttpConn::Write() 
    {
        int temp = 0;
        if(bytes_to_send == 0)
        {
            //没有数据可发，关注scokfd上的读事件
            modfd(m_epollfd,_M_sockfd,EPOLLIN,_M_TRIGMode);
            Init();
            return true;
        }
        while (1)
        {
            temp = writev(_M_sockfd,_M_ivp,_M_iv_count);
            if(temp<0)
            {
                if(errno == EAGAIN)
                {
                    //发送缓冲区已被写满，关注写事件，下次再来写
                    modfd(m_epollfd,_M_sockfd,EPOLLOUT,_M_TRIGMode);
                    return true;
                }
                Unmap();
                return false;
            }

            //deal with the left
            bytes_to_send-=temp;
            bytes_have_send+=temp;
            if(bytes_have_send>=_M_ivp[0].iov_len)
            {
                //first chunk has been wrote
                _M_ivp[0].iov_len = 0;
                _M_ivp[1].iov_base = _M_file_address+(bytes_have_send-_M_write_idx);//write_idx = write_buf.size()
                _M_ivp[1].iov_len = bytes_to_send;
            }
            else
            {
                //there is left of first chunk
                _M_ivp[0].iov_base = _M_write_buf+bytes_have_send;
                _M_ivp[0].iov_len -= bytes_have_send;
            }
            if(bytes_to_send<=0)
            {
                Unmap();
                modfd(m_epollfd,_M_sockfd,EPOLLIN,_M_TRIGMode);
                if(_M_longer)
                {
                    Init();
                    return true;
                }
                else{
                    return false;
                }
            }
        }
        
    }

}