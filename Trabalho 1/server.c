/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define N_CLIENTS 5

typedef struct bag_client {
    int* sockfd_clients;
    int sockfd_client;
} BagClient;

void error(char*);

int app_socket();

void app_bind(int, int);

int app_accept(int);

void thread_client(BagClient*);

int main(int argc, char *argv[])
{
    pthread_t thread_clients[N_CLIENTS];
    int sockfd, sockfd_clients[N_CLIENTS];
    BagClient bag[N_CLIENTS];

    for (int i = 0; i < N_CLIENTS; ++i)
    {
        sockfd_clients[i] = -1;
    }

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = app_socket();

    app_bind(sockfd, atoi(argv[1]));

    for (int i = 0; i < N_CLIENTS; i++) {
        int newsockfd = app_accept(sockfd);

        sockfd_clients[i] = newsockfd;

        bag[i].sockfd_clients = sockfd_clients;
        bag[i].sockfd_client = sockfd_clients[i];

        pthread_create(&thread_clients[i], NULL, (void*)thread_client, &bag[i]);
    }

    for (int i = 0; i < N_CLIENTS; ++i)
    {
        pthread_join(thread_clients[i], NULL);
    }
    
    return 0; 
}

void thread_client(BagClient* bag) {
    int n_read;
    char buffer[256];

    for (;;) {
        bzero(buffer, 256);

        printf("Recebendo do cliente: %d\n", bag->sockfd_client);

        n_read = read(bag->sockfd_client, buffer, 255);

        printf("Mensagem recebida: %s\n", buffer);

        if (n_read < 0) {
            error("ERROR reading from socket");
        }

        for (int i = 0; i < N_CLIENTS; i++)
        {
            if (bag->sockfd_clients[i] != -1 && bag->sockfd_clients[i] != bag->sockfd_client) {
                if (write(bag->sockfd_clients[i], buffer, strlen(buffer)) < 0) {
                    printf("ERROR writing to socket");
                }
            }
        }
    }

    printf("Here is the message: %s\n", buffer);
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int app_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    return sockfd;
}

void app_bind(int sockfd, int portno) {
    struct sockaddr_in serv_addr;

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
}

int app_accept(int sockfd) {
    int clilen;
    int newsockfd;
    struct sockaddr_in cli_addr;

    listen(sockfd,5);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (unsigned int *) &clilen);
    
    if (newsockfd < 0) {
        error("ERROR on accept");
    }

    return newsockfd;
}