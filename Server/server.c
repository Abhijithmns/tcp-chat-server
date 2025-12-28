#include "socketutils.h"

#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


struct AcceptedSocket {
    int fd;
    struct sockaddr_in address;
    int error;
    bool accepted;
};

struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount = 0;


struct AcceptedSocket* acceptClient(int serverSocketFD);
void acceptClientsLoop(int serverSocketFD);
void *handleClientMessages(void* arg);
void startClientThread(struct AcceptedSocket* client);
void broadcastMessage(char* buffer, int senderFD);

void acceptClientsLoop(int serverSocketFD) {
    while (true) {
        struct AcceptedSocket *client = acceptClient(serverSocketFD);
        if (!client->accepted) {
            free(client);
            continue;
        }

        acceptedSockets[acceptedSocketsCount++] = *client;
        startClientThread(client);
    }
}

void startClientThread(struct AcceptedSocket* client) {
    pthread_t id;
    pthread_create(&id, NULL, handleClientMessages, client);
    pthread_detach(id);
}

void* handleClientMessages(void* arg) {
    struct AcceptedSocket* client = arg;
    char buffer[1024];

    while (true) {
        ssize_t bytes = recv(client->fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        printf("%s\n", buffer);
        broadcastMessage(buffer, client->fd);
    }

    close(client->fd);
    free(client);
    return NULL;
}

void broadcastMessage(char* buffer, int senderFD) {
    for (int i = 0; i < acceptedSocketsCount; i++) {
        if (acceptedSockets[i].fd != senderFD) {
            send(acceptedSockets[i].fd,buffer,strlen(buffer),0);
        }
    }
}

struct AcceptedSocket* acceptClient(int serverSocketFD) {
    struct sockaddr_in clientAddress;
    socklen_t size = sizeof(clientAddress);

    int clientFD = accept(serverSocketFD,(struct sockaddr*)&clientAddress,&size);

    struct AcceptedSocket* client = malloc(sizeof(*client));
    client->fd = clientFD;
    client->address = clientAddress;
    client->accepted = clientFD >= 0;

    if (!client->accepted)
        client->error = clientFD;

    return client;
}

int main(void) {
    int serverSocketFD = createIPv4TCPSocket();
    struct sockaddr_in* serverAddress =
        createIPv4TCPAddress("127.0.0.1", 2000);

    int res = bind(serverSocketFD, (struct sockaddr*)serverAddress, sizeof(*serverAddress));
    if(res == 0) 
        printf("Socket Bound Successfully!\n");
    else
     printf("There is some issue idk what");
    listen(serverSocketFD, 10);

    acceptClientsLoop(serverSocketFD); //start accepting

    return 0;
}

