/* Generic */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Network */
#include <netdb.h>
#include <sys/socket.h>

#define BUF_SIZE 250

// Get host information (used to establishConnection)
struct addrinfo *getHostInfo(char* host, char* port) {
  int r;
  struct addrinfo hints, *getaddrinfo_res;
  // Setup hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM; //Provides sequenced, reliable, two-way, connection-based byte streams.An out - of - band data transmission mechanism may be supported.
  if ((r = getaddrinfo(host, port, &hints, &getaddrinfo_res)))
  {//returns 0 of succeeds so skips this if statement
    fprintf(stderr, "[getHostInfo:21:getaddrinfo] %s\n", gai_strerror(r));
    return NULL;
  }

  return getaddrinfo_res; //we have selected the socket address structures returned in a list pointed to by getaddrinfo_res
}

// Establish connection with host
int establishConnection(struct addrinfo *info) {
  if (info == NULL) return -1;

  int clientfd;
  for (;info != NULL; info = info->ai_next) {
    if ((clientfd = socket(info->ai_family,
                           info->ai_socktype,
                           info->ai_protocol)) < 0) {
      perror("[establishConnection:35:socket]");
      continue;
    }

    //clientfd is file descriptor that refers to the endpoint created by the socket method
    if (connect(clientfd, info->ai_addr, info->ai_addrlen) < 0) {
      close(clientfd);
      perror("[establishConnection:42:connect]");
      continue;
    }

    freeaddrinfo(info);
    return clientfd;
  }

  freeaddrinfo(info);
  return -1;
}

// Send GET request
void GET(int clientfd, char *path) {
  char req[1000] = {0};
  sprintf(req, "GET %s HTTP/1.0\r\n\r\n", path);
  send(clientfd, req, strlen(req), 0);
}

int main(int argc, char **argv) {
  int clientfd;
  char buf[BUF_SIZE];

  if (argc != 4) {
    fprintf(stderr, "USAGE: %s <hostname> <port> <request path>\n", argv[0]);
    return 1;
  }

  // Establish connection with <hostname>:<port>
  clientfd = establishConnection(getHostInfo(argv[1], argv[2]));
  if (clientfd == -1) {
    fprintf(stderr,
            "[main:73] Failed to connect to: %s:%s%s \n",
            argv[1], argv[2], argv[3]);
    return 3;
  }

  // Send GET request > stdout
  GET(clientfd, argv[3]);
  //while we can read what the server is sending us, an image or html file, put that in buf than enter the loop
  while (recv(clientfd, buf, BUF_SIZE, 0) > 0) {
    fputs(buf, stdout);//write what is in buf to the stdout stream so it displays
    memset(buf, 0, BUF_SIZE);//clear the buffer so other get requests can be processed on the same socket
  }

  close(clientfd);
  return 0;
}
