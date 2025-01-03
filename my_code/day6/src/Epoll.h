#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll {
private:
    int epfd;   // epoll 文件描述符
    struct epoll_event* events; // 指向 epoll_event 数组的指针
public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);    // 添加要监听的文件描述符
    void updateChannel(Channel*);       // 更新某文件描述符要监听的事件
    std::vector<Channel*> poll(int timeout=-1); // 返回存在待处理事件的Channel
};