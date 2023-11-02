#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "erproc.h"

int main(){

    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sock_addr = {0};
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        sock_addr.sin_port = htons(50000);

    Bind(sockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    Listen(sockfd, 10);

    char buf[100];
    while(1){
        socklen_t addrlen = sizeof(sock_addr);
        
        int listen_sockfd = Accept(sockfd, (struct sockaddr *) &sock_addr, &addrlen);

        Recv(listen_sockfd, buf, 1024, 0);
        //тестовый вывод, чтобы понять как парсить сообщения
        if(strcmp(buf, "exit") == 0)
            break;
        printf("Message:\n%s\n\n", buf);
        memset(buf, 0, sizeof(buf));
    }

    close(sockfd);

    return 0;
}