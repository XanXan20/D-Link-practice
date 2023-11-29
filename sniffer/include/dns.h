#ifndef DNS_H
#define DNS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>


#endif
struct dnshdr {
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

void prepare_dns_header(struct dnshdr *dnsh);

void print_dns_header(struct dnshdr *dnsh);

uint8_t print_section_name(unsigned char *payload);

void print_dns_data(const struct dnshdr *dnsh, unsigned char *payload, int payloadlen);
