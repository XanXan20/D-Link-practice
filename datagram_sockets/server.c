#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "erproc.h"

int main(){

    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in sock_addr = {0};
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = INADDR_ANY;
        sock_addr.sin_port = htons(50000);

    Bind(sockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));

    struct sockaddr_in recv_addr = {0};
    socklen_t addrlen = sizeof(recv_addr);
    char recvbuf[1024] = {0};
    
    int count = 10;
    while(count--){
        recvfrom(sockfd, recvbuf, 1024, 0, (struct sockaddr *) &recv_addr, &addrlen);

        printf("[recv from %s:%d]%s \n",inet_ntoa(*(struct in_addr*)&recv_addr.sin_addr.s_addr),ntohs(recv_addr.sin_port),recvbuf);
    }
    
    close(sockfd);

    return 0;
}