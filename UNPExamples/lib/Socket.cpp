#include "../include/Socket.h"
#include "../include/InetAddress.h"
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

Socket::Socket(int fd){
    sockfd_ = fd;
}
Socket::~Socket(){
    if(sockfd_>0){
        int ret = ::close(sockfd_);
        assert(ret==0);
    }
}
//允许移动语义
Socket::Socket(Socket&& rhs){
    sockfd_ = rhs.sockfd_;
    rhs.sockfd_ = -1;
}
Socket& Socket::operator=(Socket&& rhs){
    sockfd_ = rhs.sockfd_;
    rhs.sockfd_ = -1;
    return *this;
}

int Socket::fd() const{
    return sockfd_;
}
void Socket::bind(const InetAddress&saddr){
    int error = ::bind(sockfd_,saddr.get_sockaddr(),saddr.length());
    if(error != 0){
        perror("Socket::bind");
        abort();
    }
}
void Socket::listen(){
    int error = ::listen(sockfd_,5);
    if(error != 0){
        perror("Socket::listen");
        abort();
    }
}

int Socket::connect(const InetAddress& saddr){
    int error = ::connect(sockfd_,saddr.get_sockaddr(),saddr.length());
    if(error != 0){
        perror("Socket::connect");
        abort();
    }
    return 0;
}
void Socket::shutdownWrite(){
    int error = ::shutdown(sockfd_,SHUT_WR);
    if(error != 0){
        perror("Socket::shutdown");
    }
}
void Socket::setReuseAddr(bool on){
    int optval = on ? 1 : 0;
    if (::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                    &optval, sizeof optval) < 0)
    {
        perror("Socket::setReuseAddr");
    }
}
void Socket::setTcpNoDelay(bool on){
    int optval = on ? 1 : 0;
    if (::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
                &optval, static_cast<socklen_t>(sizeof optval)) < 0)
    {
        perror("Socket::setTcpNoDelay");
    }
}

InetAddress Socket::getLocalAddr() const{
    struct sockaddr_storage localAddr;
    socklen_t len;
    len = sizeof(localAddr);
    struct sockaddr* addr = reinterpret_cast<struct sockaddr*>(&localAddr);
    int error = ::getsockname(sockfd_,addr,&len);
    if(error != 0){
        perror("Socket::getLocakAddr");
    }
    return InetAddress(*addr);
}
InetAddress Socket::getPeerAddr() const{
    struct sockaddr_storage peerAddr;
    socklen_t len;
    len = sizeof(peerAddr);
    struct sockaddr* addr = reinterpret_cast<struct sockaddr*>(&peerAddr);
    int error = ::getpeername(sockfd_,addr,&len);
    if(error != 0){
        perror("Socket::getLocakAddr");
    }
    return InetAddress(*addr);
}

int Socket::recv(void* buf, int len){
    return ::recv(sockfd_,buf,len,0);
}
int Socket::send(const void* buf, int len){
    return ::send(sockfd_,buf,len,0);
}

// factory methods
Socket Socket::createTCP(sa_family_t family){
    int fd = ::socket(family,SOCK_STREAM | SOCK_CLOEXEC,IPPROTO_TCP);
    if(fd == -1){
        perror("Socket::createTCP");
        abort();
    }
    return Socket(fd);
}  // AF_INET or AF_INET6
Socket Socket::createUDP(sa_family_t family){
    int fd = ::socket(family,SOCK_DGRAM | SOCK_CLOEXEC,IPPROTO_TCP);
    return Socket(fd);
}



