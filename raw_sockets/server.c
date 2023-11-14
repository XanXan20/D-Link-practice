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

#define HTTP_PORT 80
#define DNS_PORT 53
#define BUF_LEN 65536

void print_payload(unsigned char *payload){
    size_t str_len = strlen((char *)payload);
    size_t i = 0;
    while(i < str_len){
        if(payload[i] > 31 && payload[i] < 127)
            printf("%c", payload[i]);
        i++;
    }
    printf("\n");
}

void print_http(unsigned char *payload){
    printf("------------HTTP HEADER------------\n");
    print_payload(payload);
    printf("-----------------------------------\n");
}

void print_dns(unsigned char *data){
    printf("-------------DNS HEADER------------\n");
    print_payload(data);
    printf("-----------------------------------\n");
}

void print_iphdr(struct iphdr iph){
    char *sip = (char *)malloc(16);
    char *dip = (char *)malloc(16);
    inet_ntop(AF_INET, &iph.saddr, (char *)sip, 16);
    inet_ntop(AF_INET, &iph.daddr, (char *)dip, 16);

    printf("-------------IP HEADER-------------\n");
    printf("IP header length    : %u\n", iph.ihl);
    printf("IP version          : %u\n", iph.version);
    printf("Type of service     : %u\n", iph.tos);
    printf("Total length        : %u\n", iph.tot_len);
    printf("Package ID          : %u\n", iph.id);
    printf("Time to life        : %u\n", iph.ttl);
    printf("Protocol            : %u\n", iph.protocol);
    printf("Check sum           : %u\n", iph.check);
    printf("Send address        : %s\n", sip);
    printf("Destination address : %s\n", dip);
}

void print_tcphdr(struct tcphdr tcph){
    printf("------------TCP HEADER-------------\n");
    printf("Source port         :%u\n", ntohs(tcph.source));
    printf("Detsination port    :%u\n", ntohs(tcph.dest));
    printf("Sequence            :%u\n", tcph.seq);
    printf("Acknowledgment      :%u\n", tcph.ack_seq);
    printf("Header length       :%u\n", tcph.doff);
    printf("Window size         :%u\n", tcph.window);
    printf("Check sum           :%u\n", tcph.check);
    printf("-----------------------------------\n");    
}

void print_udphdr(struct udphdr udph){    
    printf("------------UDP HEADER-------------\n");
    printf("Source port         :%u\n", ntohs(udph.source));
    printf("Detsination port    :%u\n", ntohs(udph.dest));
    printf("Length              :%u\n", udph.len);
    printf("Check sum           :%u\n", udph.check);
    printf("-----------------------------------\n");
}

void print_icmphdr(struct icmphdr icmph){
    printf("------------ICMP HEADER------------\n");
    printf("Type                : %u\n", icmph.type);
    printf("Code                : %u\n", icmph.code);
    printf("Checksum            : %u\n", icmph.checksum);
    printf("Un id echo id       : %u\n", icmph.un.echo.id);
    printf("Un id echo sequence : %u\n", icmph.un.echo.sequence);
    printf("Un gateway          : %u\n", icmph.un.gateway);
    printf("Un frag _unused     : %u\n", icmph.un.frag.__unused);
    printf("Un frag mtu         : %u\n", icmph.un.frag.mtu);
    printf("Un reserved         : %hhn\n", icmph.un.reserved);
    printf("-----------------------------------\n");
}

int main(){

    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sockfd == -1){
        perror("socket failed");
        return 1;
    }

    unsigned char *buf = (unsigned char *)malloc(65536);
    struct sockaddr_in src_addr = {0};
    socklen_t addr_len = sizeof(src_addr);

    while(1){
        int rcvlen = recvfrom(sockfd, buf, BUF_LEN, 0, (struct sockaddr *)&src_addr, &addr_len);
        if(rcvlen == -1){
            perror("recv failed");
            return 1;
        }    
        
        struct ethhdr *ethh = (struct ethhdr *)buf;
        if(ethh->h_proto == ETH_P_IP) continue;
        
        struct iphdr *iph = (struct iphdr *)(buf + sizeof(struct ethhdr));
        print_iphdr(*iph);

        switch(iph->protocol){
            case IPPROTO_TCP:
                struct tcphdr *tcph = (struct tcphdr*)(buf + sizeof(struct ethhdr) + (iph->ihl*4));
                print_tcphdr(*tcph);
                
                unsigned char *payload = (unsigned char *)(buf + sizeof(struct ethhdr) + (iph->ihl*4) + (tcph->doff*4));
                
                if(ntohs(tcph->source) == HTTP_PORT){
                    print_http(payload);
                    exit(EXIT_FAILURE);
                }
                else if(ntohs(tcph->source == DNS_PORT || ntohs(tcph->dest) == DNS_PORT)){
                    print_dns(payload);
                    //exit(EXIT_FAILURE);
                }
                break;

            case IPPROTO_UDP:
                struct udphdr *udph = (struct udphdr*)(buf + sizeof(struct ethhdr) + sizeof(struct udphdr)); 
                print_udphdr(*udph);
                break;

            case IPPROTO_ICMP:
                struct icmphdr *icmph = (struct icmphdr*)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr)); 
                print_icmphdr(*icmph);
                return 1;
                break;
        }
    }

    return 0;
}