#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define VERSION 25
#define BUFSIZE 8096
#define ERROR      42
#define LOG        44
#define FORBIDDEN 403
#define NOTFOUND  404

//Scheduling Definitions:
//TODO: make this a cmd arg?
#define ANY 0
#define FIFO 1
#define HPIC 2
#define HPHC 3

typedef struct{
	int job_id;
	int job_fd; // the socket file descriptor   
	// what other stuff needs to be here eventually?
} job_t;

typedef struct {
	job_t *jobBuffer; // array of server Jobs on heap   
	size_t buf_capacity;
	size_t head; // position of writer   
	size_t tail; // position of reader   
	pthread_mutex_t work_mutex;    
	pthread_cond_t c_cond; // P/C condition variables    
	pthread_cond_t p_cond;
} tpool_t;

static tpool_t the_pool; // one pool to rule them all

typedef void *(worker_fn)(void *);

/*You must have a master thread that begins by creating 
 pool of worker threads, the number of which is specified on the command line.
 The 2nd and 3rd args are taken from the command line - ARI*/
void tpool_init(tpool_t *tm, size_t num_threads, size_t buf_size, worker_fn *worker){
	pthread_t thread;
	size_t i;
	pthread_mutex_init(&(tm->work_mutex), NULL);//create a mutex
	pthread_cond_init(&(tm->p_cond), NULL);//
	pthread_cond_init(&(tm->c_cond), NULL);

	// initialize buffer to empty condition    
	tm->head = tm->tail = 0;    
	tm->buf_capacity = buf_size;   

	//FIXED: CALLOC_ACTUAL_BUFFER_SPACE_ON_HEAP
	tm->jobBuffer = (job_t*)calloc(tm->buf_capacity, sizeof(job_t));
	//https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/

	for (i = 0; i < num_threads; i++){
		pthread_create(&thread, NULL, worker, (void *)i + 1);
		pthread_detach(thread); // make non-joinable    
	}
}
/*This is the consumer. Each thread is consuming a 'job' and is performing that job by DO_THE_WORK*/
static void *tpool_worker(void *arg)
{
	tpool_t *tm = &the_pool;
	int my_id = (int) arg;
	while (1) {
		job_t *job;
		pthread_mutex_lock(&(tm->work_mutex));//"A thread wishing to enter the critical region first tries to lock the associated mutex" -our sefer
		while (tm->buf_capacity == 0) //AKA: THERE_IS_NO_WORK_TO_BE_DONE and thus we should block til there is work to be done
			pthread_cond_wait(&(tm->c_cond), &(tm->work_mutex)); //release the work_mutex, "a worker thread must wait if the buffer is empty." says the doc
		
		/* Once the worker thread wakes, it performs the read on the network descriptor,
		obtains the specified content (by reading the specified static file),
		 and then returns the content to the client by writing to the descriptor*/

		//possibly make a call to getJob() depending on what the scheduling procedure is, fix line below
		job = schedulingPolicy(*tm);//REMOVE_JOB_FROM_BUFFER
		pthread_mutex_unlock(&(tm->work_mutex));//release the mutex
		web(job->job_fd, my_id); //call web() plus ?? -VAN KELLY SHLI?TA SPECIAL
		
		/*After the work has been done on the job, lock the mutex and enter the critical zone to see if the producer needs to be woken up,
		if he needs to be woken up, that is, when the buffer is empty then call cond_signal*/
		pthread_mutex_lock(&(tm->work_mutex));
		if (tm->buf_capacity == 0) //SHOULD_WAKE_UP_THE_PRODUCER, wake up producer when the buffer needs some stuff that the producer can produce
			pthread_cond_signal(&(tm->p_cond));        
		pthread_mutex_unlock(&(tm->work_mutex));    
	}  
	return NULL;
}

/*This is, lich'ora, the producer, AKA where the master thread will hand off jobs to the other threads*/
bool tpool_add_work(tpool_t * tm, job_t job){
pthread_mutex_lock(&(tm->work_mutex));
/*While THE_BUFFER_IS_FULL*/
while (tm->buf_capacity == ) {
	pthread_cond_wait(&(tm->p_cond), &(tm->work_mutex));//wait for a signal that the producer should wake up (see the pthread_cond_signal in the previous method)
	}
	 /*ADD_JOB_TO_BUFFER -> add a job at the tail, seemingly*/
	tm->jobBuffer[tm->tail++] = job;//go to the next open entry in the buffer, designated by tail, and shove the job there
	//REMEMBER, the tail could be zero, but we will never have the head>tail cuz then it would mean we are reading data that hasn't been inserted
	
	// Wake the Keystone Cops!! (improve this eventually), what the hell is a keystone cop...I think this is a Kelly note not a mank-the-tank ha'arah   
	pthread_cond_broadcast(&(tm->c_cond));    
	pthread_mutex_unlock(&(tm->work_mutex));  
	return true;
}

