#include "../include/InetAddress.h"
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <netdb.h>
#include "../include/log.h"

InetAddress::InetAddress(std::string ip,uint16_t port){
    //connect时
    setPort(port);
    int error = ::inet_pton(AF_INET,ip.c_str(),&addr_.sin_addr);

    addr_.sin_family = AF_INET;
    assert(error == 1);//不符合ip地址的格式
}

InetAddress::InetAddress(uint16_t port,bool ipv6 /*= false*/){
    bool loopbackOnly = false;
    if(ipv6){
        bzero(&addr6_,sizeof(addr6_));
        setPort(port);
        addr6_.sin6_addr = in6addr_any;
        addr_.sin_family = AF_INET;
    }
    else{
        bzero(&addr_,sizeof(addr_));
        setPort(port);
        addr_.sin_addr.s_addr = htonl(INADDR_ANY);
        addr_.sin_family = AF_INET;
        LOG_INFO("socket.addr = %ld,port = %d\n",ntohl(addr_.sin_addr.s_addr),port);
    }
}

InetAddress::InetAddress(const struct sockaddr& saddr){
    if(saddr.sa_family == AF_INET){
        memcpy(&addr_,&saddr,sizeof(addr_));
    }
    else if(saddr.sa_family == AF_INET6){
        memcpy(&addr6_,&saddr,sizeof(addr6_));
    }
    else{
        assert(false);
    }
}
std::string InetAddress::toIp(){
    char buf[64];
    memset(buf,0,64);
    if(family() == AF_INET){
        ::inet_ntop(AF_INET,&addr_.sin_addr,buf,sizeof(addr_));
    }
    else if(family() == AF_INET6){
        ::inet_ntop(AF_INET6,&addr6_.sin6_addr,buf,sizeof(addr_));
    }
    return std::string(buf);
}
std::string InetAddress::toIpPort(){
    char buf[32] = "";
    snprintf(buf,sizeof(buf),":%u",port());

    if(family() == AF_INET6){
        return "["+toIp()+"]"+buf;
    }
    return toIp()+buf;
}

bool InetAddress::resolve(std::string hostname,uint32_t port,InetAddress* netAddr){
    std::vector<InetAddress> addrs = resolveAll(hostname,port);
    if(addrs.empty()){
        return false;
    }
    *netAddr = addrs[0];
    return true;
}
std::vector<InetAddress> InetAddress::resolveAll(std::string hostname,uint32_t port/* = 0*/){
    std::vector<InetAddress> res;
    struct addrinfo* result = NULL;
    int error = getaddrinfo(hostname.c_str(),NULL,NULL,&result);
    if(error != 0){
        if(error == EAI_SYSTEM){
            perror("InetAddress::resolve");
        }
        else{
            fprintf(stderr,"InetAddress::resolve: %s\n",gai_strerror(error));
        }
        return res;
    }
    for(struct addrinfo* ai = result; ai != NULL;ai = ai->ai_next){
        InetAddress addr(*ai->ai_addr);
        addr.setPort(port);
        res.push_back(addr);
    }
    freeaddrinfo(result);
    return res;
}

bool InetAddress::operator==(const InetAddress& rhs) const{
    if (family() == rhs.family())
    {
        if (family() == AF_INET)
        {
        return addr_.sin_port == rhs.addr_.sin_port &&
            addr_.sin_addr.s_addr == rhs.addr_.sin_addr.s_addr;
        }
        else if (family() == AF_INET6)
        {
        return addr6_.sin6_port == rhs.addr6_.sin6_port &&
            memcmp(&addr6_.sin6_addr, &rhs.addr6_.sin6_addr, sizeof addr6_.sin6_addr) == 0;
        }
    }
    return false;
}



