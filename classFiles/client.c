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
#define FIFO 0
#define CONCUR 1

typedef void *(worker_fn)(void *);

// Get host information (used to establishConnection)
struct addrinfo *getHostInfo(char* host, char* port) {
  int r;
  struct addrinfo hints, *getaddrinfo_res;
  // Setup hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((r = getaddrinfo(host, port, &hints, &getaddrinfo_res))) {
    fprintf(stderr, "[getHostInfo:21:getaddrinfo] %s\n", gai_strerror(r));
    return NULL;
  }

  return getaddrinfo_res;
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

static void *client_worker(void * arg){
  //each thread sends a reuest to the server
}

int main(int argc, char **argv) {
  int schedalg;
  int clientfd;
  int port, threads;
  struct sockaddr_in server;//socket information about the server
  struct sockaddr_in client;//socket information about
  int clientLen;
  size_t i, j;//for the loop counter

  char buf[BUF_SIZE];//recieve the data buffer

  if (argc != 4) {
    fprintf(stderr, "USAGE: %s <hostname> <port> <request path>\n", argv[0]);
    return 1;
  }

  /* The program ./client is run w/the following args
  argv[1] ==> [host] 
  argv[2] ==> [portnum] 
  argv[3] ==> [threads]
  argv[4] ==> [schedalg]
  argv[5] ==> [filename1]
  argv[6] ==> [filename2]
  */

port = atoi(argv[2]);
threads = atoi(argv[3]);
if (strcmp(argv[4], "CONCUR") == 0)
  schedalg = CONCUR;
else if (strcmp(argv[4], "FIFO") == 0)
  schedalg = FIFO;

switch(schedalg){
  case CONCUR:
    do_concurrent_groups(**argv, client_worker);
    break;
  case FIFO:
    do_fifo();
    break;
  default:
    exit(0);
}

// Establish connection with <hostname>:<port>
//establishConnection opens
  void do_concurrent_groups(char ** argv, worker_fn worker){
    for(i = 0; i < threads; i++){
      j = i+1;
      pthread_create(&thread, NULL, worker, (void *)j);
    }
    clientfd = establishConnection(getHostInfo(argv[1], argv[2]));
    if (clientfd == -1){
      fprintf(stderr,
            "[main:73] Failed to connect to: %s:%s%s \n",
            argv[1], argv[2], argv[3]);
      return 3;
    }

    // Send GET request > stdout
    GET(clientfd, argv[5]);
    while (recv(clientfd, buf, BUF_SIZE, 0) > 0) {
      fputs(buf, stdout);
      memset(buf, 0, BUF_SIZE);
    }

    close(clientfd);
    return 0;
  }//end of concurrent_groups

}//end of main
