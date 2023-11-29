#include <sys/socket.h>

#include "include/dns.h"
#include "include/ether.h"
#include "include/ip.h"
#include "include/tcp.h"
#include "include/udp.h"
#include "include/icmp.h"
#include "include/http.h"

#define HTTP_PORT htons(80)
#define DNS_PORT htons(53)
#define BUF_LEN 65536

int main() {
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if(sockfd == -1) {
        perror("socket failed");
        return 1;
    }

    unsigned char packet[BUF_LEN];

    while(1) {
        ssize_t rcvlen = recv(sockfd, packet, BUF_LEN, 0);
        if(rcvlen == -1 || rcvlen == 0) {
            perror("recv failed");
            return 1;
        }

         if(rcvlen <= (long)sizeof(struct ethhdr)) {
            printf("ETHERNET DATA NONEXIST\n");
            continue;
        }
        struct ethhdr *ethh = (struct ethhdr *)packet;
        print_ethhdr(ethh);

        if(rcvlen <= (long)sizeof(struct ethhdr) + (long)sizeof(struct iphdr)) {
            printf("IP DATA NONEXIST\n");
            continue;
        }
        int iphdr_offset = sizeof(struct ethhdr);
        struct iphdr *iph = (struct iphdr *)(packet + iphdr_offset);
        print_iphdr(iph);

        switch(iph->protocol) {

        case IPPROTO_ICMP:
        {
            if(rcvlen <= (long)sizeof(struct ethhdr) + (long)sizeof(struct iphdr) + (long)sizeof(struct icmphdr)) {
                printf("ICMP DATA NONEXIST\n");
                continue;
            }
            int icmphdr_offset = iphdr_offset + (iph->ihl)*4;
            struct icmphdr *icmph = (struct icmphdr*)(packet + icmphdr_offset);
            print_icmphdr(icmph);
            break;
        }

        case IPPROTO_IGMP:
        {
            printf("-----------IGMP PROTOCOL-----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_IPIP:
        {
            printf("-----------IPIP PROTOCOL-----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_TCP:
        {
            if(rcvlen <= (long)sizeof(struct ethhdr) + (long)sizeof(struct iphdr) + (long)sizeof(struct tcphdr)) {
                printf("TCP DATA NONEXIST\n");
                continue;
            }
            int tcphdr_offset = iphdr_offset + (iph->ihl)*4;
            struct tcphdr *tcph = (struct tcphdr*)(packet + tcphdr_offset);

            int tcp_payload_offset = tcphdr_offset + (tcph->doff)*4;
            int tcp_payload_length = rcvlen - tcp_payload_offset;
            unsigned char *tcp_payload = (unsigned char *)(packet + tcp_payload_offset);
            print_tcphdr(tcph);

////////////////////////////////////////???
            if(tcph->dest == HTTP_PORT || tcph->source == HTTP_PORT) {
                if(tcp_payload_length > 0) {
                    print_http(tcp_payload, tcp_payload_length);
                }
            }
            break;
        }

        case IPPROTO_EGP:
        {
            printf("-----------EGP PROTOCOL------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_PUP:
        {
            printf("-----------PUP PROTOCOL------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_UDP:
        {
            if(rcvlen <= (long)sizeof(struct ethhdr) + (long)sizeof(struct iphdr) + (long)sizeof(struct udphdr)) {
                printf("UDP DATA NONEXIST\n");
                continue;
            }

            int udphdr_offset = iphdr_offset + (iph->ihl)*4;
            struct udphdr *udph = (struct udphdr*)(packet + udphdr_offset);
            print_udphdr(udph);

////////////////////////////////////////???
            if(udph->source == DNS_PORT || udph->dest == DNS_PORT) {
                int dnshdr_offset = udphdr_offset + sizeof(struct udphdr);
                struct dnshdr *dnsh = (struct dnshdr*)(packet + dnshdr_offset);

                int dns_payload_offset = dnshdr_offset + sizeof(struct dnshdr);
                int dns_payload_len = rcvlen - dns_payload_offset;
                unsigned char *dns_payload = (unsigned char *)(packet + dns_payload_offset);

                print_dns_header(dnsh);
                print_dns_data(dnsh, dns_payload, dns_payload_len);
            }
            break;
        }

        case IPPROTO_IDP:
        {
            printf("-----------IDP PROTOCOL------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_TP:
        {
            printf("------------TP PROTOCOL------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_DCCP:
        {
            printf("-----------DCCP PROTOCOL-----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_RSVP:
        {
            printf("-----------RSVP PROTOCOL-----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_GRE:
        {
            printf("-----------GRE PROTOCOL-----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_ESP:
        {
            printf("-----------ESP PROTOCOL------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_AH:
        {
            printf("-----------AH PROTOCOL-------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_MTP:
        {
            printf("-----------MTP PROTOCOL------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_BEETPH:
        {
            printf("-----------BEETPH PROTOCOL---------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_ENCAP:
        {
            printf("-----------ENCAP PROTOCOL----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_PIM:
        {
            printf("-----------PIM PROTOCOL------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_COMP:
        {
            printf("-----------COMP PROTOCOL-----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_SCTP:
        {
            printf("-----------SCTP PROTOCOL-----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_UDPLITE:
        {
            printf("-----------UDPLITE PROTOCOL--------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_MPLS:
        {
            printf("-----------MPLS PROTOCOL-----------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        case IPPROTO_ETHERNET:
        {
            printf("-----------RAW PACKET--------------\n");
            printf("\n");
            printf("-----------------------------------\n");
            break;
        }

        default:
        {
            printf("Unknown protocol\n");
            break;
        }
        }
    }
    return 0;
}