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

    Connect(sockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));


    char buf[100];
    fgets(buf, sizeof(buf), STDIN_FILENO);
    Send(sockfd, buf, strlen(buf), 0);

    close(sockfd);

    return 0;
}