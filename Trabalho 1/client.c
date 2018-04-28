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
    pthread_cancel(th_read);
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

    do {
        printf("Please enter the message: ");

        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        
        n = write(*sockfd, buffer, strlen(buffer));

        if (n < 0) {
            error("ERROR writing to socket");
        }
    } while (not_bye(buffer, strlen(buffer)));
}

int not_bye(char* msg, int len_msg) {

    // Remove o carriage return.
    if (len_msg > 0 && (msg[len_msg - 1] == '\n' || msg[len_msg - 1] == '\r')) {
        msg[len_msg - 1] = '\0';
    }

    if (len_msg > 1 && msg[len_msg - 2] == '\r') {
        msg[len_msg - 2] = '\0';
    }
    // --

    for (int i = 0; i < len_msg; i++)
    {
        msg[i] = tolower(msg[i]);
    }

    return strcmp(msg, "bye");
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

        printf("\n\n%s\n\n", buffer);
    }
}