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

void print_payload(unsigned char *payload, int payloadlen, int payloadoff){
    printf("Offset: %d\n", payloadoff);
    printf("Length: %d\n", payloadlen);
    for (int i = payloadoff; i < payloadlen; i++) {
        printf("%c", payload[i]);
    }
    printf("\n");
}

void print_http(unsigned char *payload, int payloadlen, int payloadoff){
    printf("------------HTTP HEADER------------\n");
    print_payload(payload, payloadlen, payloadoff);
    printf("-----------------------------------\n");
}

void print_dns(unsigned char *data, int payloadlen, int payloadoff){
    printf("-------------DNS HEADER------------\n");
    print_payload(data, payloadlen, payloadoff);
    printf("-----------------------------------\n");
}

void print_ethhdr(const struct ethhdr *ethh){
    printf("----------ETHERNET HEADER----------\n");
    printf("Destination MAC     : %x%x:%x%x:%x%x\n", ethh->h_dest[0], ethh->h_dest[1], ethh->h_dest[2], ethh->h_dest[3], ethh->h_dest[4], ethh->h_dest[5]);
    printf("Source MAC          : %x%x:%x%x:%x%x\n", ethh->h_source[0], ethh->h_source[1], ethh->h_source[2], ethh->h_source[3], ethh->h_source[4], ethh->h_source[5]);
    printf("Ethernet Type       : 0x%04X\n", ntohs(ethh->h_proto));
}

void print_iphdr(const struct iphdr *iph){
    unsigned char *sip = (unsigned char *)malloc(16);
    unsigned char *dip = (unsigned char *)malloc(16);
    inet_ntop(AF_INET, &iph->saddr, (char *)sip, 16);
    inet_ntop(AF_INET, &iph->daddr, (char *)dip, 16);

    printf("-------------IP HEADER-------------\n");
    printf("IP header length    : %u\n", iph->ihl);
    printf("IP version          : %u\n", iph->version);
    printf("Type of service     : %u\n", iph->tos);
    printf("Total length        : %u\n", iph->tot_len);
    printf("Package ID          : %u\n", iph->id);
    printf("Time to life        : %u\n", iph->ttl);
    printf("Protocol            : %u\n", iph->protocol);
    printf("Check sum           : %u\n", iph->check);
    printf("Send address        : %s\n", sip);
    printf("Destination address : %s\n", dip);
}

void print_tcphdr(const struct tcphdr *tcph){
    printf("------------TCP HEADER-------------\n");
    printf("Source port         :%u\n", ntohs(tcph->source));
    printf("Detsination port    :%u\n", ntohs(tcph->dest));
    printf("Sequence            :%u\n", tcph->seq);
    printf("Acknowledgment      :%u\n", tcph->ack_seq);
    printf("Header length       :%u\n", tcph->doff);
    printf("Window size         :%u\n", tcph->window);
    printf("Check sum           :%u\n", tcph->check);
    printf("-----------------------------------\n");    
}

void print_udphdr(const struct udphdr *udph){    
    printf("------------UDP HEADER-------------\n");
    printf("Source port         :%u\n", ntohs(udph->source));
    printf("Detsination port    :%u\n", ntohs(udph->dest));
    printf("Length              :%u\n", udph->len);
    printf("Check sum           :%u\n", udph->check);
    printf("-----------------------------------\n");
}

void print_icmphdr(const struct icmphdr *icmph){
    printf("------------ICMP HEADER------------\n");
    printf("Type                : %u\n", icmph->type);
    printf("Code                : %u\n", icmph->code);
    printf("Checksum            : %u\n", icmph->checksum);
    printf("Un id echo id       : %u\n", icmph->un.echo.id);
    printf("Un id echo sequence : %u\n", icmph->un.echo.sequence);
    printf("Un gateway          : %u\n", icmph->un.gateway);
    printf("Un frag _unused     : %u\n", icmph->un.frag.__unused);
    printf("Un frag mtu         : %u\n", icmph->un.frag.mtu);
    printf("-----------------------------------\n");
}

int main(){
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sockfd == -1){
        perror("socket failed");
        return 1;
    }

    unsigned char *packet = (unsigned char *)malloc(65536);

    while(1){
        int rcvlen = recv(sockfd, packet, BUF_LEN, 0);
        if(rcvlen == -1){
            perror("recv failed");
            return 1;
        }    
        
        struct ethhdr *ethh = (struct ethhdr *)packet;
        if(ntohs(ethh->h_proto) != ETH_P_IP) continue;
        //print_ethhdr(ethh);
        
        int iphdr_offset = sizeof(struct ethhdr);
        struct iphdr *iph = (struct iphdr *)(packet + iphdr_offset);
        //print_iphdr(iph);

        switch(iph->protocol){
            case IPPROTO_TCP:
                int tcphdr_offset = iphdr_offset + iph->ihl*4;
                struct tcphdr *tcph = (struct tcphdr*)(packet + tcphdr_offset);
                //print_tcphdr(tcph);
                

                int payload_offset = tcphdr_offset + tcph->doff*4;
                int payload_length = rcvlen - payload_offset;
                unsigned char *payload = (unsigned char *)(packet + payload_offset);
                
                if(ntohs(tcph->dest) == HTTP_PORT || ntohs(tcph->source) == HTTP_PORT){
                    printf("rcvlen: %d\n", rcvlen);
                    print_http(payload, payload_length, payload_offset);
                    //exit(EXIT_FAILURE);
                }
                else if(ntohs(tcph->source == DNS_PORT || ntohs(tcph->dest) == DNS_PORT)){
                    print_dns(payload, payload_length, payload_offset);
                    //exit(EXIT_FAILURE);
                }
                break;

            case IPPROTO_UDP:
                //struct udphdr *udph = (struct udphdr*)(packet + sizeof(struct ethhdr) + sizeof(struct udphdr)); 
                //print_udphdr(udph);
                break;

            case IPPROTO_ICMP:
                //struct icmphdr *icmph = (struct icmphdr*)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr)); 
                //print_icmphdr(icmph);
                //return 1;
                break;
        }

        //printf("\n");
    }

    return 0;
}