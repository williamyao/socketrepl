#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int server_fd;

void* receive_incoming(void* arg);
void print_usage(char* program_name);

int main(int argc, char* argv[]) {
    char* buf = NULL;
    struct sockaddr_in server_addr;
    struct hostent* host;
    char* hostname;
    char* end;
    pthread_t ret;
    int port;
    ssize_t bytes;
    size_t cap;

    if(argc != 3) {
        print_usage(argv[0]);
        exit(1);
    }

    hostname = argv[1];

    /* Convert port string to number, and check for errors. */
    port = (int) strtol(argv[2], &end, 10);

    if(end == argv[2] || *end != '\0') {
        fprintf(stderr,
                "Not a number: %s\n",
                argv[2]);
        exit(1);
    }

    /* Create a socket to connect to the server, and check for errors. */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0) {
        fprintf(stderr, "Failed to create socket.\n");
        exit(1);
    }

    /* Try to look up the host, and check for errors. */
    host = gethostbyname(hostname);

    if(host == NULL) {
        fprintf(stderr,
                "Failed to resolve host %s.\n",
                hostname);
        exit(1);
    }

    /* Attempt to connect the server, and check for errors. */
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    bcopy(host->h_addr,
          &(server_addr.sin_addr.s_addr),
          host->h_length);

    if(connect(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr,
                "Failed to connect to the server %s:%s.\n",
                hostname,
                argv[2]);
        exit(1);
    }

    pthread_create(&ret, NULL, receive_incoming, NULL);

    while(1) {
        buf = NULL;
        cap = 0;
        bytes = getline(&buf, &cap, stdin);
        write(server_fd, buf, bytes - 1);
        write(server_fd, "\r\n", 2);
        free(buf);
    }

    close(server_fd);

    return 0;
}


void print_usage(char* program_name) {
    fprintf(stderr,
            "usage -- %s <host> <port>\n\
\n\
    Connects to a remote host and treats it like an REPL,\n\
    sending standard input to the server. Any responses from\n\
    the server are printed to standard output, immediately and\n\
    asynchronously.\n",
    		program_name);
} 

void* receive_incoming(void* arg) {
    char buf[512];
    ssize_t bytes;

    while(1) {
        int i;
        
        bytes = read(server_fd, buf, 512);

        for(i = 0; i < bytes; i++) printf("%c", buf[i]);
    }
}
