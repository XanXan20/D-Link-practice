#include "erproc.h"

int Socket(int domain, int type, int protocol){
    int res = socket(domain, type, protocol);
    if(res == -1){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    int res = bind(sockfd, addr, addrlen);
    if(res == -1){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog){
    int res = listen(sockfd, backlog);
    if(res == -1){
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

void Recv(int sockfd, void *buf, size_t len, int flags){
    int res = recv(sockfd, buf, len, flags);
    if(res == -1){
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
}

void Send(int sockfd, const void *buf, size_t len, int flags){
    int res = send(sockfd, buf, len, flags);
    if(res == -1){
        perror("send failed");
        exit(EXIT_FAILURE);
    }
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    int res = connect(sockfd, addr, addrlen);
    if(res == -1){
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}