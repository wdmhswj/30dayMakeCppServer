#pragma once

#include <memory>

class InetAddress;

// Socket 自封装类
class Socket {
private:
    int fd; // 文件描述符
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);    // Socket 绑定地址
    // void bind(std::shared_ptr<InetAddress>);    // Socket 绑定地址
    void listen();              // 监听
    void setNonBlocking();      // 设置为非阻塞

    int accept(InetAddress*);   // 接收客户端socket
    // int accept(std::shared_ptr<InetAddress>);   // 接收客户端socket

    int getFd();
};