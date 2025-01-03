#pragma once 
#include <sys/epoll.h>
#include <cstdint>
#include <functional>

class EventLoop;
class Channel {
private:
    EventLoop* loop;    // 事件循环
    int fd;             // 监听的文件描述符
    uint32_t events;    // 监听的事件
    uint32_t revents;   // 正在发生的事件
    bool inEpoll;       // 是否在 epoll 中
    std::function<void()> callback; // 回调函数
public:
    Channel(EventLoop* _loop, int _fd);
    ~Channel();

    void handleEvent();     // 调用回调函数处理事件
    void enableReading();   // 允许监听读事件

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    // void setEvents(uint32_t);
    void setRevents(uint32_t);
    void setCallback(std::function<void()>);    // 设置回调函数
};