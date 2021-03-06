/* Generic */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Network */
#include <netdb.h>
#include <sys/socket.h>

/*Treads */
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 250
#define FIFO 0
#define CONCUR 1

typedef struct
{
  size_t thread_num;
  pthread_mutex_t work_mutex;
  pthread_cond_t c_cond; // P/C condition variables
  pthread_cond_t p_cond;

  char* host;
  char* port;
  char* filename;

  int clientfd;
} tpool_t; //KRAZY FOR KELLY

typedef void *(worker_fn)(void *);
//static int threads;
int i;
static pthread_barrier_t barrier;

/*function header*/
static tpool_t *client_pool;
void tpool_init_client(tpool_t *tm, size_t num_threads, worker_fn worker, char* host, char* port, char* filename);
struct addrinfo *getHostInfo(char *host, char *port);
int establishConnection(struct addrinfo *info);
void GET(int clientfd, char *path);
static void *client_worker_fifo(void *arg);
static void *client_worker_concur(void *arg);

void tpool_init_client(tpool_t *tm, size_t num_threads, worker_fn worker, char* host, char* port, char* filename)
{
    pthread_t thread;
  size_t i, j;

  //initialize barrier here
  //barrier = malloc(sizeof(pthread_barrier_t));
  pthread_barrier_init(&barrier, NULL, num_threads);

  //tm->work_mutex = (*pthread_mutex_t)malloc(sizeof(pthread_mutex_t);
  pthread_mutex_init(&(tm->work_mutex), NULL);//create a mutex
  pthread_cond_init(&(tm->p_cond), NULL);
  pthread_cond_init(&(tm->c_cond), NULL);

  tm->thread_num = num_threads;
  tm->host = host;
  tm->port = port;
  tm->filename = filename;
  tm->clientfd = establishConnection(getHostInfo(tm->host, tm->port));

  for (i = 0; i < num_threads; i++)
  {
    j = i + 1;
    pthread_create(&thread, NULL, worker, (void *)j);
    pthread_detach(thread);
  }
}

// Get host information (used to establishConnection)
struct addrinfo *getHostInfo(char *host, char *port)
{
  int r;
  struct addrinfo hints, *getaddrinfo_res;
  // Setup hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((r = getaddrinfo(host, port, &hints, &getaddrinfo_res)))
  {
    fprintf(stderr, "[getHostInfo:21:getaddrinfo] %s\n", gai_strerror(r));
    return NULL;
  }

  return getaddrinfo_res;
}

// Establish connection with host
int establishConnection(struct addrinfo *info)
{
  if (info == NULL)
    return -1;

  int clientfd;
  for (; info != NULL; info = info->ai_next)
  {
    if ((clientfd = socket(info->ai_family,
                           info->ai_socktype,
                           info->ai_protocol)) < 0)
    {
      perror("[establishConnection:35:socket]");
      continue;
    }

    //clientfd is file descriptor that refers to the endpoint created by the socket method
    if (connect(clientfd, info->ai_addr, info->ai_addrlen) < 0)
    {
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
void GET(int clientfd, char *path)
{
  char req[1000] = {0};
  sprintf(req, "GET %s HTTP/1.0\r\n\r\n", path);
  send(clientfd, req, strlen(req), 0);
}

static void *client_worker_fifo(void *arg)
{
  char buf[BUF_SIZE];
  //each thread sends a reuest to the server
  /*1. request the same file*/
  while (1)
  {
    pthread_mutex_lock(&(client_pool->work_mutex));
    //GET request
    GET(client_pool->clientfd, client_pool->filename);
    //RELEASES TO ANOTHER THREAD
    pthread_mutex_unlock(&(client_pool->work_mutex));
    //RECEIVE
    while (recv(client_pool->clientfd, buf, BUF_SIZE, 0) > 0)
    {
      fputs(buf, stdout);
      memset(buf, 0, BUF_SIZE);
    }
    //BLOCK AT THE BARRIER
    pthread_barrier_wait(&barrier);
    //then repeat
  }
  return NULL;
}

static void *client_worker_concur(void *arg)
{
  char buf[BUF_SIZE];
  while (1)
  {
    GET(client_pool->clientfd, client_pool->filename);
    while (recv(client_pool->clientfd, buf, BUF_SIZE, 0) > 0)
    {
      fputs(buf, stdout);
      memset(buf, 0, BUF_SIZE);
    }
    //BLOCK AT THE BARRIER
    pthread_barrier_wait(&barrier);
    //then repeat
  }
  return NULL;
}

int main(int argc, char **argv)
{
  int schedalg;
  //int clientfd;

  //int port, threads;
  //struct sockaddr_in server; //socket information about the server
  //struct sockaddr_in client; //socket information about

  if (argc != 6)
  {
    fprintf(stderr, "USAGE: %s <hostname> <port> <threads> <schedalg> <filename1> <filename2> \n", argv[0]);
    return 1;
  }
  /*Initialize a thread pool*/
  //Set the schedalg:
  if (strcmp(argv[4], "CONCUR") == 0)
    schedalg = CONCUR;
  else if (strcmp(argv[4], "FIFO") == 0)
    schedalg = FIFO;
  else
    schedalg = FIFO; //FIXME: invalid schedalg;

  //allocate memory for the client_pool
  client_pool = malloc(sizeof(tpool_t));

  switch (schedalg)
  {
  case CONCUR:
    tpool_init_client(client_pool, atoi(argv[3]), client_worker_concur, argv[1], argv[2], argv[5]);
    break;
  case FIFO:
    tpool_init_client(client_pool, atoi(argv[3]), client_worker_fifo, argv[1], argv[2], argv[5]);
    break;
  default:
    exit(0);
  }

  /* The program ./client is run w/the following args
  argv[1] ==> [host] 
  argv[2] ==> [portnum] 
  argv[3] ==> [threads]
  argv[4] ==> [schedalg]
  argv[5] ==> [filename1]
  argv[6] ==> [filename2]
  */
  return 0;

} //end of main