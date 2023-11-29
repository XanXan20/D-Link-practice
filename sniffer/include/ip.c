#include "ip.h"

void print_iphdr(const struct iphdr *iph) {
    unsigned char sip[INET_ADDRSTRLEN];
    unsigned char dip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &iph->saddr, (char *)sip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &iph->daddr, (char *)dip, INET_ADDRSTRLEN);

    printf("-------------IP HEADER-------------\n");
    printf("IP header length    : %u\n", iph->ihl);
    printf("IP version          : %u\n", iph->version);
    printf("Type of service     : %u\n", iph->tos);
    printf("Total length        : %u\n", ntohs(iph->tot_len));
    printf("Package ID          : %u\n", ntohs(iph->id));
    printf("Time to life        : %u\n", iph->ttl);
    printf("Protocol            : %u\n", iph->protocol);
    printf("Check sum           : %u\n", ntohs(iph->check));
    printf("Send address        : %s\n", sip);
    printf("Destination address : %s\n", dip);
}