#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <unistd.h>


void error(char *);

int app_socket();

void app_connect(int, int, struct hostent *);

char app_read_order(int);

void app_write(int);

void app_read(int);

int main(int argc, char *argv[]) {
    int sockfd;
    char order;
    struct hostent *server;


    if (argc < 3) {
       fprintf(stderr, "usage %s hostname port\n", argv[0]);
       exit(0);
    }

    sockfd = app_socket();
    server = gethostbyname(argv[1]);
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    app_connect(sockfd, atoi(argv[2]), server);

    order = app_read_order(sockfd);

    while (1) {
        if (order == '1') {
            app_write(sockfd);
            app_read(sockfd);
        } else {
            app_read(sockfd);
            app_write(sockfd);
        }
    }
    
    return 0;
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int app_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    return sockfd;
}

void app_connect(int sockfd, int portno, struct hostent *server) {
    struct sockaddr_in serv_addr;
    
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
}

char app_read_order(int sockfd) {
    char buffer[1];
    int n;

    printf("Entrou no app_read_order");

    bzero(buffer, 1);
    n = read(sockfd, buffer, 1);

    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("Order: %s\n", buffer);

    return buffer[0];
}

void app_write(int sockfd) {
    printf("Please enter the message: ");
    char buffer[256];
    int n;

    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        error("ERROR writing to socket");
    }
}

void app_read(int sockfd) {
    char buffer[256];
    int n;

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);

    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("%s\n", buffer);
}