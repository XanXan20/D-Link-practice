#include "erproc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //для поддержки констант
#include <sys/socket.h> // для поддержки сокетов
#include <unistd.h> // для поддержки функции чтения 
#include <arpa/inet.h>

int Socket(int domain, int type, int protocol){
    int res = socket(domain, type, protocol);
    if(res == -1){
        perror("socked failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    int res = bind(sockfd, addr, addrlen);
    if (res == -1){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog){
    int res = listen(sockfd, backlog);
    if (res == -1){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
    int res = accept(sockfd, addr, addrlen);
    if(res == -1){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    int res = connect(sockfd, addr, addrlen);
    if(res == -1){
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}

void Inet_pton(int af, const char *src, void *dst){
    int res = inet_pton(af, src, dst);
    if(res == 0){
        printf("inet_pton failed: src does not contain a character"
        " string representing a valid network address");
    }
    if(res == -1){
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
}

void Recv(int sockfd, void *buf, size_t len, int flags){
    int res = recv(sockfd, buf, len, flags);
    if(res == -1){
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
}

int Recvfrom(int sockfd, void * buf, size_t len, int flags, struct sockaddr * src_addr, socklen_t * addrlen){
    int res = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    if(res == -1){
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Send(int sockfd, const void *buf, size_t len, int flags){
    int res = send(sockfd, buf, len, flags);
    if(res == -1){
        perror("send failed");
        exit(EXIT_FAILURE);
    }
}
