#include <sys/socket.h>
#include <arpa/inet.h>  // struct sockaddr_in
#include <string.h> // 写C语言代码
#include <stdio.h>

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));   // 确定对象被使用前已被初始化
    serv_addr.sin_family = AF_INET; // ipv4
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(sockfd, SOMAXCONN);  // SOMAXCONN: 最大监听队列长度

    struct sockaddr_in clnt_addr;   // 客户端
    socklen_t clnt_add_len = sizeof(clnt_addr);
    bzero(&clnt_addr, clnt_add_len);

    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_add_len);

    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    return 0;
}
