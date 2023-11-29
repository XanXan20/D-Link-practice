#include "udp.h"

void print_udphdr(const struct udphdr *udph) {
    printf("------------UDP HEADER-------------\n");
    printf("Source port         : %u\n", ntohs(udph->source));
    printf("Detsination port    : %u\n", ntohs(udph->dest));
    printf("Length              : %u\n", ntohs(udph->len));
    printf("Check sum           : %u\n", ntohs(udph->check));
    printf("-----------------------------------\n");
}