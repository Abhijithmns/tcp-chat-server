#include "socketutils.h"
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

int createIPv4TCPSocket(void) {
    return socket(AF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in *createIPv4TCPAddress(const char *ip, int port) {
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));

    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if(strlen(ip) == 0) 
        address->sin_addr.s_addr = INADDR_ANY; //listen for any incoming address
    else 
        inet_pton(AF_INET, ip, &address->sin_addr);

    return address;
}

