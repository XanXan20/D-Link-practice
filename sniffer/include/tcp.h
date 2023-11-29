#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <linux/tcp.h>
#include <arpa/inet.h>


#endif

void print_tcphdr(const struct tcphdr *tcph);