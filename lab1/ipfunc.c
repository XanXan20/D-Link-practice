#include "ipfunc.h"

void apply_mask(ipdata data, int * net_addr){
    
    char bin_mask[32] = {"00000000000000000000000000000000"};
    for(int i = 0; i < data.mask; i++){
        bin_mask[i] = '1';
    }

    int dec_mask[4];
    
    for(int i = 0, counter = 0; i < 32; i += 8, counter++){
        int num = 0;
        for(int j = 0; j < 8; j++){
            if(bin_mask[i+j] == '1'){
                num += pow(2, 7-j);
            }
        }  
        dec_mask[counter] = num;
    }

    for(int i = 0; i < 4; i++){
        net_addr[i] = data.ip[i] & dec_mask[i];
    }   
}

void check_for_address(ipdata data){

    int net_addr[4];
    apply_mask(data, net_addr);


    if(net_addr[0] == Network_1.ip[0] && net_addr[1] == Network_1.ip[1] && net_addr[2] == Network_1.ip[2] && net_addr[3] == Network_1.ip[3])
        puts("Адрес принадлежит сети Network_1");
    else if(net_addr[0] == Network_2.ip[0] && net_addr[1] == Network_2.ip[1] && net_addr[2] == Network_2.ip[2] && net_addr[3] == Network_2.ip[3])
        puts("Адрес принадлежит сети Network_2");
    else if(net_addr[0] == Network_3.ip[0] && net_addr[1] == Network_3.ip[1] && net_addr[2] == Network_3.ip[2] && net_addr[3] == Network_3.ip[3])
        puts("Адрес принадлежит сети Network_3");
    else if(net_addr[0] == Network_4.ip[0] && net_addr[1] == Network_4.ip[1] && net_addr[2] == Network_4.ip[2] && net_addr[3] == Network_4.ip[3])
        puts("Адрес принадлежит сети Network_4");
    else
        puts("Адрес не принадлежит ни одной из известных сетей");
}

void check_for_private(ipdata data){
    if(data.ip[0] == 10 || 
    (data.ip[0] == 172 && (data.ip[1] > 15 && data.ip[1] < 32)) || 
    (data.ip[0] == 192 && data.ip[1] == 168))
        puts("Адрес является частным");
    else
        puts("Адрес является публичным");
}

void check_for_broadcast(ipdata data){
    char reverse_bin_mask[32] = {"11111111111111111111111111111111"};
    
    for(int i = 0; i < data.mask; i++){
        reverse_bin_mask[i] = '0';
    }
    
    int net_adr[4];
    int reverse_mask[4];
    
    apply_mask(data, net_adr);

    for(int i = 0, counter = 0; i < 32; i += 8, counter++){
        int num = 0;
        for(int j = 0; j < 8; j++){
            if(reverse_bin_mask[i+j] == '1'){
                num += pow(2, 7-j);
            }
        }  
        reverse_mask[counter] = num;
    }

    int broadcast_addr[4] = {net_adr[0] + reverse_mask[0], net_adr[1] + reverse_mask[1], net_adr[2] + reverse_mask[2], net_adr[3] + reverse_mask[3]};
    if (data.ip[0] == broadcast_addr[0] && data.ip[1] == broadcast_addr[1] && data.ip[2] == broadcast_addr[2] && data.ip[3] == broadcast_addr[3])
        puts("Адрес широковещательный");
    else
        puts("Адрес не широковещательный");
}