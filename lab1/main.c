#include <stdio.h>
#include <stdlib.h>
#include "ipdata.h"
#include "ipfunc.h"

ipdata input(){
    ipdata data;
    
    puts("Введите IP-адрес в формате: 192.168.62.72/28");
    scanf("%d.%d.%d.%d/%d", data.ip, data.ip+1, data.ip+2, data.ip+3, &(data.mask));

    return data;
}

int main(){
    ip_data_init();
    ipdata input_data = input();
    
    check_for_address(input_data);
    check_for_private(input_data);
    check_for_broadcast(input_data);
    
    return 0;
}