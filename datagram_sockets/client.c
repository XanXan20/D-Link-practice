#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "erproc.h"

int main(){

    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in sock_addr = {0};
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        sock_addr.sin_port = htons(50000);

    char buf[] = {"hello, world!"};

    int count = 5;
    while(count--){
        Sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &sock_addr, sizeof(sock_addr)); 

        sleep(1);
    }

    close(sockfd);

    return 0;
}