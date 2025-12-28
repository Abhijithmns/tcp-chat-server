#include "socketutils.h"

#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>


void startReceiverThread(int socketFD);
void *receiveMessages(void* arg);
void sendMessagesFromStdin(int socketFD);


//Reads user input and sends messages to server 
void sendMessagesFromStdin(int socketFD) {
    char *name = NULL;
    size_t nameSize = 0;

    printf("Enter your name: ");
    ssize_t nameLen = getline(&name, &nameSize, stdin);
    if (nameLen > 0)
        name[nameLen - 1] = '\0';

    char *line = NULL;
    size_t lineSize = 0;
    char buffer[1024];

    printf("Type messages (type 'exit' to quit)\n");

    while (true) {
        ssize_t len = getline(&line, &lineSize, stdin);
        if (len <= 0)
            continue;

        line[len - 1] = '\0';

        if (strcmp(line, "exit") == 0)
            break;

        snprintf(buffer, sizeof(buffer), "%s: %s\n", name, line);
        send(socketFD, buffer, strlen(buffer), 0);
    }

    free(name);
    free(line);
}

// Starts a background thread to receive messages 
void startReceiverThread(int socketFD) {
    pthread_t tid;
    pthread_create(&tid, NULL,receiveMessages,(void*)(intptr_t)socketFD);
    pthread_detach(tid);
}

// Receives messages from server 
void *receiveMessages(void* arg) { // the pthread_create function should have: void *(*start_routine)(void *)

    int socketFD = (int)(intptr_t)arg; //intptr_t:pointer-sized integer (large enough to hold a ptr)
    char buffer[1024];

    while (true) {
        ssize_t bytes = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0)
            break;

        buffer[bytes] = '\0';
        printf("%s\n", buffer);
    }

    return NULL;
}


int main(void) {

    int socketFD = createIPv4TCPSocket();
    struct sockaddr_in *address = createIPv4TCPAddress("127.0.0.1", 2000);

    if (connect(socketFD, (struct sockaddr*)address, sizeof(*address)) == 0)
        printf("Connection successful\n");

    startReceiverThread(socketFD);
    sendMessagesFromStdin(socketFD);

    close(socketFD);
    return 0;
}

