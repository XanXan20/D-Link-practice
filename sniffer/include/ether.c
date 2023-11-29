#include "ether.h"

void print_ethhdr(const struct ethhdr *ethh) {
    printf("\n----------ETHERNET HEADER----------\n");
    printf("Destination MAC     : %02X%02X:%02X%02X:%02X%02X\n", ethh->h_dest[0], ethh->h_dest[1], ethh->h_dest[2], ethh->h_dest[3], ethh->h_dest[4], ethh->h_dest[5]);
    printf("Source MAC          : %02X%02X:%02X%02X:%02X%02X\n", ethh->h_source[0], ethh->h_source[1], ethh->h_source[2], ethh->h_source[3], ethh->h_source[4], ethh->h_source[5]);
    printf("Ethernet Type       : 0x%04X\n", ntohs(ethh->h_proto));
}