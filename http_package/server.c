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

#define BUF_SIZE 65536

int main() {
    unsigned char buf[BUF_SIZE];
    int sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_TCP);
    if(sockfd == -1){
        perror("sock error");
        exit(EXIT_FAILURE);
    }
    while(1){
        int recvlen = recv(sockfd, buf, BUF_SIZE, 0);
        if(recvlen < 0){
            perror("recv failure");
            exit(EXIT_FAILURE);
        }

        struct iphdr *iph = (struct iphdr *)buf;
        if(iph->protocol != IPPROTO_TCP) continue;
        printf("tcp header\n");

        struct tcphdr *tcph = (struct tcphdr *)(buf + sizeof(struct iphdr));
        if(ntohs(tcph->dest) != 80 && ntohs(tcph->source) != 80) continue;
        printf("http header\n");

        unsigned char *payload = buf + sizeof(struct iphdr) + sizeof(struct tcphdr);
        int i = 0;
        while(payload[i] != '\0'){

            if(payload[i]>=32 && payload[i]<=127)
					printf("%c",(unsigned char)payload[i]); 
				
				else printf(".");
            i++;
        }
        printf("\n");
        return 1;
    }
    return 0;   
}