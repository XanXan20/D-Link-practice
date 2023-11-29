#include "http.h"

void print_http(unsigned char *payload, int payloadlen) {
    printf("------------HTTP HEADER------------\n");
    print_payload(payload, payloadlen);
    printf("-----------------------------------\n\n");
}

void print_payload(unsigned char *payload, int payloadlen) {
    for (int i = 0; i < payloadlen; i++) {
        printf("%c", payload[i]);
    }
    printf("\n");
}
