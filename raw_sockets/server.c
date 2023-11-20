#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>

#define HTTP_PORT 80
#define DNS_PORT 53
#define BUF_LEN 65536

struct dnshdr{
    uint16_t id;

    uint8_t rd : 1; 
    uint8_t tc : 1; 
    uint8_t aa : 1; 
    uint8_t opcode : 4; 
    uint8_t qr : 1; 
    uint8_t rcode : 4; 
    uint8_t z : 3; 
    uint8_t ra : 1; 

    uint16_t qdcount; 
    uint16_t ancount; 
    uint16_t nscount;
    uint16_t arcount; 
};

struct dns_qsection
{
    uint16_t type;
    uint16_t class;
};

struct dns_ansection
{
    uint16_t type;
    uint16_t class;
    uint16_t ttl;
    uint16_t data_len;
};

struct dns_nssection
{
    uint16_t type;
    uint16_t class;
    uint16_t ttl;
    uint16_t data_len;
};

struct dns_arsection
{
    uint16_t type;
    uint16_t class;
    uint16_t ttl;
    uint16_t data_len;
};

void print_payload(unsigned char *payload, int payloadlen){
    for (int i = 0; i < payloadlen; i++) {
        printf("%c", payload[i]);
    }
    printf("\n");
}

void print_ethhdr(const struct ethhdr *ethh){
    printf("\n----------ETHERNET HEADER----------\n");
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

void print_dns_header(struct dnshdr *dnsh){

    dnsh->id = ntohs(dnsh->id);
    dnsh->qdcount = ntohs(dnsh->qdcount);
    dnsh->ancount = ntohs(dnsh->ancount);
    dnsh->nscount = ntohs(dnsh->nscount);
    dnsh->arcount = ntohs(dnsh->arcount);

    printf("-------------DNS HEADER------------\n");
    printf("ID                  : %u\n\n", dnsh->id);

    printf("QR                  : %u\n", dnsh->qr);
    printf("OPCODE              : %u\n", dnsh->opcode);
    printf("AA                  : %u\n", dnsh->aa);
    printf("TC                  : %u\n", dnsh->tc);
    printf("RD                  : %u\n", dnsh->rd);
    printf("RA                  : %u\n", dnsh->ra);
    printf("Z                   : %u\n", dnsh->z);
    printf("RCODE               : %u\n\n", dnsh->rcode);

    printf("QDCOUNT             : %u\n", dnsh->qdcount);
    printf("ANCOUNT             : %u\n", dnsh->ancount);
    printf("NSCOUNT             : %u\n", dnsh->nscount);
    printf("ARCOUNT             : %u\n", dnsh->arcount);
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

void print_http(unsigned char *payload, int payloadlen){
    printf("------------HTTP HEADER------------\n");
    print_payload(payload, payloadlen);
    printf("-----------------------------------\n\n");
}

uint8_t print_section_name(unsigned char *payload){
    uint8_t first_byte = *payload;
            
    uint8_t label = (first_byte & 0xC0) >> 6;
    if(label == 3){
        printf("archived label\n");
        exit(EXIT_FAILURE);
    }
    uint8_t len = first_byte & 0x3F; 
    uint32_t offset = 1;
    uint8_t *name = payload + offset;

    printf("Data                 : ");
    for(int j = 0; j <= len; j++){
        printf("%c", (char)name[j]);
    }

    printf("\n");  
    return offset + 1;  
}

void print_dns_data(const struct dnshdr *dnsh, unsigned char *payload, int payloadlen){
    uint32_t offset = 0;
    if(dnsh->qdcount > 0){
        printf("------------QUESTION SECTION-------\n");
        for(int i = 0; i < dnsh->qdcount; i++){
            offset += print_section_name(payload);
            payloadlen -= offset;
                        
            struct dns_qsection *qsec = (struct dns_qsection *)(payload + offset);
            
            offset += sizeof(struct dns_qsection);
            payloadlen -= sizeof(struct dns_qsection);

            printf("Question class       : 0x%04X\n", ntohs(qsec->class));
            printf("Question type        : 0x%04X\n", ntohs(qsec->type));
            printf("-----------------------------------\n");   
        }
    }
    if(dnsh->ancount > 0){
        printf("------------ANSWER SECTION---------\n");
        for(int i = 0; i < dnsh->ancount; i++){ 
            offset += print_section_name(payload);
            payloadlen -= offset;
            
            struct dns_ansection *ansec = (struct dns_ansection *)(payload + offset);

            offset += sizeof(struct dns_ansection);
            payloadlen -= sizeof(struct dns_ansection);

            printf("Answer type         : 0x%04X\n", ntohs(ansec->type));
            printf("Answer class        : 0x%04X\n", ntohs(ansec->class));
            printf("Answer ttl          : %u\n", ntohs(ansec->ttl));
            printf("Answer data_len     : %u\n", ntohs(ansec->data_len));
            //print_payload(payload, (int)(ansec->data_len)); большая часть вывода требует специальной обработки поэтому решил закомментировать
            payloadlen -= ansec->data_len;
            offset += ntohs(ansec->data_len);
            printf("-----------------------------------\n");    
        }
    }
    if(dnsh->nscount > 0){
        printf("----------AUTHORITY SECTION--------\n");
        for(int i = 0; i < dnsh->nscount; i++){
            offset += print_section_name(payload);
            payloadlen -= offset;

            struct dns_nssection *nssec = (struct dns_nssection *)(payload + offset);

            offset += sizeof(struct dns_nssection);
            payloadlen -= sizeof(struct dns_nssection);

            printf("Autority type       : 0x%04X\n", ntohs(nssec->type));
            printf("Autority class      : 0x%04X\n", ntohs(nssec->class));
            printf("Autority ttl        : %u\n", ntohs(nssec->ttl));
            printf("Autority data_len   : %u\n", ntohs(nssec->data_len));
            //print_payload(payload, (int)(ansec->data_len)); 
            payloadlen -= nssec->data_len;
            offset += ntohs(nssec->data_len);
            printf("-----------------------------------\n");    
        }
    }
    if(dnsh->arcount > 0){
        printf("----------ADDITIONAL SECTION-------\n");
        for(int i = 0; i < dnsh->arcount; i++){
            offset += print_section_name(payload);
            payloadlen -= offset;

            struct dns_arsection *arsec = (struct dns_arsection *)(payload + offset);

            offset += sizeof(struct dns_arsection);
            payloadlen -= sizeof(struct dns_arsection);

            printf("Autority type       : 0x%04X\n", ntohs(arsec->type));
            printf("Autority class      : 0x%04X\n", ntohs(arsec->class));
            printf("Autority ttl        : %u\n", ntohs(arsec->ttl));
            printf("Autority data_len   : %u\n", ntohs(arsec->data_len));
            //print_payload(payload, (int)(ansec->data_len)); 
            payloadlen -= arsec->data_len;
            offset += ntohs(arsec->data_len);
            printf("-----------------------------------\n");    
        }
    }
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
        if(rcvlen == -1 || rcvlen == 0){
            perror("recv failed");
            return 1;
        }    
        
        struct ethhdr *ethh = (struct ethhdr *)packet;
        if(ntohs(ethh->h_proto) != ETH_P_IP) continue;
        
        int iphdr_offset = sizeof(struct ethhdr);
        struct iphdr *iph = (struct iphdr *)(packet + iphdr_offset);

        switch(iph->protocol){
            case IPPROTO_TCP:
                int tcphdr_offset = iphdr_offset + (iph->ihl)*4;
                struct tcphdr *tcph = (struct tcphdr*)(packet + tcphdr_offset);                

                int tcp_payload_offset = tcphdr_offset + (tcph->doff)*4;
                int tcp_payload_length = rcvlen - tcp_payload_offset;
                unsigned char *tcp_payload = (unsigned char *)(packet + tcp_payload_offset);
                
                if(ntohs(tcph->dest) == HTTP_PORT || ntohs(tcph->source) == HTTP_PORT){
                    if(tcp_payload_length > 0){
                        print_ethhdr(ethh);
                        print_iphdr(iph);
                        print_tcphdr(tcph);
                        print_http(tcp_payload, tcp_payload_length);
                    }                         
                }
                break;

            case IPPROTO_UDP:
                int udphdr_offset = iphdr_offset + (iph->ihl)*4;
                struct udphdr *udph = (struct udphdr*)(packet + udphdr_offset);

                int dnshdr_offset = udphdr_offset + sizeof(struct udphdr);
                struct dnshdr *dnsh = (struct dnshdr*)(packet + dnshdr_offset);

                int dns_payload_offset = dnshdr_offset + sizeof(struct dnshdr);
                int dns_payload_len = rcvlen - dns_payload_offset;
                unsigned char *dns_payload = (unsigned char *)(packet + dns_payload_offset);

                if(ntohs(udph->source) == DNS_PORT || ntohs(udph->dest) == DNS_PORT){
                    print_ethhdr(ethh);
                    print_iphdr(iph);
                    print_udphdr(udph);
                    print_dns_header(dnsh);
                    print_dns_data(dnsh, dns_payload, dns_payload_len);
                }
                break;

            case IPPROTO_ICMP:
                int icmphdr_offset = iphdr_offset + (iph->ihl)*4;
                struct icmphdr *icmph = (struct icmphdr*)(packet + icmphdr_offset); 
                print_ethhdr(ethh);
                print_iphdr(iph);
                print_icmphdr(icmph);
                break;
        }
    }
    return 0;
}