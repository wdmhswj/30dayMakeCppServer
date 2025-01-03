#pragma once
class Epoll;
class Channel;
class EventLoop {   // 事件循环类
private:
    Epoll* ep;      // epoll 封装类
    bool quit;      // 是否退出
public:
    EventLoop();
    ~EventLoop();

    void loop();    // 事件循环主体函数
    void updateChannel(Channel*);   // 更新 Channel
};