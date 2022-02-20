#pragma once

#include <netinet/in.h>

class InetAddress;
class Socket{
public:
    explicit Socket(int fd);
    ~Socket();
    //禁止拷贝语义
    Socket(const Socket&rhs) = delete;
    Socket& operator=(Socket& rhs) = delete;
    //允许移动语义
    Socket(Socket&& rhs);
    Socket& operator=(Socket&& rhs);
    
    int fd() const;
    void bind(const InetAddress&saddr);
    void listen();

    int connect(const InetAddress& saddr);
    void shutdownWrite();
    void setReuseAddr(bool on);
    void setTcpNoDelay(bool on);

    InetAddress getLocalAddr() const;
    InetAddress getPeerAddr() const;

    int recv(void* buf, int len);
    int send(const void* buf, int len);

    // factory methods
    static Socket createTCP(sa_family_t family);  // AF_INET or AF_INET6
    static Socket createUDP(sa_family_t family);  // AF_INET or AF_INET6
private:
    int sockfd_;
};




