#ifndef IPDATA_H
#define IPDATA_H

typedef struct 
{
    int ip[4];
    int mask;
} ipdata;

extern ipdata Network_1;
extern ipdata Network_2;
extern ipdata Network_3;
extern ipdata Network_4;

void ip_data_init();

#endif

