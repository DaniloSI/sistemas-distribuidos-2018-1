/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>

#define N_CLIENTS 5

typedef struct bag_client {
    int* sockfd_clients;
    int client_id;
    int sockfd_client;
} BagClient;

void error(char*);

int app_socket();

void app_bind(int, int);

int app_accept(int);

void thread_client(BagClient*);

int not_bye(char*, int);

int main(int argc, char *argv[])
{
    int i;
    pthread_t thread_clients[N_CLIENTS];
    int sockfd, sockfd_clients[N_CLIENTS];
    BagClient bag[N_CLIENTS];

    for (i = 0; i < N_CLIENTS; ++i)
    {
        sockfd_clients[i] = -1;
    }

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = app_socket();

    app_bind(sockfd, atoi(argv[1]));

    for (i = 0; i < N_CLIENTS; i++) {
        int newsockfd = app_accept(sockfd);

        sockfd_clients[i] = newsockfd;

        bag[i].sockfd_clients = sockfd_clients;
        bag[i].sockfd_client = sockfd_clients[i];
        bag[i].client_id = i + 1;

        printf("Cliente conectado: %d\n", bag[i].client_id);

        pthread_create(&thread_clients[i], NULL, (void*)thread_client, &bag[i]);
    }

    for (i = 0; i < N_CLIENTS; ++i)
    {
        pthread_join(thread_clients[i], NULL);
    }
    
    return 0; 
}

void thread_client(BagClient* bag) {
    char buffer[256];
    int i;
    char identificador_client[24];

    sprintf(identificador_client, "O usuario [%d] disse: ", bag->client_id);

    do {
        int n_read;

        bzero(buffer, 256);
        n_read = read(bag->sockfd_client, buffer, 255);

        if (n_read < 0) {
            error("ERROR reading from socket");
        }

        for (i = 0; i < N_CLIENTS; i++)
        {
            if (bag->sockfd_clients[i] != -1 && bag->sockfd_clients[i] != bag->sockfd_client) {
                char msg_send[280];

                bzero(msg_send, 280);
                strcpy(msg_send, identificador_client);
                strcat(msg_send, buffer);

                if (write(bag->sockfd_clients[i], msg_send, strlen(msg_send)) < 0) {
                    printf("Falha ao enviar mensagem para o cliente de sockfd: [%d]. Talvez ele acabou de ser desconectado, enquanto era enviado a mensagem.\n", bag->sockfd_clients[i]);
                }
            }
        }
    } while (not_bye(buffer, strlen(buffer)));

    /* Remove o sockfd da lista de clients. */
    bag->sockfd_clients[bag->client_id - 1] = -1;

    printf("Cliente desconectado: %d\n", bag->client_id);
}

int not_bye(char* msg, int len_msg) {
    int i;

    /* Remove o carriage return. */
    if (len_msg > 0 && (msg[len_msg - 1] == '\n' || msg[len_msg - 1] == '\r')) {
        msg[len_msg - 1] = '\0';
    }

    if (len_msg > 1 && msg[len_msg - 2] == '\r') {
        msg[len_msg - 2] = '\0';
    }
    /* -- */

    for (i = 0; i < len_msg; i++)
    {
        msg[i] = tolower(msg[i]);
    }

    return strcmp(msg, "bye");
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