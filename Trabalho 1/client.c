#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define h_addr h_addr_list[0]


void error(char *);

int app_socket();

void app_connect(int, int, struct hostent *);

void thread_write(int*);

void thread_read(int*);

int main(int argc, char *argv[]) {
    int sockfd;
    struct hostent *server;
    pthread_t th_read, th_write;


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

    pthread_create(&th_write, NULL, (void*)thread_write, &sockfd);
    pthread_create(&th_read, NULL, (void*)thread_read, &sockfd);

    pthread_join(th_write, NULL);
    pthread_join(th_read, NULL);
    
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

void thread_write(int* sockfd) {
    char buffer[256];
    int n;

    for (;;) {
        printf("Please enter the message: ");

        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        printf("Enviando: %s...\n", buffer);
        
        n = write(*sockfd, buffer, strlen(buffer));

        if (n < 0) {
            error("ERROR writing to socket");
        }
    }
}

void thread_read(int* sockfd) {
    char buffer[256];
    int n;

    for (;;) {
        bzero(buffer, 256);
        n = read(*sockfd, buffer, 255);

        if (n < 0) {
            error("ERROR reading from socket");
        }

        printf("%s\n", buffer);
    }
}