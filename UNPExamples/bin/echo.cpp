#include "../include/InetAddress.h"
#include "../include/Acceptor.h"
#include "../include/TCPStream.h"
#include "../include/log.h"
#include <thread>
void echoRS(TcpStreamPtr stream,int count){
    char buf[4096];
    int n_recevice = 0;
    while((n_recevice = stream->receiveSome(buf, 4096))>0){
        LOG_INFO("recevice:%s",buf);
        int n_send = stream->sendAll(buf,n_recevice);
        LOG_INFO("send:%dbytes",n_send);
        if(n_send<n_recevice){
            break;
        }
    }
    LOG_INFO("Thread for %d client ended\n",count);

}

int main(){
    //创建监听套接字
    InetAddress addr(3007);
    Acceptor accept(addr);
    printf("Listen on port 3307\n");
    printf("Accepting...\n");
    int count = 0;
    while(true){
        TcpStreamPtr tcpStream = accept.accept();
        tcpStream->setTcpNoDelay(true);
        printf("Accept the No.%d client",count);
        std::thread thr(echoRS,std::move(tcpStream),count++);
        thr.detach();
    }
    return 0;
}