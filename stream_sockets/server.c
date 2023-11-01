#include "erproc.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(){

    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sock_addr = {0};
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_port = htons(50000);
        sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    Bind(sockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    Listen(sockfd, 5);

    socklen_t listen_socklen;
    int listen_sockfd = Accept(sockfd, (struct sockaddr *) &sock_addr, &listen_socklen);

    char buf[1024] = {0}; 
    Recv(listen_sockfd, buf, 1024, 0);
    printf("Message: %s\n", buf);
    
    strcpy(buf, "Answer from server");
    Send(listen_sockfd, buf, strlen(buf), 0);

    sleep(5);

    close(sockfd);

    return 0;
}