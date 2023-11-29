#ifndef IP_H
#define IP_H

#include <stdio.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#endif

void print_iphdr(const struct iphdr *iph);