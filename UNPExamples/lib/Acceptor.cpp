#include "../include/Acceptor.h"

#include "../include/InetAddress.h"
#include "../include/TCPStream.h"

#include <stdio.h>
#include <sys/socket.h>
#include "../include/log.h"
Acceptor::Acceptor(const InetAddress& listenAddr)
  : listenSock_(Socket::createTCP(listenAddr.family()))
{
  listenSock_.setReuseAddr(true);
  listenSock_.bind(listenAddr);
  listenSock_.listen();
}

TcpStreamPtr Acceptor::accept()
{
  // FIXME: use accept4
  LOG_INFO("listen on %s\n",listenSock_.getLocalAddr().toIpPort().c_str());
  int sockfd = ::accept(listenSock_.fd(), NULL, NULL);
  LOG_INFO("accept socket fd = %d\n",sockfd);
  if (sockfd >= 0)
  {
    LOG_INFO("Accept One Client,socketfd = %d\n",listenSock_.fd());
    return TcpStreamPtr(new TcpStream(Socket(sockfd)));
  }
  else
  {
    perror("Acceptor::accept");
    return TcpStreamPtr();
  }
}

Socket Acceptor::acceptSocketOrDie()
{
  // FIXME: use accept4
  int sockfd = ::accept(listenSock_.fd(), NULL, NULL);
  if (sockfd >= 0)
  {
    return Socket(sockfd);
  }
  else
  {
    perror("Acceptor::acceptSocketOrDie");
    abort();
  }
}