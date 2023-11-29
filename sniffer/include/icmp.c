#include "icmp.h"

void print_icmphdr(const struct icmphdr *icmph) {
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