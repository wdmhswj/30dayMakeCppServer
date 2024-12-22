#include <sys/socket.h>
#include <arpa/inet.h>  // struct sockaddr_in
#include <string.h> // 写C语言代码
#include <stdio.h>
#include "util.h"
#include <unistd.h>

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd==-1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));   // 确定对象被使用前已被初始化
    serv_addr.sin_family = AF_INET; // ipv4
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN)==-1, "socket listen error");  // SOMAXCONN: 最大监听队列长度

    struct sockaddr_in clnt_addr;   // 客户端
    socklen_t clnt_add_len = sizeof(clnt_addr);
    bzero(&clnt_addr, clnt_add_len);

    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_add_len);
    errif(clnt_sockfd==-1, "socket accept error");

    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    
    while(true) {
        char buf[1024];
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
        if(read_bytes>0){
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf));
        }else if(read_bytes==0){
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        }else if(read_bytes==-1) {
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
    }

    close(sockfd);
    return 0;
}
