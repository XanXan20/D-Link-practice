#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Recv(int sockfd, void *buf, size_t len, int flags);

void Send(int sockfd, const void *buf, size_t len, int flags);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);