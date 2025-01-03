#pragma once

class EventLoop;
class Socket;
class Server {
private:
    EventLoop* loop;    // 事件循环
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);              // 处理读事件
    void newConnection(Socket *serv_sock);  // 处理新连接
};