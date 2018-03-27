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

int app_accept(int);

void app_read(int);

void app_write(int);

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = app_socket();

    app_bind(sockfd, atoi(argv[1]));

    newsockfd = app_accept(sockfd);

    app_read(newsockfd);

    app_write(newsockfd);
    
    return 0; 
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

void app_read(int sockfd) {
    int n;
    char buffer[256];

    n = read(sockfd, buffer, 255);

    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("Here is the message: %s\n", buffer);
}

void app_write(int sockfd) {
    int n;

    n = write(sockfd, "I got your message", 18);
    
    if (n < 0) {
        error("ERROR writing to socket");
    }
}