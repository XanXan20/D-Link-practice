#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>

struct ethh{
    uint8_t dmac[6];
    uint8_t smac[6];
    
    uint16_t ethtype;
};

struct iph{

};

void print_ethh(const struct ethh *eth_h){
    printf("----ETHERNET HEADER----------\n");
    printf("Dest MAC: %x%x:%x%x:%x%x\n", eth_h->dmac[0], eth_h->dmac[1], eth_h->dmac[2], eth_h->dmac[3], eth_h->dmac[4], eth_h->dmac[5]);
    printf("Source MAC: %x%x:%x%x:%x%x\n", eth_h->smac[0], eth_h->smac[1], eth_h->smac[2], eth_h->smac[3], eth_h->smac[4], eth_h->smac[5]);
    printf("Eth Type: 0x%04X\n", ntohs(eth_h->ethtype));
    printf("-----------------------------\n");
}

void print_iph(const struct iph *ip_h){

}

// ETHERNER /\ IP /\ TCP /\ HTTP 
int main(){

    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sockfd == -1){
        perror("socket failed");
        return 1;
    }

    unsigned char *buf = (unsigned char *)malloc(65536);
    //memset(buf, 0, 65535);

    while(1){
        int rcvlen = recv(sockfd, buf, 65536, 0);
        if(rcvlen == -1){
            perror("recv failed");
            return 1;
        }

        struct ethh *eth_h = (struct ethh*)(buf);
        
        print_ethh(eth_h);

        switch(ntohs(eth_h->ethtype)){
            case 0x0800:
                struct iph *ip_h = (struct iph*)(buf + sizeof(struct ethh));
                print_iph(ip_h);
                break;
            default:
                printf("Uncknown network protocol\n");

        } //проверка на вышестоящий протокол (в данном случае только на IP)

        return 1;
    }
    
    return 0;
}
/*

    НА ЧЕМ ОСТАНОВИЛСЯ:::
    НАЧАЛ ПИСАТЬ СТРУКТУРУ IP ЗАГОЛОВКА И ЧИТАТЬ ОТТУДА БАЙТЫ


*/