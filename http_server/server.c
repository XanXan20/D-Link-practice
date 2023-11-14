#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>

int main(){

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == -1){
        perror("sock failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sock_addr = {0};
        sock_addr.sin_addr.s_addr = INADDR_ANY;
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_port = htons(8080);
    socklen_t addr_len = sizeof(sock_addr);

    int bindres = bind(sockfd, (struct sockaddr *)&sock_addr, addr_len);
    if(bindres == -1){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int listenres = listen(sockfd, 5);
    if(listenres == -1){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    char *buf = (char *)malloc(10000);
    while(1){
        int acceptres = accept(sockfd, (struct sockaddr *)&sock_addr, &addr_len);
        if(acceptres == -1){
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        int recvres = recv(sockfd, buf, 10000, 0);
        if(resvres == -1){
            perror("recv failed");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", buf);
    }

    return 0;
}