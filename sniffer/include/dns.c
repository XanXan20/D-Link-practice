#include "dns.h"

void prepare_dns_header(struct dnshdr *dnsh) {
    dnsh->id = ntohs(dnsh->id);
    dnsh->qdcount = ntohs(dnsh->qdcount);
    dnsh->ancount = ntohs(dnsh->ancount);
    dnsh->nscount = ntohs(dnsh->nscount);
    dnsh->arcount = ntohs(dnsh->arcount);
}

void print_dns_header(struct dnshdr *dnsh) {

    prepare_dns_header(dnsh);

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

uint8_t print_section_name(unsigned char *payload) {
    uint8_t first_byte = *payload;

    uint8_t label = (first_byte & 0xC0) >> 6;
    if(label == 3) {
        printf("archived label\n");
    }
    uint8_t len = first_byte & 0x3F;
    uint32_t offset = 1;
    uint8_t *name = payload + offset;

    printf("Data                : ");
    for(int j = 0; j <= len; j++) {
        printf("%c", (char)name[j]);
    }

    printf("\n");
    return offset + 1;
}

void print_dns_data(const struct dnshdr *dnsh, unsigned char *payload, int payloadlen) {
    uint32_t offset = 0;
    if(dnsh->qdcount > 0) {
        printf("------------QUESTION SECTION-------\n");
        for(int i = 0; i < dnsh->qdcount; i++) {
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
    if(dnsh->ancount > 0) {
        printf("------------ANSWER SECTION---------\n");
        for(int i = 0; i < dnsh->ancount; i++) {
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
    if(dnsh->nscount > 0) {
        printf("----------AUTHORITY SECTION--------\n");
        for(int i = 0; i < dnsh->nscount; i++) {
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
    if(dnsh->arcount > 0) {
        printf("----------ADDITIONAL SECTION-------\n");
        for(int i = 0; i < dnsh->arcount; i++) {
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