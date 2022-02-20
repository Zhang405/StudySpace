#include<netinet/in.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<arpa/inet.h>

#define BUFFSIZE 10
int setNonblocking(int fd){
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}
void enableRead(int epollfd, int fd,bool enableEt){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if(enableEt){
        event.events|=EPOLLET;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setNonblocking(fd);
}
void lt(epoll_event*events,int size,int epollfd,int listenfd){
    for(int i = 0;i < size;i++){
        int eventfd = events[i].data.fd;
        char buf[BUFFSIZE];
        if(eventfd == listenfd){
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof client_addr;
            int connfd = accept(listenfd,(struct sockaddr*)(&client_addr) ,&client_addr_len);
            enableRead(epollfd,connfd,false);
        }
        else if(events[i].events&EPOLLIN){
            printf("Trige onc\n");
            memset(buf,0,BUFFSIZE);
            int ret = recv(events[i].data.fd,buf,BUFFSIZE,0);
            if(ret<=0){
                close(events[i].data.fd);
                continue;
            }
            printf("Receive %d bytes from client : %s\n",ret,buf);
        }
        else{
            assert(false);
        }
    }
}

void etWithReadOnce(epoll_event*events,int size,int epollfd,int listenfd){
    for(int i = 0;i < size;i++){
        int eventfd = events[i].data.fd;
        char buf[BUFFSIZE];
        if(eventfd == listenfd){
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof client_addr;
            int connfd = accept(listenfd,(struct sockaddr*)(&client_addr) ,&client_addr_len);
            enableRead(epollfd,connfd,true);
        }
        else if(events[i].events&EPOLLIN){
            printf("Trige onc\n");
            while(true){
                memset(buf,0,BUFFSIZE);
                int ret = recv(events[i].data.fd,buf,BUFFSIZE,0);
                if(ret<=0){
                    if(errno == EAGAIN||errno == EWOULDBLOCK){
                        printf("Receive All,Wait Next\n");
                        break;
                    }
                    else if(ret = 0){
                        close(events[i].data.fd);
                        break;
                    }
                }
                else{
                    printf("Receive %d bytes from client : %s\n",ret,buf);
                }
            }

        }
        else{
            assert(false);
        }
    }
}
void etWithoutReadOnce(epoll_event*events,int size,int epollfd,int listenfd){
    for(int i = 0;i < size;i++){
        int eventfd = events[i].data.fd;
        char buf[BUFFSIZE];
        if(eventfd == listenfd){
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof client_addr;
            int connfd = accept(listenfd,(struct sockaddr*)(&client_addr) ,&client_addr_len);
            enableRead(epollfd,connfd,true);
        }
        else if(events[i].events&EPOLLIN){
            printf("Trige once\n");
            memset(buf,0,BUFFSIZE);
            int ret = recv(events[i].data.fd,buf,BUFFSIZE,0);
            if(ret<=0){
                if(errno == EAGAIN||errno == EWOULDBLOCK){
                    printf("Receive All,Wait Next\n");
                    break;
                }
                else if(ret = 0){
                    close(events[i].data.fd);
                    continue;
                }
            }
            else{
                printf("Receive %d bytes from client : %s\n",ret,buf);
            }
        }
        else{
            assert(false);
        }
    }
}
int main(int argc ,char** argv){
    if(argc < 2){
        printf("Usage: %s ip_address port_number\n",basename(argv[0]));
        return 1;
    }
    char* ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    bzero(&address,sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd>=0);
    int error = bind(listenfd,(struct sockaddr*)&address,sizeof address);
    assert(error!=-1);
    error = listen(listenfd,5);
    assert(error !=-1);
    epoll_event events[1000];
    int epoll_fd = epoll_create(5);
    enableRead(epoll_fd,listenfd,true);
    int stop = false;
    while(!stop){
        int ret = epoll_wait(epoll_fd,events,1000,-1);
        if(ret<0){
            printf("crashed\n");
            return 1;
        }
        //lt(events,ret,epoll_fd,listenfd);
        //etWithReadOnce(events,ret,epoll_fd,listenfd);
        etWithoutReadOnce(events,ret,epoll_fd,listenfd);
    }
    close(listenfd);
    return 0;
}
