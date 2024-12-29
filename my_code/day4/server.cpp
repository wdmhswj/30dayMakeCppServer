#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"

#include <memory>

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setNonBlocking(int fd) {
    // 先使用 F_GETFL 获取 fd 文件描述符的标志，再使用 F_SETFL 设置为非阻塞
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void handleReadEvent(int);

int main() {
    Socket *serv_sock = new Socket();
    // auto serv_sock = std::make_shared<Socket>();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 8888);
    // auto serv_addr = std::make_shared<InetAddress>("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();

    Epoll *ep = new Epoll();
    serv_sock->setNonBlocking();
    ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);
    while (true) {
        std::vector<epoll_event> events = ep->poll();
        int nfds = events.size();
        for(int i=0; i<nfds; ++i) {
            if (events[i].data.fd == serv_sock->getFd()) {  // 新客户端连接
                InetAddress* clnt_addr = new InetAddress(); //会发生内存泄露！没有delete
                // auto clnt_addr = std::make_shared<InetAddress>();
                Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));       //会发生内存泄露！没有delete
                // auto clnt_sock = std::make_shared<Socket>(serv_sock->accept(clnt_addr));
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
                clnt_sock->setNonBlocking();
                ep->addFd(clnt_sock->getFd(), EPOLLIN | EPOLLET);

                
            } else if(events[i].events & EPOLLIN) { // 可读事件
                handleReadEvent(events[i].data.fd);
            } else {    // 其他事件，之后的版本实现
                printf("something else happened\n");
            }
        }
    }

    // delete serv_sock;
    // delete serv_addr;
    return 0;
}

void handleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while (true) {  // 由于使用非阻塞IO，读取客户端buffer，一次性读取buf大小数据，直到全部读取完毕
                    // 非阻塞 I/O：当 socket 被设置为非阻塞模式时，read() 调用不会因为没有数据可读而阻塞程序。相反，如果没有数据，read() 会立即返回并设置 errno 为 EAGAIN 或 EWOULDBLOCK。
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}