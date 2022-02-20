#pragma once
#include "Socket.h"

#include <memory>

class InetAddress;

class TcpStream;
typedef std::unique_ptr<TcpStream> TcpStreamPtr;

class Acceptor
{
 public:
  explicit Acceptor(const InetAddress& listenAddr);

  ~Acceptor() = default;
  Acceptor(Acceptor&&) = default;
  Acceptor& operator=(Acceptor&&) = default;

  // thread safe
  TcpStreamPtr accept();
  Socket acceptSocketOrDie();

 private:
  Socket listenSock_;
};
