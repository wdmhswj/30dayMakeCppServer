#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop(): ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop() {
    while (!quit) {
        auto chs = ep->poll();  // 获取
        for(auto ch: chs) {
            ch->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* ch) {
    ep->updateChannel(ch);
}