/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void error(char*);

int app_socket();

void app_bind(int, int);

int app_accept(int, char);

void app_read(int, char*);

void app_write(int, char*);

int app_finish(char*);

int main(int argc, char *argv[]) {
    int sockfd_server, sockfd_client_x, sockfd_client_y;
    char buffer[256];

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd_server = app_socket();

    app_bind(sockfd_server, atoi(argv[1]));

    sockfd_client_x = app_accept(sockfd_server, '1');
    sockfd_client_y = app_accept(sockfd_server, '2');

    do {
        app_read(sockfd_client_x, buffer);
        app_write(sockfd_client_y, buffer);

        app_read(sockfd_client_y, buffer);
        app_write(sockfd_client_x, buffer);
    } while (!app_finish(buffer));
    
    return 0; 
}

void error(char *msg) {
    perror(msg);
    exit(1);
}

int app_finish(char *buffer) {
    if (strlen(buffer)) {
        char *token;

        token = strtok(buffer, "\n\r");

        return !strcmp(token, "bye");
    }

    return 0;
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

int app_accept(int sockfd, char order) {
    int clilen;
    int newsockfd;
    struct sockaddr_in cli_addr;
    char buffer[1] = { order };
    int n;

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (unsigned int *) &clilen);
    
    if (newsockfd < 0) {
        error("ERROR on accept");
    }

    n = write(newsockfd, buffer, 1);
    
    if (n < 0) {
        error("ERROR writing to socket");
    }

    return newsockfd;
}

void app_read(int sockfd, char* buffer) {
    int n;

    bzero(buffer, 256);

    n = read(sockfd, buffer, 255);

    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("Here is the message: %s\n", buffer);
}

void app_write(int sockfd, char* buffer) {
    int n;

    n = write(sockfd, buffer, strlen(buffer));
    
    if (n < 0) {
        error("ERROR writing to socket");
    }
}