#include "tcp.h"

void print_tcphdr(const struct tcphdr *tcph) {
    printf("------------TCP HEADER-------------\n");
    printf("Source port         : %u\n", ntohs(tcph->source));
    printf("Detsination port    : %u\n", ntohs(tcph->dest));
    printf("Sequence            : %u\n", ntohs(tcph->seq));
    printf("Acknowledgment      : %u\n", ntohs(tcph->ack_seq));
    printf("Header length       : %u\n", tcph->doff);
    printf("Window size         : %u\n", ntohs(tcph->window));
    printf("Check sum           : %u\n", ntohs(tcph->check));
    printf("-----------------------------------\n");
}