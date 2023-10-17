#include "ipdata.h"

ipdata Network_1;
ipdata Network_2;
ipdata Network_3;
ipdata Network_4;

void ip_data_init(){
    Network_1.ip[0] = 192;
    Network_1.ip[1] = 168;
    Network_1.ip[2] = 10;
    Network_1.ip[3] = 0;
    Network_1.mask = 24;

    Network_2.ip[0] = 192;
    Network_2.ip[1] = 168;
    Network_2.ip[2] = 160;
    Network_2.ip[3] = 0;
    Network_2.mask = 19;

    Network_3.ip[0] = 141;
    Network_3.ip[1] = 127;
    Network_3.ip[2] = 0;
    Network_3.ip[3] = 0;
    Network_3.mask = 16;

    Network_4.ip[0] = 195;
    Network_4.ip[1] = 0;
    Network_4.ip[2] = 172;
    Network_4.ip[3] = 0;
    Network_4.mask = 22;
}