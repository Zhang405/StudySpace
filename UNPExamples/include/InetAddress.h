#pragma once
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>


class InetAddress{    
public:
    InetAddress(){addr_.sin_family = AF_UNSPEC;}
    InetAddress(std::string ip,uint16_t port);
    explicit InetAddress(uint16_t port,bool ipv6 = false);
    explicit InetAddress(const struct sockaddr& saddr);
    
    sa_family_t family() const{return addr_.sin_family;}
    uint32_t port(){return ntohs(addr_.sin_port);}
    void setPort(uint32_t port){addr_.sin_port = htons(port);}

    std::string toIp();
    std::string toIpPort();

    const struct sockaddr* get_sockaddr() const{
        return reinterpret_cast<const struct sockaddr*>(&addr6_);
    }
    socklen_t length() const{
        return family() == AF_INET6?sizeof(addr6_):sizeof(addr_);
    }
    static bool resolve(std::string hostname,uint32_t port,InetAddress*);
    static std::vector<InetAddress> resolveAll(std::string hostname,uint32_t port = 0);

    bool operator==(const InetAddress& rhs) const;
private:
    union{
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    };  
};




