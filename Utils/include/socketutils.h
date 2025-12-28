#ifndef SOCKETUTIL_SOCKETUTIL_H
#define SOCKETUTIL_SOCKETUTIL_H

#include <netinet/in.h>

int createIPv4TCPSocket(void);
struct sockaddr_in *createIPv4TCPAddress(const char *ip, int port);

#endif

