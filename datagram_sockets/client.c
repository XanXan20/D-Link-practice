#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "erproc.h"

int main(){

    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in dest_addr = {0};
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        dest_addr.sin_port = htons(50000);

    char buf[] = {"hello, world!"};

    int count = 5;
    while(count--){
        int res = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
	    printf("ret = %d \n", res);

        sleep(1);
    }

    close(sockfd);

    return 0;
}