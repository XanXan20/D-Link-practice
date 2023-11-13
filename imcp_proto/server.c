#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>

void print_data(unsigned char *data, int len){
    for(int i = 0; i < len; i++){
        
        /*
        *   если это не первый проход цикла и счетчик кратен 16, 
        *   то это конец октета и мы выводим символьные значения байтов, если они находятся в алфавитном диапазоне,
        *   а если нет, то вместо этого лупасим точку
        */
        if(i!=0 && i%16==0) 
        {
			printf("         ");
			for(int j = i-16 ; j<i ; j++)
			{
				if(data[j]>=32 && data[j]<=128)
					printf("%c",(unsigned char)data[j]); 
				
				else printf(".");
			}
			printf("\n");
		} 
		// разделяем шестнадцатиричное представление числа от символьного
		if(i%16 == 0){
            printf("   ");
        }
		
        printf(" %02X",(unsigned int)data[i]); // собственно выводим само шестнадцатиричное представления числа из буффера

		/*
        *   последний октет не всегда содержит 16 байт, поэтому здесь делается отступ нужного размера
        *   и выводится остаток октета сиволами (второй цикл условия аналогичен первому вложенному циклу функции)
        */
        if(i == len-1)  
		{
			for(int j = 0; j < 15-i % 16; j++) {
                printf("   "); 
            }
			printf("         ");

			for(int j = i-i % 16; j <= i; j++)
			{
				if(data[j]>=32 && data[j]<=128){
                    printf("%c",(unsigned char)data[j]);
                }
				else printf(".");
			}
			printf("\n");
		}
    }
}

int print_iphdr(char *buf){
    unsigned char *sip = (unsigned char *)malloc(100);
    unsigned char *dip = (unsigned char *)malloc(100);
    
    struct iphdr *iph = (struct iphdr *)(buf + sizeof(struct ethhdr));
    inet_ntop(AF_INET, &iph->saddr, (char *)sip, 100);
    inet_ntop(AF_INET, &iph->daddr, (char *)dip, 100);

    printf("-------------IP HEADER-------------\n");
    printf("IP header length    : %u\n", iph->ihl);
    printf("IP version          : %u\n", iph->version);
    printf("Type of service     : %u\n", iph->tos);
    printf("Total length        : %u\n", iph->tot_len);
    printf("Package ID          : %u\n", iph->id);
    printf("Time to life        : %u\n", iph->ttl);
    printf("Protocol            : %u\n", iph->protocol);
    printf("Check sum           : %u\n", iph->check);
    printf("Send address        : %s\n", sip);
    printf("Destination address : %s\n", dip);

    return iph->protocol;
}

void print_tcphdr(unsigned char *buf){
    //struct iphdr *iph = (struct iphdr *)buf;
    // short iphlen = iph->ihl*4;

    struct tcphdr *tcph = (struct tcphdr*)(buf + sizeof(struct iphdr)); 

    printf("------------TCP HEADER-------------\n");
    printf("Source port         :%u\n", tcph->source);
    printf("Detsination port    :%u\n", tcph->dest);
    printf("Sequence            :%u\n", tcph->seq);
    printf("Acknowledgment      :%u\n", tcph->ack_seq);
    printf("Header length       :%u\n", tcph->doff);
    printf("Window size         :%u\n", tcph->window);
    printf("Check sum           :%u\n", tcph->check);
    printf("Data: \n");
    //print_data(buf, (sizeof(struct ethhdr)+sizeof(struct iphdr)+sizeof(struct tcphdr)));
    //printf("\n");
    printf("-----------------------------------\n");
    printf("\n");
}

void print_udphdr(unsigned char *buf){
    struct udphdr *udph = (struct udphdr*)(buf + sizeof(struct iphdr)); 
    
    printf("------------UDP HEADER-------------\n");
    printf("Source port         :%u\n", udph->source);
    printf("Detsination port    :%u\n", udph->dest);
    printf("Length              :%u\n", udph->len);
    printf("Check sum           :%u\n", udph->check);
    //print_data(buf, (sizeof(struct ethhdr)+sizeof(struct iphdr)+sizeof(struct udphdr)));
    //printf("\n");
    printf("-----------------------------------\n");
    printf("\n");
}

void print_icmphdr(unsigned char *buf){
    struct icmphdr *icmph = (struct icmphdr*)(buf + sizeof(struct iphdr)); 

    printf("------------ICMP HEADER------------\n");
    printf("Type                : %u\n", icmph->type);
    printf("Code                : %u\n", icmph->code);
    printf("Checksum            : %u\n", icmph->checksum);
    printf("Un id echo id       : %u\n", icmph->un.echo.id);
    printf("Un id echo sequence : %u\n", icmph->un.echo.sequence);
    printf("Un gateway          : %u\n", icmph->un.gateway);
    printf("Un frag _unused     : %u\n", icmph->un.frag.__unused);
    printf("Un frag mtu         : %u\n", icmph->un.frag.mtu);
    printf("Un reserved         : %hhn\n", icmph->un.reserved);
    printf("-----------------------------------\n");
    printf("\n");
}

int main(){

    int sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_ICMP);
    if(sockfd == -1){
        perror("socket failed");
        return 1;
    }

    unsigned char *buf = (unsigned char *)malloc(65536);
    struct sockaddr_in src_addr = {0};
    socklen_t addr_len = sizeof(src_addr);

    while(1){
        int rcvlen = recvfrom(sockfd, buf, 65536, 0, (struct sockaddr *)&src_addr, &addr_len);
        if(rcvlen == -1){
            perror("recv failed");
            return 1;
        }    
        
        
        int protocol = print_iphdr((char *)buf);
        switch(protocol){
            case IPPROTO_TCP:
                print_tcphdr(buf);
                break;
            case IPPROTO_UDP:
                print_udphdr(buf);
                break;
            case IPPROTO_ICMP:
                print_icmphdr(buf);
                return 1;
                break;
        }
    }

    return 0;
}