struct {
	char *ext;
	char *filetype;
} extensions[] = {
	{"gif", "image/gif"},
	{"jpg", "image/jpg"},
	{"jpeg", "image/jpeg"},
	{"png", "image/png"},
	{"ico", "image/ico"},
	{"zip", "image/zip"},
	{"gz", "image/gz"},
	{"tar", "image/tar"},
	{"htm", "text/html"},
	{"html", "text/html"},
	{0, 0}
	};

	static const char *HDRS_FORBIDDEN = "HTTP/1.1 403 Forbidden\nContent-Length: 185\nConnection: close\nContent-Type: text/html\n\n<html><head>\n<title>403 Forbidden</title>\n</head><body>\n<h1>Forbidden</h1>\nThe requested URL, file type or operation is not allowed on this simple static file webserver.\n</body></html>\n";
	static const char *HDRS_NOTFOUND = "HTTP/1.1 404 Not Found\nContent-Length: 136\nConnection: close\nContent-Type: text/html\n\n<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\nThe requested URL was not found on this server.\n</body></html>\n";
	static const char *HDRS_OK = "HTTP/1.1 200 OK\nServer: nweb/%d.0\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n";
	static int dummy; //keep compiler happy

	void logger(int type, char *s1, char *s2, int socket_fd) {
		//socket_fd is the socket file descriptor
		int fd;
		char logbuffer[BUFSIZE * 2];

		switch (type) {
			case ERROR:
				(void)sprintf(logbuffer, "ERROR: %s:%s Errno=%d exiting pid=%d", s1, s2, errno, getpid());
				break;
			case FORBIDDEN:
				dummy = write(socket_fd, HDRS_FORBIDDEN, 271);
				(void)sprintf(logbuffer, "FORBIDDEN: %s:%s", s1, s2);
				break;
			case NOTFOUND:
				dummy = write(socket_fd, HDRS_NOTFOUND, 224);
				(void)sprintf(logbuffer, "NOT FOUND: %s:%s", s1, s2);
				break;
			case LOG:
				(void)sprintf(logbuffer, " INFO: %s:%s:%d", s1, s2, socket_fd);
				break;
			}
			/* No checks here, nothing can be done with a failure anyway */
		if ((fd = open("nweb.log", O_CREAT | O_WRONLY | O_APPEND, 0644)) >= 0)
		{
			dummy = write(fd, logbuffer, strlen(logbuffer));
			dummy = write(fd, "\n", 1);
			(void)close(fd);
		}
	}

			/* this is a child web server process, so we can exit on errors -VAN KELLY SHLI?TA*/
		void web(int fd, int hit) {
			int j, file_fd, buflen;
			long i, ret, len;
			char *fstr;
			static char buffer[BUFSIZE + 1]; /* static so zero filled */

			ret = read(fd, buffer, BUFSIZE); /* read Web request in one go */
			if (ret == 0 || ret == -1)
			{ /* read failure stop now */
				logger(FORBIDDEN, "failed to read browser request", "", fd);
				goto endRequest;
			}
			if (ret > 0 && ret < BUFSIZE)
			{					 /* return code is valid chars */
				buffer[ret] = 0; /* terminate the buffer */
			}
			else
			{
				buffer[0] = 0;
			}
			for (i = 0; i < ret; i++)
			{ /* remove CF and LF characters */
				if (buffer[i] == '\r' || buffer[i] == '\n')
				{
					buffer[i] = '*';
				}
			}
			logger(LOG, "request", buffer, hit);//LOG == 44
			//Compare the first 4 characters of buffer and see if it matches GET
			if (strncmp(buffer, "GET ", 4) && strncmp(buffer, "get ", 4))
			{
				logger(FORBIDDEN, "Only simple GET operation supported", buffer, fd);
				goto endRequest;
			}
			for (i = 4; i < BUFSIZE; i++)
			{ /* null terminate after the second space to ignore extra stuff */
				if (buffer[i] == ' ')
				{ /* string is "GET URL " +lots of other stuff */
					buffer[i] = 0;
					break;
				}
			}
			for (j = 0; j < i - 1; j++)
			{ /* check for illegal parent directory use .. */
				if (buffer[j] == '.' && buffer[j + 1] == '.')
				{
					logger(FORBIDDEN, "Parent directory (..) path names not supported", buffer, fd);
					goto endRequest;
				}
			}
			if (!strncmp(&buffer[0], "GET /\0", 6) || !strncmp(&buffer[0], "get /\0", 6))
			{ /* convert no filename to index file */
				(void)strcpy(buffer, "GET /index.html");
			}
				/* work out the file type and check we support it */
			buflen = strlen(buffer);
			fstr = (char *)0;
			for (i = 0; extensions[i].ext != 0; i++)
			{
				len = strlen(extensions[i].ext);
				if (!strncmp(&buffer[buflen - len], extensions[i].ext, len))
				{
					fstr = extensions[i].filetype;
					break;
				}
			}
			if (fstr == 0)
			{
				logger(FORBIDDEN, "file extension type not supported", buffer, fd);
			}
			if ((file_fd = open(&buffer[5], O_RDONLY)) == -1)
			{ /* open the file for reading */
				logger(NOTFOUND, "failed to open file", &buffer[5], fd);
				goto endRequest;
			}
			logger(LOG, "SEND", &buffer[5], hit);
			len = (long)lseek(file_fd, (off_t)0, SEEK_END); /* lseek to the file end to find the length */
			(void)lseek(file_fd, (off_t)0, SEEK_SET);		/* lseek back to the file start ready for reading */
			/* print out the response line, stock headers, and a blank line at the end. */
			(void)sprintf(buffer, HDRS_OK, VERSION, len, fstr);
			logger(LOG, "Header", buffer, hit);
			dummy = write(fd, buffer, strlen(buffer));
				/* send file in 8KB block - last block may be smaller */
			while ((ret = read(file_fd, buffer, BUFSIZE)) > 0)
			{
				dummy = write(fd, buffer, ret);
			}
		endRequest:
			sleep(1); /* allow socket to drain before signalling the socket is closed */
			close(fd);
		}

		void schedulingPolicy(int policy){
			switch (policy)
			{
			case ANY:
				/* fall through */
			case FIFO:
				//get oldest item in buffer and work on it
				break;
			case HPIC:
				//Highest priority to image content- dont do any other work if img is available
				break;
			case HPHC:
				//highest priority to HTML content - dont do any other work if HTML is available
				break;
			
			default:
				break;
			}

		}
		
		int main(int argc, char **argv){
			int i, port, listenfd, socketfd, hit;
			socklen_t length;
			static struct sockaddr_in cli_addr;  /* static = initialised to zeros */
			static struct sockaddr_in serv_addr; /* static = initialised to zeros */
			if (argc < 3 || argc > 3 || !strcmp(argv[1], "-?"))
			{
				(void)printf("USAGE: %s <port-number> <top-directory>\t\tversion %d\n\n"
								 "\tnweb is a small and very safe mini web server\n"
								 "\tnweb only servers out file/web pages with extensions named below\n"
								 "\t and only from the named directory or its sub-directories.\n"
								 "\tThere is no fancy features = safe and secure.\n\n"
								 "\tExample: nweb 8181 /home/nwebdir &\n\n"
								 "\tOnly Supports:",
								 argv[0], VERSION);
				for (i = 0; extensions[i].ext != 0; i++)
					(void)printf(" %s", extensions[i].ext);
					(void)printf("\n\tNot Supported: URLs including \"..\", Java, Javascript, CGI\n"
							 "\tNot Supported: directories / /etc /bin /lib /tmp /usr /dev /sbin \n"
							 "\tNo warranty given or implied\n\tNigel Griffiths nag@uk.ibm.com\n");
			exit(0);
			}
			if (!strncmp(argv[2], "/", 2) || !strncmp(argv[2], "/etc", 5) ||
				!strncmp(argv[2], "/bin", 5) || !strncmp(argv[2], "/lib", 5) ||
				!strncmp(argv[2], "/tmp", 5) || !strncmp(argv[2], "/usr", 5) ||
				!strncmp(argv[2], "/dev", 5) || !strncmp(argv[2], "/sbin", 6))
			{
				(void)printf("ERROR: Bad top directory %s, see nweb -?\n", argv[2]);
				exit(3);
			}
			if (chdir(argv[2]) == -1)
			{
				(void)printf("ERROR: Can't Change to directory %s\n", argv[2]);
				exit(4);
			}
				logger(LOG, "nweb starting", argv[1], getpid());
			/* setup the network socket */
			if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				logger(ERROR, "system call", "socket", 0);
			}
			port = atoi(argv[1]);
			if (port < 1025 || port > 65000)
			{
				logger(ERROR, "Invalid port number (try 1025->65000)", argv[1], 0);
			}
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			serv_addr.sin_port = htons(port);
			if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
			{
				logger(ERROR, "system call", "bind", 0);
			}
			if (listen(listenfd, 64) < 0)
			{
				logger(ERROR, "system call", "listen", 0);
			}
			/*The basic web server that we start with is a single-threaded server 
			that enters an infinite loop to handle sequential requests*/
			for (hit = 1;; hit++)
			{
				length = sizeof(cli_addr);
				//if the client connects to a socket, then skip this if-statement
				if ((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0)
				{
					logger(ERROR, "system call", "accept", 0);
				}
				web(socketfd, hit); /* this is where the action happens */
			}
		}
